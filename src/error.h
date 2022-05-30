
#include <string>
#include <iostream>

namespace GDPP
{
    class Error
    {
        public:
            static bool hadError;
            static void Push(int line, std::string msg);
            static void Report(int line, std::string where, std::string msg);
    };
}