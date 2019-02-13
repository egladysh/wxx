#ifndef h_D3A689BDFF154D7C9E344BE5EC721859 
#define h_D3A689BDFF154D7C9E344BE5EC721859

#include <iostream>
#include <wxx/s.h>
#include <wxx/func.h>

namespace wxx
{
	template<typename T> 
	inline std::string phpval__(const std::string& param_name, const std::string&)
	{
		return std::string("'.") + param_name + ".'";
	}

	template<>
	inline std::string phpval__<std::string>(const std::string& param_name, const std::string& unesc)
	{
		return unesc + "(\"'.addcslashes(" + param_name + ", \"\\\\\\\"\\n\\r\").'\")";
	}

	template<typename S // pass in where the php binding script will go
		,typename T>
	S& php_bind_print(S& s, const var<T>& v, const std::string& php_func, const std::string& unescape_func = "html_unescape")
	{
		assert(php_func.find("function") == 0); //must be a closure
		s 
			<< "$wxxfunc__=" << php_func << ";" << std::endl
			<< "$wxxvar__=$wxxfunc__();" << std::endl
			<< "echo '<script> var " << v << "=" << phpval__<T>("$wxxvar__", unescape_func) << "; </script>';"
			;
		return s;
	}

	template<typename S>
	S& php_init_locale(S& ss, const std::vector<std::pair<std::string, std::string>>& lc) //name -> link
	{
		assert(!lc.empty());

		ss << "$locale__=''; if (function_exists('get_current_locale')) { $locale__=get_current_locale(); } $lfound__=false;" << std::endl;
		bool first=true;
		for (const auto& v: lc) {
			if (!first) {
				ss << "else ";
			}
			else {
				first = false;
			}
			ss << "if ($locale__=='" << v.first << "') {" << std::endl
				<< "echo \"<script charset='UTF-8' src='" << v.second << "'></script>\";" << std::endl
				<< "$lfound__ = true;"
				<< std::endl << "}" << std::endl;
		}
		ss << "if (!$lfound__) {" << std::endl
			<< "echo \"<script charset='UTF-8' src='" << lc.begin()->second << "'></script>\";" << std::endl
			<< "}" << std::endl;
		return ss;
	}

	inline decltype(std::endl(std::cout)) 
	php(decltype(std::endl(std::cout)) s)
	{ 
		s << "<?php" << std::endl;
		return s;
	}

	inline decltype(std::endl(std::cout)) 
	phpend(decltype(std::endl(std::cout)) s)
	{ 
		s << "?>" << std::endl;
		return s;
	}
}

#endif 
