#include <wxx/s.h>
#include <wxx/wxx.h>

namespace wxx
{
	int dummy{0};

	std::string xml_escape(const std::string& in)
	{
		std::string out;
		for (auto c : in) {
			switch (c) {
			case '>':
				out += "&gt;";
				break;

			case '<':
				out += "&lt;";
				break;

			case '&':
				out += "&amp;";
				break;

			default:
				out.push_back(c);
				break;
			}
		}
		return out;
	}
}

using namespace wxx;
