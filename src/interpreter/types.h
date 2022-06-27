#include <variant>
#include <string>

#ifndef TYPES_H
#define TYPES_H

namespace GDPP
{
	typedef std::variant<std::string, bool, int, float> variant;
}

#endif
