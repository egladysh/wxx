#ifndef h_6F0F813A296342E8894139374BC335F8
#define h_6F0F813A296342E8894139374BC335F8

#include <wxx/func.h>

namespace wxx
{
	struct xml_t;

	template<>
	struct s__<xml_t> : sb__
	{
		typedef xml_t value_type;

		using sb__::n_;

		explicit s__(const std::string& n)
			:sb__{n}
		{
		}

		s__<array_t<xml_t>> find_all(const s__<std::string>& tagname) const
		{
			std::ostringstream ss;
//			ss << *this << ".getElementsByTagName(" << tagname << ")";
			ss << "get_tags_by_name(" << *this << "," << tagname << ")";
			return s__<array_t<xml_t>>{ss.str()};
		}

		s__<array_t<xml_t>> childnodes() const
		{
			std::ostringstream ss;
			ss << *this << ".childNodes";
			return s__<array_t<xml_t>>{ss.str()};
		}

		s__<std::string> node_value() const
		{
			return s__<std::string>(childnodes().n_ + "[0].nodeValue");
		}

		s__<std::string> get_attr(const s__<std::string>& n)
		{
			std::ostringstream ss;
			ss << *this << ".getAttribute(" << n << ")";
			return s__<std::string>(ss.str());
		}
	};

	template<>
	struct var<xml_t> : s__<xml_t>
	{
		mutable bool valid_;

		explicit var()
			:s__<xml_t>{ 
				[]() -> std::string {
					std::ostringstream ss;
					ss << "vx__" << get_symcode();
					return ss.str();
				}()
			}
			,valid_{false}
		{
		}
		explicit var(const std::string& name)
			:s__<xml_t>(name)
			,valid_{false}
		{
		}

		s__<void> operator=(const var& v)
		{
			std::ostringstream ss;
			ss << *this << "=" << v;
			return s__<void>(ss.str());
		}

		s__<void> operator=(const s__<xml_t>& v)
		{
			std::ostringstream ss;
			ss << *this << "=" << v;
			return s__<void>(ss.str());
		}
	};
}

#endif
