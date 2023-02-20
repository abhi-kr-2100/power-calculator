#ifndef A2100_PCALC_TOKEN_EXCEPTIONS
#define A2100_PCALC_TOKEN_EXCEPTIONS   1

/**
 * This module contains exceptions thrown by the token library.
*/

#include <stdexcept>
#include <string>

class Unknown_token : public std::exception
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

class Bad_number : public std::exception
{
public:
    Bad_number(const std::string s = "")
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
