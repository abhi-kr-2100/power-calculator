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
    Divide_by_zero(std::string)
    {
    }
};

class Unmatched_parentheses : std::exception
{
};

class Syntax_error : std::exception
{
public:
    Syntax_error(std::string)
    {
    }
};

#endif
