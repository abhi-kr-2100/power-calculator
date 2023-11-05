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

class Incompatible_units : public std::exception
{
public:
    Incompatible_units(const std::string &s = "")
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

class Unknown_unit : public std::exception
{
public:
    Unknown_unit(const std::string &s = "")
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

/**
 * Presently, we only support one unit per base in a single primary.
 * For example, meter / (second * second) is valid but meter / (second * hour)
 * is not. This exception is thrown if user tries to create a primary with
 * differing units for the same base.
 */
class Different_units_for_same_base : public std::exception
{
public:
    Different_units_for_same_base(const std::string &s = "")
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

class Division_by_zero : public std::exception
{
public:
    Division_by_zero(const std::string &s = "")
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

class Invalid_operands : public std::exception
{
public:
    Invalid_operands(const std::string &s = "")
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
