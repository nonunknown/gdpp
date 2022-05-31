#include <variant>
#include <string>

namespace GDPP
{
	typedef std::variant<std::string, bool, int, float> variant;
}