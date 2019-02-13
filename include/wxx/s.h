#ifndef h_C41E7E873BD44F34B1904B26DAF9BCD3F
#define h_C41E7E873BD44F34B1904B26DAF9BCD3F

#include <string>
#include <assert.h>
#include <sstream>
#include <type_traits>

namespace wxx
{
	template <typename T>
	struct func;

	struct tag_t;
	struct json;

	template<typename T> struct array_t;
	template<typename K, typename T> struct map_t;

		template<typename LT>
			struct do_length;

		template<>
			struct do_length<std::string>
			{
				const std::string& n_;
				do_length(const std::string& n)
					:n_{n}
				{}
				std::string operator()() const
				{
					return n_ + ".length";
				}
			};
		template<typename LT>
			struct do_length<array_t<LT>>
			{
				const std::string& n_;
				do_length(const std::string& n)
					:n_{n}
				{}
				std::string operator()() const
				{
					return n_ + ".length";
				}
			};
		template<typename K, typename T>
			struct do_length<map_t<K, T>>
			{
				const std::string& n_;
				do_length(const std::string& n)
					:n_{n}
				{}
				std::string operator()() const
				{
					return n_ + ".length";
				}
			};

		template<typename T> struct s__;

	struct sb__
	{
		std::string n_;

		explicit sb__(std::string n)
			:n_(std::move(n))
		{
		}

		template< typename S >
		void print(S& s) const
		{
			if (!n_.empty())
				s << n_;
		}

		template<typename F> 
		func<F> mf_(const std::string& n) const
		{
			std::ostringstream ss;
			print(ss);
			ss << "." << n;
			return func<F>{ss.str()};
		}

		sb__& operator=(const sb__&) = delete;

		inline s__<bool> operator==(const sb__& v) const;
		inline s__<bool> operator!=(const sb__& v) const;
		inline s__<bool> operator>=(const sb__& v) const;
		inline s__<bool> operator>(const sb__& v) const;
		inline s__<bool> operator<(const sb__& v) const;
		inline s__<bool> operator<=(const sb__& v) const;
		inline s__<bool> operator||(const sb__& v) const;
		inline s__<bool> operator&&(const sb__& v) const;
		inline s__<bool> operator!() const;
	};

	template< typename T >
	struct s__ : sb__
	{
		typedef T value_type;

		using sb__::n_;

		explicit s__(const std::string& n)
			:sb__{n}
		{
		}
		
		s__<int> length() const
		{
			return s__<int>(do_length<T>{n_}());
		}
	};

	template< typename T >
	struct s__<array_t<T>> : sb__
	{
		typedef array_t<T> value_type;

		using sb__::n_;

		explicit s__(const std::string& n)
			:sb__{n}
		{
		}
		
		s__<int> length() const
		{
			return s__<int>(do_length<value_type>{n_}());
		}

		s__<T> operator[](const s__<int>& i);

		s__<void> push(const s__<T>& v)
		{
			std::ostringstream ss;
			ss << n_ << ".push(" << v << ")";
			return s__<void>(ss.str());
		}
	};

	template<typename T>
	s__<array_t<T>> new_array()
	{
		std::ostringstream ss;
		ss << "new Array()";
		return s__<array_t<T>>(ss.str());
	}
	
	template<typename K, typename T>
	struct s__<map_t<K, T>> : sb__
	{
		typedef map_t<K, T> value_type;

		using sb__::n_;

		explicit s__(const std::string& n)
			:sb__{n}
		{
		}
		
		s__<int> length() const
		{
			return s__<int>(do_length<value_type>{n_}());
		}

		s__<T> operator[](const s__<K>& i);

		s__<void> insert(const s__<K>& k, const s__<T>& v)
		{
			std::ostringstream ss;
			ss << n_ << "[" << k << "]=" << v;
			return s__<void>(ss.str());
		}

		s__<bool> exists(const s__<K>& k)
		{
			std::ostringstream ss;
			ss << "(" << k << " in " << n_ << ")";
			return s__<bool>(ss.str());
		}
	};


	template<>
	struct s__<std::string> : sb__
	{
		typedef std::string value_type;

		using sb__::n_;

		explicit s__(const std::string& n)
			:sb__{n}
		{
		}

		bool empty() const
		{
			return n_.empty();
		}
		
		s__<int> length() const
		{
			return s__<int>(do_length<value_type>{n_}());
		}

		inline s__<int> index_of(const s__& v);
		inline s__<char> operator[](const s__<int>& i);

		inline s__<std::string> substring(const s__<int>& b, const s__<int>& e);
		inline s__<std::string> substring(const s__<int>& b);
		inline s__<std::string> replace(const s__<std::string>& s, const s__<std::string>& r);
	};

	/*
	template<>
	struct s__<void> : sb__
	{
		typedef void value_type;

		using sb__::n_;

		s__(const std::string& n)
			:sb__{n}
		{
		}
	};
	*/

	template<typename S, typename T>
		auto operator<<(S& os, const T& t) -> decltype(t.print(os), os) 
		{ 
			t.print(os); 
			return os; 
		} 

	inline s__<int> s__<std::string>::index_of(const s__& v)
	{
		std::ostringstream ss;
		ss << n_ << ".indexOf(" << v << ")";
		return s__<int>(ss.str());
	}
	inline s__<char> s__<std::string>::operator[](const s__<int>& i)
	{
		std::ostringstream ss;
		ss << n_ << "[" << i << "]";
		return s__<char>(ss.str());
	}
	inline s__<std::string> s__<std::string>::substring(const s__<int>& b, const s__<int>& e)
	{
		std::ostringstream ss;
		ss << n_ << ".substring(" << b << "," << e << ")";
		return s__<std::string>(ss.str());
	}
	inline s__<std::string> s__<std::string>::substring(const s__<int>& b)
	{
		std::ostringstream ss;
		ss << n_ << ".substring(" << b << ")";
		return s__<std::string>(ss.str());
	}
	inline s__<std::string> s__<std::string>::replace(const s__<std::string>& s, const s__<std::string>& r)
	{
		std::ostringstream ss;
		ss << n_ << ".replace(" << s << "," << r << ")";
		return s__<std::string>(ss.str());
	}

	template<typename T>
	s__<T> s__<array_t<T>>::operator[](const s__<int>& i)
	{
		std::ostringstream ss;
		ss << n_ << "[" << i << "]";
		return s__<T>(ss.str());
	}
	template<typename K, typename T>
	s__<T> s__<map_t<K, T>>::operator[](const s__<K>& i)
	{
		std::ostringstream ss;
		ss << n_ << "[" << i << "]";
		return s__<T>(ss.str());
	}

	std::string xml_escape(const std::string& in);

	inline unsigned int get_symcode() 
	{
		static int symcode__;
		return symcode__++;
	}

	inline const char* get_func_prefix()
	{
		static const char f[] = "f__";
		return f;
	}


	struct x_
	{
		std::string xs_;

		template<typename T>
		explicit x_(const T& s)
			:xs_{
				[&s] { std::ostringstream ss; ss << s; return ss.str(); }()
			}
		{}
		template< typename S >
		void print(S& s) const
		{
			if (!xs_.empty())
				s << xs_ << ";" << std::endl;
		}
	};

	template<typename S>
		auto operator<<(S& os, const x_& t) -> decltype(t.print(os), os) 
		{ 
			t.print(os); 
			return os; 
		} 

		inline s__<bool> sb__::operator==(const sb__& v) const
		{
			std::ostringstream ss;
			ss << "(" << *this << "==" << v << ")";
			return s__<bool>(ss.str());
		}
		inline s__<bool> sb__::operator!=(const sb__& v) const
		{
			std::ostringstream ss;
			ss << "(" << *this << "!=" << v << ")";
			return s__<bool>(ss.str());
		}
		inline s__<bool> sb__::operator>=(const sb__& v) const
		{
			std::ostringstream ss;
			ss << "(" << *this << ">=" << v << ")";
			return s__<bool>(ss.str());
		}
		inline s__<bool> sb__::operator>(const sb__& v) const
		{
			std::ostringstream ss;
			ss << "(" << *this << ">" << v << ")";
			return s__<bool>(ss.str());
		}
		inline s__<bool> sb__::operator<(const sb__& v) const
		{
			std::ostringstream ss;
			ss << "(" << *this << "<" << v << ")";
			return s__<bool>(ss.str());
		}
		inline s__<bool> sb__::operator<=(const sb__& v) const
		{
			std::ostringstream ss;
			ss << "(" << *this << "<=" << v << ")";
			return s__<bool>(ss.str());
		}
		inline s__<bool> sb__::operator||(const sb__& v) const
		{
			std::ostringstream ss;
			ss << "(" << *this << "||" << v << ")";
			return s__<bool>(ss.str());
		}
		inline s__<bool> sb__::operator&&(const sb__& v) const
		{
			std::ostringstream ss;
			ss << "(" << *this << "&&" << v << ")";
			return s__<bool>(ss.str());
		}
		inline s__<bool> sb__::operator!() const
		{
			std::ostringstream ss;
			ss << "(!" << *this << ")";
			return s__<bool>(ss.str());
		}

	template<typename T>
	s__<T> operator+(const s__<T>& e1, const s__<T>& e2 )
	{
		static_assert(std::is_arithmetic<T>::value, "must be arithmetic");
		std::ostringstream ss;
		ss << "(" << e1 << "+" << e2 << ")";
		return s__<T>{ss.str()};
	}
	inline s__<std::string> operator+(const s__<std::string>& e1, const s__<std::string>& e2)
	{
		std::ostringstream ss;
		ss << "(" << e1 << "+" << e2 << ")";
		return s__<std::string>{ss.str()};
	}

	template<typename T>
	s__<T> operator-(const s__<T>& e1, const s__<T>& e2 )
	{
		static_assert(std::is_arithmetic<T>::value, "must be arithmetic");
		std::ostringstream ss;
		ss << "(" << e1 << "-" << e2 << ")";
		return s__<T>{ss.str()};
	}
	template<typename T>
	s__<T> operator*(const s__<T>& e1, const s__<T>& e2 )
	{
		static_assert(std::is_arithmetic<T>::value, "must be arithmetic");
		std::ostringstream ss;
		ss << "(" << e1 << "*" << e2 << ")";
		return s__<T>{ss.str()};
	}
	template<typename T>
	s__<T> operator/(const s__<T>& e1, const s__<T>& e2 )
	{
		static_assert(std::is_arithmetic<T>::value, "must be arithmetic");
		std::ostringstream ss;
		ss << "(" << e1 << "/" << e2 << ")";
		return s__<T>{ss.str()};
	}

	template<typename T>
		using factor = s__<T>;
}


#endif
