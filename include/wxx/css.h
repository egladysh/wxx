#ifndef h_04277A061B2141D9A9AF1A7FE109C5E6
#define h_04277A061B2141D9A9AF1A7FE109C5E6

#include <sstream>

namespace wxx
{
	inline std::string style_alpha(float a)
	{
		std::ostringstream ss;
		ss << "opacity: " << a << ";" /* Standard: FF gt 1.5, Opera, Safari, CSS3 */
			<< "filter: alpha(opacity=" << (int)(a*100.0) << ");" /* IE lt 8 */
			<< "-ms-filter: \"alpha(opacity=" << (int)(a*100.0) << ")\";" /* IE 8 */
			<< "-khtml-opacity: " << a << ";" /* Safari 1.x */
			<< "-moz-opacity: " << a << ";" /* FF lt 1.5, Netscape */
			;
		return ss.str();
	}
}

#endif
