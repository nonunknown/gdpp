#include "error.h"

using namespace GDPP;

void Error::Push(int line, std::string msg)
{
    Report(line, "", msg);
}

void Error::Report(int line, std::string where, std::string msg)
{
    // Error::hadError = true;
    std::cout << "[line: " << line << "] Error -> " << where << ": " << msg << std::endl;
}