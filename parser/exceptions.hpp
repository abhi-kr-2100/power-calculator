#ifndef A2100_PCALC_PARSER_EXCEPTIONS
#define A2100_PCALC_PARSER_EXCEPTIONS 1
#pragma once

#include <stdexcept>
#include <string>

class Runtime_error : public std::exception
{
public:
    Runtime_error(std::string s = "")
        : what_err{s}
    {
    }

    const char *what() const noexcept
    {
        return what_err.c_str();
    }

private:
    std::string what_err;
};

class Syntax_error : public std::exception
{
public:
    Syntax_error(std::string s = "")
        : what_err{s}
    {
    }

    const char *what() const noexcept
    {
        return what_err.c_str();
    }

private:
    std::string what_err;
};

#endif
