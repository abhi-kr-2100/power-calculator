#include <string>
#include <algorithm>

#include <boost/uuid/uuid_generators.hpp>

#include "primary.hpp"
#include "exceptions.hpp"

using namespace std::string_literals;

using boost::uuids::random_generator;
using std::find_if;
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
    const auto from_iter = find_if(units.begin(), units.end(), [&from](const auto &e)
                                   { return e.name == from; });
    if (from_iter == units.end())
    {
        throw Unknown_unit(from + " is not a known unit.");
    }

    const auto to_iter = find_if(units.begin(), units.end(), [&to](const auto &e)
                                 { return e.name == to; });
    if (to_iter == units.end())
    {
        throw Unknown_unit(to + " is not a known unit.");
    }

    if (from_iter->base != to_iter->base)
    {
        throw Incompatible_units("Can't convert between "s + from + " and " + to);
    }

    const auto from_to_base = from_iter->a + v * from_iter->x;
    const auto base_to_to = (from_to_base - to_iter->a) / to_iter->x;

    return base_to_to;
}

Unit_type Unit_system::get_base(const string &unit) const
{
    const auto unit_iter = find_if(units.begin(), units.end(), [&unit](const auto &u)
                                   { return u.name == unit; });
    if (unit_iter == units.end())
    {
        throw Unknown_unit(unit + " is not a known unit.");
    }

    return unit_iter->base;
}

bool Unit_system::operator==(const Unit_system &other) const
{
    return this->tag == other.tag;
}

bool Unit_system::operator!=(const Unit_system &other) const
{
    return !(*this == other);
}

Primary::Primary()
    : value{}, unit_system{}
{
}

Primary::Primary(double v)
    : value{v}, unit_system{}
{
}

Primary::Primary(double v, const Unit_system &system, const string &unit)
    : value{v}, unit_system{system}
{
    const auto base = unit_system.get_base(unit);
    numerator_units[base] = {unit, 1};
}

Primary::Primary(double v, const Unit_system &system,
                 const std::multiset<std::string> &nunits,
                 const std::multiset<std::string> &dunits)
    : value{v}, unit_system{system}
{
    for (const auto &nunit : nunits)
    {
        const auto base = unit_system.get_base(nunit);
        if (numerator_units.find(base) == numerator_units.end())
        {
            numerator_units[base] = {nunit, 1};
        }
        else if (numerator_units[base].first != nunit)
        {
            throw Different_units_for_same_base{
                nunit + " and " + numerator_units[base].first +
                " measure the same quantities but are different. " +
                "Presently, we require one unit per base."};
        }
        else
        {
            ++numerator_units[base].second;
        }
    }

    for (const auto &dunit : dunits)
    {
        const auto base = unit_system.get_base(dunit);
        if (denominator_units.find(base) == denominator_units.end())
        {
            denominator_units[base] = {dunit, 1};
        }
        else if (denominator_units[base].first != dunit)
        {
            throw Different_units_for_same_base{
                dunit + " and " + denominator_units[base].first +
                " measure the same quantities but are different. " +
                "Presently, we require one unit per base."};
        }
        else
        {
            ++denominator_units[base].second;
        }
    }
}

double Primary::get_value() const
{
    return value;
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
