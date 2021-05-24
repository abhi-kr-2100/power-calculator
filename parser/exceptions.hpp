#ifndef A2100_PCALC_PARSER_EXCEPTIONS
#define A2100_PCALC_PARSER_EXCEPTIONS   1

/**
 * This module contains expressions thrown by the parser library.
*/

#include <stdexcept>
#include <string>

class Divide_by_zero : std::exception
{
public:
    Divide_by_zero(const std::string& s = "")
        : what_err{s}
    {
    }

    const char* what() const noexcept
    {
        return what_err.c_str();
    }
private:
    const std::string& what_err;
};

class Unmatched_parentheses : std::exception
{
public:
    Unmatched_parentheses(const std::string& s = "")
        : what_err{s}
    {
    }

    const char* what() const noexcept
    {
        return what_err.c_str();
    }
private:
    const std::string& what_err;
};

class Syntax_error : std::exception
{
public:
    Syntax_error(const std::string& s = "")
        : what_err{s}
    {
    }

    const char* what() const noexcept
    {
        return what_err.c_str();
    }
private:
    const std::string& what_err;
};

#endif
