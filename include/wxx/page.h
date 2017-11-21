#ifndef h_D11021BDEFB047C2823142551BE7CA34
#define h_D11021BDEFB047C2823142551BE7CA34

#include <wxx/tag.h>
#include <wxx/php.h>

namespace wxx
{

	struct page
	{
		std::string type_;

		std::string php_top_;
		std::string php_;
		tag html_;


		explicit page(
				const std::string& t = "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.1//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml11.dtd\">\n"
				)
			:type_{t}
			,html_{"html"}
		{
			html_ += tag{"head"};
			html_ += tag{"body"};
			{
				tag m("meta");
				m += {"http-equiv", "content-type"_v};
				m += {"content", "text/html; charset=UTF-8"_v};
				*this += m;
			}
			{
				tag m("meta");
				m += {"http-equiv", "X-UA-Compatible"_v};
				*this += m;
			}
		}

		tag& body()
		{
			return find_tag("body");
		}


		template<typename T>
		void set_title(const T& t)
		{
			tag tl("title");
			tl += t;
			*this += tl;
		}

		page& operator +=(const tag& t)
		{
			assert(t.name_ != "head" && t.name_ != "body");
			if (t.name_ == "script") {
				if (t.find_attr("src")) {
					std::ostringstream ss;
					ss << t;
					find_tag("head").injection_ += ss.str();
				}
				else {
					find_tag("head") += t;
				}
			}
			else if (t.name_ == "meta" 
					|| t.name_ == "title"
					|| t.name_ == "link"
					) {
				std::ostringstream ss;
				ss << t;
				find_tag("head").injection_ += ss.str();
			}
			else {
				body() += t;
			}
			return *this;
		}

		page& init_locale(const std::vector<std::pair<std::string, std::string>>& lc) //name -> link
		{
			{
				std::ostringstream ss;
				php_init_locale(ss, lc);
				php_inject(ss.str());
			}
			return *this;
		}

		template<typename Var>
		page& php_bind(const Var& v, const std::string& php_func)
		{
			std::ostringstream ss;
			php_bind_print(ss, v, php_func);
			php_inject(ss.str());
			return *this;
		}
		//top of the page
		page& php_top(const std::string& ps)
		{
			php_top_ += ps;
			return *this;
		}
		//injects into <head>
		page& php_inject(const std::string& ps)
		{
			php_ += ps;
			return *this;
		}

		page& add_keywords(const std::vector<std::string>& k)
		{
			std::stringstream ss;

			bool first = true;
			for (const auto& v: k) {
				if (!first) {
					ss << ",";
				}
				else {
					first = false;
				}
				ss << v;
			}
			tag m("meta");
			m += {"name", "keywords"_v};
			m += {"content", val<std::string>(ss.str())};
			*this += m;
			return *this;
		}

		template< typename S >
		void print(S& s) const
		{
			if (!php_top_.empty()) {
				s << php << php_top_ << phpend << std::endl;
			}
			if (!type_.empty())
				s << type_;
			if (!php_.empty()) {
				std::ostringstream ss;
				ss << php << php_ << phpend << std::endl;
				find_tag("head").injection_ += ss.str();
			}
			s << html_;
		}

	private:
		tag& find_tag(const std::string& n)
		{
			return *std::find_if(html_.tags_.begin(), html_.tags_.end(), [&n](const tag& v) ->bool { return v.name_ == n; });
		}
		const tag& find_tag(const std::string& n) const
		{
			return *std::find_if(html_.tags_.begin(), html_.tags_.end(), [&n](const tag& v) ->bool { return v.name_ == n; });
		}
	};

}

#endif
