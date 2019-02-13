#ifndef h_AD924C7AB79D44238808E8FA3A250F2E
#define h_AD924C7AB79D44238808E8FA3A250F2E

#include <vector>
#include <wxx/xml.h>
#include <wxx/jstd.h>

namespace wxx
{
namespace ajax
{

	typedef func<void(xml_t, std::string)> xml_callback;


	struct param
	{
		std::string n_;
		const s__<std::string>& v_;

		/*
		param(param&& p)
			:n_(std::move(p.n_))
			,v_(std::move(p.v_))
		{}
		*/
	};

	typedef std::vector<param> params;


	template<typename S>
		auto operator<<(S& s, const params& p) -> decltype(s) 
		{ 
			bool first = true;
			for (const auto& v: p) {
				if (!first) {
					s << "+'&";
				}
				else {
					first = false;
					s << "'";
				}
				s << v.n_ << "='+" << jstd::encodeURI()(v.v_);
			}
			return s;
		} 

	struct xml_request
	{
		s__<std::string> t_;
		const params& p_;
		s__<xml_callback> cb_;
		

		explicit xml_request(s__<std::string> target, const params& p, s__<xml_callback> cb)
			:t_(std::move(target))
			,p_(p)
			,cb_(cb)
		{}
		explicit xml_request(s__<std::string> target, const params& p, const xml_callback& cb)
			:t_(std::move(target))
			,p_(p)
			,cb_(cb.forward())
		{}
		explicit xml_request(const std::string& target, const params& p, const xml_callback& cb)
			:t_{std::move(val<std::string>(target))}
			,p_(p)
			,cb_(cb.forward())
		{}

		template< typename S >
		void print(S& s) const
		{
			func<void(std::string, std::string, xml_callback)> f("do_xml_ajax"); //in utils.js
			std::ostringstream ss;
			ss << p_;

			s << f(t_, var<std::string>(ss.str()), cb_);
		}
	};

	


}
}
#endif
