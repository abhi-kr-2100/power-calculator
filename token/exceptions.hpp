#ifndef A2100_PCALC_PARSER_EXCEPTIONS
#define A2100_PCALC_PARSER_EXCEPTIONS   1

/**
 * This module contains expressions thrown by the token library.
*/

#include <stdexcept>

class Unknown_token : std::exception
{
public:
    Unknown_token(char)
    {
    }
};

#endif
