#ifndef h_7A09E5250A354BC09A70DF6244C3C34F
#define h_7A09E5250A354BC09A70DF6244C3C34F

#include <wxx/s.h>
#include <regex>
#include <tuple>
#include <algorithm>
#include <utility>
#include <iostream>

namespace wxx
{
	struct lambda_t
	{
		std::string body_;

		explicit lambda_t(const std::string& b)
			:body_(b)
		{}

		template< typename S >
		void print(S& s) const
		{
			s << body_;
		}
	};
	template<typename S>
		auto operator<<(S& os, const lambda_t& t) -> decltype(t.print(os), os) 
		{ 
			t.print(os); 
			return os; 
		} 


	template<typename T>
		inline const char* get_var_prefix()
		{
			static const char n[] = "v__";
			return n;
		}
	template<>
		inline const char* get_var_prefix<int>()
		{
			static const char n[] = "vi__";
			return n;
		}
	template<>
		inline const char* get_var_prefix<std::string>()
		{
			static const char n[] = "vs__";
			return n;
		}
	template<>
		inline const char* get_var_prefix<float>()
		{
			static const char n[] = "vf__";
			return n;
		}
	template<>
		inline const char* get_var_prefix<bool>()
		{
			static const char n[] = "vb__";
			return n;
		}
	
	template< typename T >
	struct val : s__<T>
	{
		explicit val(const T& v)
			:s__<T>{ 
				[&v] {
					std::ostringstream ss;
					ss << v;
					return ss.str();
				}()
			}
		{}
	};

	template<>
	struct val<std::string> : s__<std::string>
	{
		explicit val(const std::string& v)
			:s__<std::string>{ 
				[&v] {
					std::ostringstream ss;
					std::string s1 = std::regex_replace( v, std::regex("'"), "\"" );
					s1 = std::regex_replace( s1, std::regex("\\\\"), "\\\\" );
					ss << "\'" << s1 << "\'";
					return ss.str();
				}()
			}
		{}
	};
	template<>
	struct val<char> : s__<char>
	{
		explicit val(const char& v)
			:s__<char>{ 
				[&v] {
					std::ostringstream ss;
					ss << "\'" << v << "\'";
					return ss.str();
				}()
			}
		{
		}
	};

	template<>
	struct val<bool> : s__<bool>
	{
		explicit val(bool v)
			:s__<bool>{ 
				[&v] {
					std::ostringstream ss;
					ss << (v?"true":"false"); 
					return ss.str();
				}()
			}
		{}
	};

	template< typename T>
	struct val< func<T> > : s__< func<T> >
	{
		explicit val(const func<T>& v)
			:s__< func<T> >{ 
				[&v] {
					std::ostringstream ss;
					ss << v.n_;
					return ss.str();
				}()
			}
		{}
	};

	template< typename T >
	struct var : s__<T>
	{
		explicit var()
			:s__<T>{ 
				[] {
					std::ostringstream ss;
					ss << get_var_prefix<T>() << get_symcode();
					return ss.str();
				}()
			}
		{}

		explicit var(const std::string& constant)
			:s__<T>(constant)
		{
		}

		s__<void> init()
		{
			std::ostringstream ss;
			ss << "var " << *this << "=null";
			return s__<void>(ss.str());
		}

		s__<void> init(const s__<T>& vv)
		{
			std::ostringstream ss;
			ss << "var " << *this << "=" << vv;
			return s__<void>(ss.str());
		}

		s__<void> operator=(const var& v)
		{
			std::ostringstream ss;
			ss << *this << "=" << v;
			return s__<void>(ss.str());
		}

		s__<void> operator=(const s__<T>& v)
		{
			std::ostringstream ss;
			ss << *this << "=" << v;
			return s__<void>(ss.str());
		}

		s__<void> operator+=(const s__<T>& v)
		{
			static_assert(std::is_arithmetic<T>::value, "must be arithmetic");
			std::ostringstream ss;
			ss << *this << "+=" << v;
			return s__<void>(ss.str());
		}
		s__<void> operator-=(const s__<T>& v)
		{
			static_assert(std::is_arithmetic<T>::value, "must be arithmetic");
			std::ostringstream ss;
			ss << *this << "-=" << v;
			return s__<void>(ss.str());
		}

		s__<std::string> to_string() const
		{
			std::ostringstream ss;
			ss << *this << ".toString()";
			return s__<std::string>(ss.str());
		}

		s__<array_t<std::string>> split(const s__<std::string>& d) const
		{
			static_assert(std::is_same<T, std::string>::value, "must be string");
			std::ostringstream ss;
			ss << *this << ".split(" << d << ")";
			return s__<array_t<std::string>>(ss.str());
		}

		template<typename F> 
		func<F> mf_(const std::string& n) const
		{
			std::ostringstream ss;
			ss << *this << "." << n;
			return func<F>{ss.str()};
		}
	};

	template<typename T>
	struct var< array_t<T> > : s__< array_t<T> >
	{
		typedef s__<array_t<T>> base_t;

		explicit var()
			:base_t{ 
				[] {
					std::ostringstream ss;
					ss << "a" << get_var_prefix<T>() << get_symcode();
					return ss.str();
				}()
			}
		{}

		explicit var(const std::string& constant)
			:base_t(constant)
		{
		}

		s__<void> init()
		{
			std::ostringstream ss;
			ss << "var " << *this << "=[]";
			return s__<void>(ss.str());
		}

		s__<void> init(const s__<array_t<T>>& v)
		{
			std::ostringstream ss;
			ss << "var " << *this << "=" << v;
			return s__<void>(ss.str());
		}

		s__<void> operator=(const s__<array_t<T>>& v)
		{
			std::ostringstream ss;
			ss << *this << "=" << v;
			return s__<void>(ss.str());
		}

		template<typename F> 
		func<F> mf_(const std::string& n) const
		{
			std::ostringstream ss;
			ss << *this << "." << n;
			return func<F>{ss.str()};
		}
	};

	template<typename K, typename T>
	struct var< map_t<K, T> > : s__< map_t<K, T> >
	{
		typedef s__<map_t<K, T>> base_t;

		explicit var()
			:base_t{ 
				[] {
					std::ostringstream ss;
					ss << "m" << get_var_prefix<T>() << get_symcode();
					return ss.str();
				}()
			}
		{}

		explicit var(const std::string& constant)
			:base_t(constant)
		{
		}

		s__<void> init()
		{
			std::ostringstream ss;
			ss << "var " << *this << "=[]";
			return s__<void>(ss.str());
		}

		s__<void> init(const s__<map_t<K, T>>& v)
		{
			std::ostringstream ss;
			ss << "var " << *this << "=" << v;
			return s__<void>(ss.str());
		}

		s__<void> operator=(const s__<map_t<K, T>>& v)
		{
			std::ostringstream ss;
			ss << *this << "=" << v;
			return s__<void>(ss.str());
		}

		template<typename F> 
		func<F> mf_(const std::string& n) const
		{
			std::ostringstream ss;
			ss << *this << "." << n;
			return func<F>{ss.str()};
		}
	};

	template<typename T>
		std::string format(const T& v)
		{
			std::ostringstream ss;
			ss << v;
			return ss.str();
		}

	struct arg_name
	{
		std::string n_;
		arg_name(const std::string& n)
			:n_(n)
		{}
		arg_name()
		{}
	};

	template<typename T = std::string>
		struct arg_dummy : arg_name
	{
		arg_dummy(const std::string& n)
			:arg_name(n)
		{}
		arg_dummy()
		{}
	};

	template< typename R, typename... Args >
	struct func<R(Args...)>
	{
		template<typename Tup, std::size_t I, std::size_t N, bool stop> struct init_tuple;
		template<typename Tup, std::size_t I=0, std::size_t N = std::tuple_size<Tup>::value, bool stop = (I == N)>
			struct init_tuple
			{
				explicit init_tuple(Tup& t, const arg_name* v)
				{
					std::get<I>(t).n_ = v[I].n_;
					init_tuple<Tup, I+1>{t, v};
				}
			};

		template<typename Tup, std::size_t I, std::size_t N>
			struct init_tuple<Tup, I, N, true>
			{
				explicit init_tuple(Tup&, const arg_name*) {}
			};

		typedef R result;

		std::string n_;
		std::tuple<var<Args>...> t_;

		template<typename ...Names>
		explicit func(const arg_dummy<Names>&... ns)
			:n_{
				[] {
					std::ostringstream ss;
					ss << get_func_prefix() << get_symcode();
					return ss.str();
				}()
			}
			,print_cnt_{0}
		{
			arg_name v[]= {ns...};
			static_assert(sizeof(v)/sizeof(v[0]) == std::tuple_size<decltype(t_)>::value, "number of arguments and names don't match");
			init_tuple<decltype(t_)>(t_, v);
		}

		template<typename ...Names>
		explicit func(const std::string& n, const arg_dummy<Names>&... ns)
			:n_{n}
			,print_cnt_{0}
		{
			arg_name v[]= {ns...};
			static_assert(sizeof(v)/sizeof(v[0]) == std::tuple_size<decltype(t_)>::value, "number of arguments and names don't match");
			init_tuple<decltype(t_)>(t_, v);
		}

		explicit func()
			:n_{
				[] {
					std::ostringstream ss;
					ss << get_func_prefix() << get_symcode();
					return ss.str();
				}()
			}
			,print_cnt_{0}
		{
			default_args();
		}

		explicit func(const std::string& n)
			:n_(n)
			,print_cnt_{0}
		{
			default_args();
		}


		s__<func> forward() const
		{
			return s__<func>(n_);
		}

		s__<result> operator()(const s__<Args>&... args) const
		{
			assert(!n_.empty());
			return s__<result>(n_ + list_args(args...)); 
		}

		s__<std::string> call_string(const s__<Args>&... args) const
		{
			assert(!n_.empty());
			val<std::string> fn{n_ + "("};
			std::ostringstream ss;
			ss << fn;
			args_to_val(ss, args...);
			ss << "+" << val<std::string>(")");
			return s__<std::string>(ss.str()); 
		}


		template<size_t I>
		typename std::tuple_element<I,decltype(t_)>::type const& arg() const
		{
			return std::get<I>(t_);
		}
		template<size_t I>
		typename std::tuple_element<I,decltype(t_)>::type& arg()
		{
			return std::get<I>(t_);
		}
	
		template<size_t I>
		typename std::tuple_element<I,decltype(t_)>::type const& arg(const std::string& n) const
		{
			typename std::tuple_element<I,decltype(t_)>::type& r = arg<I>();
			assert(r.n_ == n);
			return r;

		}
		template<size_t I>
		typename std::tuple_element<I,decltype(t_)>::type& arg(const std::string& n)
		{
			typename std::tuple_element<I,decltype(t_)>::type& r = arg<I>();
			assert(r.n_ == n);
			return r;

		}

		template<typename S, typename Tup, std::size_t I, std::size_t N, bool stop> struct print_tuple;
		template<typename S, typename Tup, std::size_t I=0, std::size_t N = std::tuple_size<Tup>::value, bool stop = (I == N)>
			struct print_tuple
			{
				explicit print_tuple(S& s, const Tup& t)
				{
					if (I != 0) {s << ",";}
					s << std::get<I>(t).n_;
					print_tuple<S, Tup, I+1>{s, t};
				}
			};
		template<typename S, typename Tup, std::size_t I, std::size_t N>
			struct print_tuple<S, Tup, I, N, true>
			{
				explicit print_tuple(S&, const Tup&) {}
			};

		template< typename S >
		void print(S& s) const
		{
			if (print_cnt_) {
				std::cerr << "func: " << n_ << std::endl;
				assert(!print_cnt_);
			}
			++print_cnt_;
			s << "function " << n_ << "(";
			print_tuple<S, decltype(t_)>(s, t_);
 			s << ")";
		}

	private:
		mutable int print_cnt_;

		void default_args()
		{
			std::size_t n = std::tuple_size<decltype(t_)>::value;
			if (!n)
				return;
			arg_name v[std::tuple_size<decltype(t_)>::value];
			for (std::size_t i = 0; i != n; ++i) {
				std::ostringstream ss;
				ss << "arg__" << get_symcode();
				v[i] = arg_name(ss.str());
			}
			init_tuple<decltype(t_)>(t_, v);
		}

		static std::string list_args(const s__<Args>&... args)
 		{
			std::string v[] = { format(args)... };

 			std::ostringstream ss;
 			ss << "(";
			constexpr size_t nargs = sizeof...(args);
			for (int i = 0; i != nargs; ++i) {
 				if (i) {
 					ss << ",";
 				}
				ss << v[i];
 			}
 			ss << ")";
 			return ss.str(); 
 		}
		template<typename S>
		static void args_to_val(S& ss, const s__<Args>&... args)
 		{
			std::string v[] = { format(args)... };
			constexpr size_t nargs = sizeof...(args);
			for (int i = 0; i != nargs; ++i) {
 				if (i) {
 					ss << "+" << val<std::string>(",");
 				}
				ss << "+" << val<std::string>("'");
				ss << "+" << v[i];
				ss << "+" << val<std::string>("'");
 			}
 		}
 
		func(const func&) = delete;
		func& operator=(const func&) = delete;
	};

	struct cond__
	{
		const std::string s_;

		cond__(const s__<bool>& s)
			:s_{
				[&s] { std::ostringstream ss; ss << s; return ss.str(); }()
			}
		{}
		cond__(const s__<tag_t>& s)
			:s_{
				[&s] { std::ostringstream ss; ss << s; return ss.str(); }()
			}
		{}
		cond__(const s__<int>& s)
			:s_{
				[&s] { std::ostringstream ss; ss << "(" << s << "!=0)"; return ss.str(); }()
			}
		{}
		cond__(const s__<std::string>& s)
			:s_{
				[&s] { std::ostringstream ss; ss << "(" << s.length() << "!=0)"; return ss.str(); }()
			}
		{}
	};

	struct if_ : cond__
	{
		template<typename T>
			if_(const T& s)
			:cond__(s)
		{}
		template< typename S >
		void print(S& s) const
		{
			s << "if (" << s_ << ") {" << std::endl;
		}
	};
	struct elseif_ : cond__
	{
		template<typename T>
			elseif_(const T& s)
			:cond__(s)
		{}
	
		template< typename S >
		void print(S& s) const
		{
			s << "} else if (" << s_ << ") {" << std::endl;
		}
	};

	template<typename T>
	struct ret_
	{
		std::string s_;
		ret_(const s__<T>& s1)
			:s_{
				[&s1] { std::ostringstream ss; ss << s1; return ss.str(); }()
			}
		{ }
		template< typename S >
		void print(S& s) const
		{
			s << "return " << s_ << ";" << std::endl;
		}
	};

	template<>
	struct ret_<void>
	{ 
		template< typename S >
		void print(S& s) const
		{
			s << "return;" << std::endl;
		}
	};

	struct void__;
	inline void void_(void__*) {}

	struct for_ : cond__
	{
		std::string s1_;
		std::string s2_;

		template<typename C, typename T>
			for_(const s__<void>& s1, const C& c, const s__<T>& s2)
			:cond__(c)
			,s1_{
				[&s1] { std::ostringstream ss; ss << s1; return ss.str(); }()
			}
			,s2_{
				[&s2] { std::ostringstream ss; ss << s2; return ss.str(); }()
			}
		{}
		template<typename C, typename T>
			for_(void (*)(void__*), const C& c, const s__<T>& s2)
			:cond__(c)
			,s2_{
				[&s2] { std::ostringstream ss; ss << s2; return ss.str(); }()
			}
		{}
		template<typename C>
			for_(const s__<void>& s1, const C& c, void(*)(void__*))
			:cond__(c)
			,s1_{
				[&s1] { std::ostringstream ss; ss << s1; return ss.str(); }()
			}
		{}
		template<typename C>
			for_(void (*)(void__*), const C& c, void (*)(void__*))
			:cond__(c)
		{}
	
		template< typename S >
		void print(S& s) const
		{
			s << "for(" << s1_ << ";" << cond__::s_ << ";" << s2_ << ") {" << std::endl;
		}
	};

	inline decltype(std::endl(std::cout)) endfor_(decltype(std::endl(std::cout)) ss)
	{
		ss << "}" << std::endl;
		return ss;
	}

	inline decltype(std::endl(std::cout)) begin_(decltype(std::endl(std::cout)) ss)
	{
		ss << " {" << std::endl;
		return ss;
	}

	inline decltype(std::endl(std::cout)) end_(decltype(std::endl(std::cout)) ss)
	{
		ss << "}" << std::endl;
		return ss;
	}

	inline decltype(std::endl(std::cout)) endif_(decltype(std::endl(std::cout)) ss)
	{
		ss << "}" << std::endl;
		return ss;
	}

	inline decltype(std::endl(std::cout)) else_(decltype(std::endl(std::cout)) ss)
	{
		ss << "} else {" << std::endl;
		return ss;
	}

	inline decltype(std::endl(std::cout)) break_(decltype(std::endl(std::cout)) ss)
	{
		ss << "break;" << std::endl;
		return ss;
	}
	inline decltype(std::endl(std::cout)) continue_(decltype(std::endl(std::cout)) ss)
	{
		ss << "continue;" << std::endl;
		return ss;
	}

	inline decltype(std::endl(std::cout)) return_(decltype(std::endl(std::cout)) ss)
	{
		ss << "return;" << std::endl;
		return ss;
	}

	inline val<float> operator "" _v(long double v)
	{
		return val<float>(static_cast<float>(v));
	}
	inline val<int> operator "" _v(unsigned long long int v)
	{
		return val<int>(static_cast<int>(v));
	}
	inline val<std::string> operator "" _v(const char* p, size_t n)
	{
		return val<std::string>(std::string(p, p+n));
	}

	inline arg_dummy<> operator "" _arg(const char* p, size_t n)
	{
		return arg_dummy<>(std::string(p, p+n));
	}
}
#endif
