#ifndef h_CA8129137949409DBB6BA0F8EC0D70AE
#define h_CA8129137949409DBB6BA0F8EC0D70AE

#include <regex>
#include <vector>
#include <iostream>
#include <algorithm>
#include <wxx/func.h>
#include <wxx/jstd.h>

namespace wxx
{
	template< typename T >
	struct entity : s__<T>
	{
		explicit entity(const T& v)
			:s__<T>{ 
				[&v] {
					std::ostringstream ss;
					ss << v;
					return ss.str();
				}()
			}
		{
		}
	};

	template<>
	struct entity<std::string> : s__<std::string>
	{
		explicit entity(const std::string& v)
			:s__<std::string>{xml_escape(v)}
		{
		}
	};

	inline entity<std::string> operator "" _ent(const char* p, size_t n)
	{
		return entity<std::string>(std::string(p, p+n));
	}

	struct tag_content__t;
	struct value_content__t;

	template<typename Type>
	struct content
	{
		template<typename T, typename Tag>
			struct is_static
			{
				enum 
				{
					value = false
				};
			};

			//must not use JS constants with xml content
		template<typename T>
			struct is_static<entity<T>, tag_content__t>
			{
				enum 
				{
					value = true
				};
			};

		template<typename T>
			struct is_static<val<T>, value_content__t>
			{
				enum 
				{
					value = true
				};
			};

		bool static_;
		std::string v_;

		content()
			:static_(true)
		{ }

		template<typename T>
		content(const entity<T>& v)
			:static_(is_static<entity<T>, Type>::value)
			,v_{ 
				[&v] {
					std::stringstream ss;
					ss << v;
					return ss.str();
				}()
			}
		{
		}
		template<typename T>
		content(const val<T>& v)
			:static_(is_static<val<T>, Type>::value)
			,v_{ 
				[&v] {
					std::stringstream ss;
					ss << v;
					return ss.str();
				}()
			}
		{
		}
		template<typename T>
		content(const s__<T>& v)
			:static_(is_static<s__<T>, Type>::value)
			,v_{ 
				[&v] {
					std::stringstream ss;
					ss << v;
					return ss.str();
				}()
			}
		{
		}

		bool empty() const
		{
			return v_.empty();
		}

		content& operator +=(const content& c) 
		{
			assert(static_ && c.static_);
			if (c.v_.empty())
				return *this;

			if (std::is_same<Type, value_content__t>::value) {
				std::string tmp;
				if (v_.empty()) {
					v_ += c.v_;
				}
				else {
					std::string tmp = c.v_;
					tmp.erase(tmp.begin());
					v_.erase(--v_.end());
					v_ += tmp;
				}
			}
			else {
				v_ += c.v_;
			}
			return *this;
		}

		template< typename S >
		void print(S& s) const
		{
			s << v_;
		}
	};

	using tag_content = content<tag_content__t>;
	using val_content = content<value_content__t>;


	struct tag_attr
	{
		std::string n_;
		val_content v_;

		tag_attr(const std::string& n, const std::string& v)
			:n_{n}
			,v_{val<std::string>(v)} 
		{
			assert(!n_.empty());
		}
		tag_attr(const std::string& n, const char* v)
			:n_{n}
			,v_{val<std::string>(v)} 
		{
			assert(!n_.empty());
		}

		template<typename T>
		tag_attr(const std::string& n, const T& v)
			:n_{n}
			,v_{v} 
		{
			assert(!n_.empty());
		}

		tag_attr& operator += (const tag_attr& a)
		{
			assert(n_ == a.n_);
			if (v_.empty())
				v_ = a.v_;
			else
				v_ += a.v_;
			return *this;
		}

		template< typename S >
		void print(S& s) const
		{
			s << n_;
			if (!v_.empty()) {
				s << "=" << v_;
			}
		}
	};

	struct tag
	{
		std::string name_;
		mutable std::vector<tag_attr> attrs_;
		tag_content cont_;
		std::vector<tag> tags_;
		mutable std::string injection_;

		explicit tag(const std::string& n)
			:name_(n)
		{
			assert(!n.empty());
		}

		tag_attr* find_attr(const std::string& name)
		{
			auto it = std::find_if(attrs_.begin(), attrs_.end(), [&name](const tag_attr& v) -> bool { return v.n_ == name; });
			if (it == attrs_.end()) {
				return nullptr;
			}
			return &*it;
		}
		const tag_attr* find_attr(const std::string& name) const
		{
			auto it = std::find_if(attrs_.begin(), attrs_.end(), [&name](const tag_attr& v) -> bool { return v.n_ == name; });
			if (it == attrs_.end()) {
				return nullptr;
			}
			return &*it;
		}

		tag& operator +=(const tag_attr& a)
		{
			tag_attr* pf = find_attr(a.n_);
			if (pf) {
				assert(a.n_ != "id"); //don't want adding id's
				*pf += a;
			}
			else {
				attrs_.push_back(a);
			}
			return *this;
		}

		tag& operator +=(const tag_content& c)
		{
			assert(tags_.empty());
			if (cont_.empty()) {
				cont_ = c;
			}
			else  {
				cont_ += c;
			}
			return *this;
		}

		tag& operator +=(const tag& t)
		{
			assert(cont_.empty());
			tags_.push_back(t);
			return *this;
		}

		template< typename S >
		void print(S& s) const;

	private:
		bool is_dynamic() const;
	};


	struct script : tag
	{
		script()
			:tag("script")
		{
			*(tag*)this += {"language", "JavaScript"_v};
		}
		script(const std::string& s)
			:tag("script")
		{
			*(tag*)this += {"language", "JavaScript"_v};
			injection_ += s;
		}
		script& operator+=(const std::string& s)
		{
			injection_ += s;
			return *this;
		}
	};


	struct table_element
	{
		const tag& t_;
		const s__<int>& ind_; 
	};

	struct js_stream
	{
		std::ostream& s_;

		js_stream(std::ostream& s)
			:s_{s}
			,table_row_{0}
			,table_cell_{0}
		{}

		js_stream& operator<<(const tag& t);
		js_stream& operator<<(const table_element& t);

	private:
		int table_row_;
		int table_cell_;
	};


	struct tag;

	template<>
	struct var<tag_t> : s__<tag_t>
	{
		explicit var()
			:s__<tag_t>{ 
				[]() -> std::string {
					std::ostringstream ss;
					ss << "vt__" << get_symcode();
					return ss.str();
				}()
			}
		{
		}
		explicit var(const std::string& name)
			:s__<tag_t>(name)
		{
		}

		s__<void> init() const
		{
			std::ostringstream ss;
			ss << "var " << *this;
			return s__<void>(ss.str());
		}

		inline s__<void> operator=(const tag& v) const;
		inline s__<void> operator+=(const table_element& v) const;


		s__<void> operator=(const s__<std::string>& v) const
		{
			std::ostringstream ss;
			ss << *this << "=document.getElementById(" << v << ")";
			return s__<void>(ss.str());
		}

		s__<void> operator=(const val_content& v) const
		{
			std::ostringstream ss;
			ss << *this << "=document.getElementById(" << v << ")";
			return s__<void>(ss.str());
		}
		s__<void> operator=(const s__<tag_t>& v) const
		{
			std::ostringstream ss;
			ss << *this << "=" << v;
			return s__<void>(ss.str());
		}
		s__<void> operator=(const var<tag_t>& v) const
		{
			std::ostringstream ss;
			ss << *this << "=" << v;
			return s__<void>(ss.str());
		}
		template<typename T>
		var<T> m_(const std::string& mem)
		{
			std::ostringstream ss;
			ss << *this << "." << mem;
			return var<T>(ss.str());
		}

		s__<std::string> text() const
		{
			std::ostringstream ss;
			ss << *this << ".innerText";
			return s__<std::string>(ss.str());
		}

		s__<void> remove()
		{
			std::ostringstream ss;
			ss << *this << ".parentNode.removeChild(" << *this << ")";
			return s__<void>(ss.str());
		}

		s__<void> display(const std::string& mode)
		{
			std::ostringstream ss;
			ss << *this << ".style.display='" << mode << "'";
			return s__<void>(ss.str());
		}

		s__<void> submit()
		{
			std::ostringstream ss;
			ss << *this << ".submit()";
			return s__<void>(ss.str());
		}

		s__<void> append(const var& v)
		{
			std::ostringstream ss;
			ss << *this << ".appendChild(" << v << ")";
			return s__<void>(ss.str());
		}

		s__<int> width() const
		{
			std::ostringstream ss;
			ss << *this << ".offsetWidth";
			return s__<int>(ss.str());
		}

	};

	inline s__<array_t<tag_t>> document_get_by_tagname(const std::string& v)
	{
		std::ostringstream ss;
		ss << "document.getElementsByTagName(" << val<std::string>(v) << ")";
		return s__<array_t<tag_t>>(ss.str());
	}

	struct set_attribute
	{
		const s__<tag_t> &n_;
		const tag_attr& a_;
		
		set_attribute(const s__<tag_t>& node, const tag_attr& a)
			:n_{node}
			,a_(a)
		{}

		template< typename S >
		void print(S& s) const
		{
			bool event = false;
			if (a_.n_ == "onclick" || a_.n_ == "onchange")
				event = false;
			s << n_ << ".setAttribute('" << a_.n_ << "'," 
				<< (event?"'":"")
				<< a_.v_ 
				<< (event?"'":"")
				<< ")";

			if (a_.n_ == "style") {
				s << ";if( typeof browser !== 'undefined' && browser=='IE' ) { " << n_ << ".style.setAttribute(";
				s << "\"" << "cssText" << "\", " << a_.v_ << "); } ";
			}
			else if (a_.n_ == "class") {
				s << ";" << n_ << ".setAttribute('" << "className" << "'," 
					<< a_.v_ 
					<< ")";
			}
		}
	};

	struct set_inner
	{
		const s__<tag_t> &n_;
		const tag_content c_;
		
		set_inner(const s__<tag_t>& node, const tag_content& c)
			:n_{node}
			,c_(c)
		{}

		template< typename S >
		void print(S& s) const
		{
			s << n_ << ".innerHTML=" << c_;
		}
	};



	template< typename S >
		void tag::print(S& s) const
		{
			if (is_dynamic()) {
				const tag_attr* pid = find_attr("id"); //generate auto id for dynamic stuff
				if (!pid) {
					std::ostringstream ss;
					ss << "autoid__" << get_symcode();
					attrs_.push_back({"id", val<std::string>(ss.str())});
				}
				else {
					assert(pid->v_.static_); //id must be static for dynamic modifications
				}
			}
			//content or tags
			//assert(!(cont_.empty() && !tags_.empty()));
			s << "<" << name_;
			for (const auto& a : attrs_) {
				if (a.v_.static_) {
					s << " " << a;
				}
			}
			if (name_ == "meta") {
				assert(tags_.empty());
				s << "/>" << std::endl;
				return;
			}

			s << ">";

			s << injection_;

			if (!cont_.empty() && cont_.static_)
				s << cont_;

			for (const auto& t: tags_) {
				s << t;
			}
			s << "</" << name_ << ">" << std::endl;
			//do the dyanmic stuff
			//
			if(is_dynamic())
			{
				std::ostringstream ss;
				var<tag_t> tt;
				const tag_attr* pid = find_attr("id");
				assert(pid);


				ss << (tt = pid->v_) << ";";

				for (const auto& v: attrs_) {
					if (!v.v_.static_) {
						ss << set_attribute(tt, v) << ";" << std::endl;
					}
				}

				if (!cont_.static_) {
					ss << set_inner(tt, cont_) << ";" << std::endl;
				}

				if (!ss.str().empty())
					s << script(ss.str());
			}
		}
	
		inline s__<void> var<tag_t>::operator=(const tag& t) const
		{
			std::ostringstream ss;
			js_stream js(ss);
			js << t;
			ss << *this << "=tc()" << std::endl;
			if (t.name_ == "table") {
				ss << "; var table=" << *this;
			}
			return s__<void>(ss.str());
		}
		inline s__<void> var<tag_t>::operator+=(const table_element& v) const
		{
			std::ostringstream ss;
			js_stream js(ss);
			js << v;
			if (v.t_.name_ == "tr") {
				ss << "var row=tc()";
			}
			else {
				ss << "tc()";
			}
			return s__<void>(ss.str());
		}

}

#endif
