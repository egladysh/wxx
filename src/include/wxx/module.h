#ifndef h_9F10284A6EE541B18973B9B21ACEEDED
#define h_9F10284A6EE541B18973B9B21ACEEDED

#include <wxx/func.h>
#include <wxx/tag.h>

namespace wxx
{
	template<typename S, typename Ret>
		struct module__
		{
			S& s_;
			module__(S& s)
				:s_{s}
			{}

			template<typename St, typename Tup, std::size_t I, std::size_t N, bool stop> struct init_tuple;
			template<typename St, typename Tup, std::size_t I=0, std::size_t N = std::tuple_size<Tup>::value, bool stop = (I == N)>
				struct print_tuple
				{
					explicit print_tuple(St& s, const Tup& t)
					{
						s << std::get<I>(t) << ";" << std::endl;
						print_tuple<St, Tup, I+1>{s, t};
					}
				};

			template<typename St, typename Tup, std::size_t I, std::size_t N>
				struct print_tuple<St, Tup, I, N, true>
				{
					explicit print_tuple(St&, const Tup&) {}
				};

			template<typename... St>
			module__& operator()(const St&... v)
			{
				std::tuple<const St&...> t{v...};
				print_tuple<S, decltype(t)> pt(s_, t);
				return *this;
			}

			template<typename St>
			module__& operator [](const St& v)
			{
				s_ << x_{ v };
				return *this;
			}

			template<typename S1, typename C, typename S2>
			module__& for_(const S1& s1, const C& c, const S2& s2)
			{
				s_ << wxx::for_(s1, c, s2);
				return *this;
			}

			module__& break_()
			{
				s_ << wxx::break_;
				return *this;
			}
			module__& continue_()
			{
				s_ << wxx::continue_;
				return *this;
			}

			template<typename T>
			module__& if_(const T& v)
			{
				s_ << wxx::if_(v);
				return *this;
			}

			template<typename T>
			module__& elseif_(const T& v)
			{
				s_ << wxx::elseif_(v);
				return *this;
			}

			module__& else_()
			{
				s_ << wxx::else_;
				return *this;
			}

			module__& end_()
			{
				s_ << wxx::end_;
				return *this;
			}
			module__& endif_()
			{
				s_ << wxx::endif_;
				return *this;
			}
			module__& endfor_()
			{
				s_ << wxx::endfor_;
				return *this;
			}

			template<typename T>
			module__& return_(const T& v)
			{
				s_ << wxx::ret_<Ret>(v);
				return *this;
			}
			module__& return_()
			{
				s_ << wxx::ret_<Ret>();
				return *this;
			}
			
			module__& operator <<(const tag& t);

			template<typename T>
			var<T> make_var(const std::string& n)
			{
				var<T> v(n);
				s_ << "var " << v << ";";
				return v;
			}
			template<typename T>
			var<T> make_var()
			{
				var<T> v;
				s_ << "var " << v << ";";
				return v;
			}

			template<typename T>
			var<array_t<T>> make_array()
			{
				var<array_t<T>> v;
				s_ << "var " << v << ";";
				return v;
			}
			template<typename T>
			var<array_t<T>> make_array(const std::string& n)
			{
				var<array_t<T>> v(n);
				s_ << "var " << v << ";";
				return v;
			}
		};

	template<typename S>
		module__<S,void> module_(S& s)
		{
			return module__<S,void>(s);
		}

	template<typename S, typename F>
		struct def__ : module__<S, typename F::result>
		{
			using base_t = module__<S, typename F::result>;
			using base_t::s_;

			F& f_;
			def__(S& s, F& f)
				:base_t{s}
				,f_{f}
			{
				s_ << f_ << begin_;
			}

			~def__()
			{
				s_ << wxx::end_;
			}
			
		};


		template<typename S, typename F>
			def__<S,F> def_(S& s, F&f)
			{
				return def__<S, F>(s,f);
			}
}
#endif
