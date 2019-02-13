#ifndef h_34347C4AE4E14E2B48DEEB6F
#define h_34347C4AE4E14E2B48DEEB6F

namespace wxx
{
namespace jstd
{
	template<typename T>
		s__<void> alert(const s__<T>& v)
		{
			std::ostringstream ss;
			ss << "alert(" << v << ")";
			return s__<void>(ss.str());
		}

	inline const func<std::string(std::string)>& encodeURI()
	{
		static func<std::string(std::string)> f{"encodeURIComponent"};
		return f;
	}

	inline const func<std::string(std::string)>& log()
	{
		static func<std::string(std::string)> f{"console.log"};
		return f;
	}

	inline s__<void> navigate(const factor<std::string>& uri)
	{
		std::ostringstream ss;
		ss << (var<std::string>("window.location.href") = uri);
		return s__<void>(ss.str());
	}

	inline s__<int> parseInt(const factor<std::string>& s)
	{
		func<int(std::string)> f("parseInt");
		std::ostringstream ss;
		ss << f(s);
		return s__<int>(ss.str());
	}
}
}
#endif
