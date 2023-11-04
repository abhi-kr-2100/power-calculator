#ifndef A2100_PCALC_PRIMARY_EXCEPTIONS
#define A2100_PCALC_PRIMARY_EXCEPTIONS 1

#include <stdexcept>
#include <string>

class Unit_already_exists : public std::exception
{
public:
    Unit_already_exists(const std::string &s = "")
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
