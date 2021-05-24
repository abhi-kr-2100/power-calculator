#ifndef A2100_PCALC_TOKEN_EXCEPTIONS
#define A2100_PCALC_TOKEN_EXCEPTIONS   1

/**
 * This module contains expressions thrown by the token library.
*/

#include <stdexcept>
#include <string>

class Unknown_token : std::exception
{
public:
    Unknown_token(const std::string s = "")
        : what_err{s}
    {
    }

    const char* what() const noexcept
    {
        return what_err.c_str();
    }
private:
    std::string what_err;
};

#endif
