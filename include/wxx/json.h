#ifndef h_5C60AA89922049A8BFD8425ED4AD11A7 
#define h_5C60AA89922049A8BFD8425ED4AD11A7 

#include <wxx/func.h>

namespace wxx
{
	template<typename T>
	struct index_to
	{
		std::string n_;

		index_to(const s__<std::string>& i)
			:n_{
				[&] { std::ostringstream ss; ss << i; return ss.str(); }()
			}
		{}

		index_to(const s__<int>& i)
			:n_{
				[&] { std::ostringstream ss; ss << i; return ss.str(); }()
			}
		{}
	};

	template<>
	struct var<json> : s__<json>
	{
		mutable bool valid_;

		explicit var()
			:s__<json>{ 
				[]() -> std::string {
					std::ostringstream ss;
					ss << "jsn__" << get_symcode();
					return ss.str();
				}()
			}
			,valid_{false}
		{
		}

		explicit var(const std::string& name)
			:s__<json>(name)
			,valid_{false}
		{
		}

		s__<void> init(const s__<std::string>& v) const
		{
			std::ostringstream ss;
			ss << "var " << *this << "=JSON.parse(" << v << ")" << std::endl;
			valid_ = true;
			return s__<void>(ss.str());
		}

		s__<void> operator=(const s__<std::string>& v) const
		{
			std::ostringstream ss;
			ss << *this << "=JSON.parse(" << v << ")" << std::endl;
			valid_ = true;
			return s__<void>(ss.str());
		}

		template<typename T>
		s__<T> operator[](const index_to<T>& ind) const
		{
			assert(valid_);
			std::ostringstream ss;
			ss << *this << "[" << ind.n_ << "]";
			return s__<T>(ss.str());
		}
		s__<std::string> operator[](const s__<std::string>& ind) const
		{
			assert(valid_);
			std::ostringstream ss;
			ss << *this << "[" << ind << "]";
			return s__<std::string>(ss.str());
		}
	};
}

#endif
