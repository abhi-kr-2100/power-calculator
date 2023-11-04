#include <string>

#include <boost/uuid/uuid_generators.hpp>

#include "primary.hpp"
#include "exceptions.hpp"

using boost::uuids::random_generator;
using std::less;
using std::string;

Unit_system::Unit_system()
    : tag(random_generator()())
{
}

void Unit_system::add_new_unit(const Unit_information &new_unit_info)
{
    if (units.find(new_unit_info) != units.end())
    {
        throw Unit_already_exists(new_unit_info.name + " is already defined.");
    }

    units.insert(new_unit_info);
}

double Unit_system::convert(double v, const string &from, const string &to)
    const
{
    return 0;
}

Primary::Primary()
    : value{}
{
}

Primary::Primary(double v)
    : value{v}
{
}

Primary::Primary(double v, const Unit_system &system, const string &unit)
    : value{v}, unit_system{system}
{
    numerator_units.insert(unit);
}

Primary::Primary(double v, const Unit_system &system,
                 const std::multiset<std::string> &nunits,
                 const std::multiset<std::string> &dunits)
    : value{v}, unit_system{system},
      numerator_units(nunits.begin(), nunits.end()),
      denominator_units(dunits.begin(), dunits.end())
{
}

Primary Primary::operator+(const Primary &other) const
{
    return other;
}

Primary Primary::operator-(const Primary &other) const
{
    return other;
}

Primary Primary::operator*(const Primary &other) const
{
    return other;
}

Primary Primary::operator/(const Primary &other) const
{
    return other;
}

Primary Primary::operator%(const Primary &other) const
{
    return other;
}

Primary Primary::operator^(const Primary &other) const
{
    return other;
}

Primary Primary::factorial() const
{
    return *this;
}
