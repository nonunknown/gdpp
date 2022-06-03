#ifndef TYPES_H
#define TYPES_H

#include <variant>
#include <string>

namespace GDPP
{
	typedef std::variant<std::string, bool, int, float> variant;
}

#endif
