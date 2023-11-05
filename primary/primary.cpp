#include <string>
#include <algorithm>
#include <cmath>

#include <boost/uuid/uuid_generators.hpp>

#include "primary.hpp"
#include "primary_helpers.hpp"
#include "exceptions.hpp"

using namespace std::string_literals;

using boost::uuids::random_generator;
using std::find_if;
using std::fmod;
using std::less;
using std::string;
using std::tgamma;

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

Primary::Primary(double v, const Unit_system &system)
    : value{v}, unit_system{system}
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
    : value{v}, unit_system{system},
      numerator_units{to_units_map(nunits, unit_system)},
      denominator_units{to_units_map(dunits, unit_system)}
{
}

double Primary::get_value() const
{
    return value;
}

Primary Primary::operator+(const Primary &other) const
{
    if (unit_system != other.unit_system)
    {
        throw Incompatible_units{
            "Primaries of different unit systems can't be added."};
    }

    if (!addition_compatible(numerator_units, other.numerator_units) ||
        !addition_compatible(denominator_units, other.denominator_units))
    {
        throw Incompatible_units{
            "Primaries measuring different quantities can't be added."};
    }

    const auto nval = compound_convert(value, unit_system,
                                       numerator_units, other.numerator_units);
    const auto dval = compound_convert(1.0, unit_system,
                                       denominator_units, other.denominator_units);
    const auto val = nval / dval + other.value;

    return Primary(val, unit_system,
                   to_units_list(other.numerator_units),
                   to_units_list(other.denominator_units));
}

Primary Primary::operator-(const Primary &other) const
{
    if (unit_system != other.unit_system)
    {
        throw Incompatible_units{
            "Primaries of different unit systems can't be subtracted."};
    }

    if (!addition_compatible(numerator_units, other.numerator_units) ||
        !addition_compatible(denominator_units, other.denominator_units))
    {
        throw Incompatible_units{
            "Primaries measuring different quantities can't be subtracted."};
    }

    const auto nval = compound_convert(value, unit_system,
                                       numerator_units, other.numerator_units);
    const auto dval = compound_convert(1.0, unit_system,
                                       denominator_units, other.denominator_units);
    const auto val = nval / dval - other.value;

    return Primary(val, unit_system,
                   to_units_list(other.numerator_units),
                   to_units_list(other.denominator_units));
}

Primary Primary::operator*(const Primary &other) const
{
    if (unit_system != other.unit_system)
    {
        throw Incompatible_units{
            "Primaries of different unit systems can't be multiplied."};
    }

    const auto units_of_other = get_combined_units(other.numerator_units,
                                                   other.denominator_units);

    const auto n_to_units = fill_units(units_of_other, numerator_units);
    const auto nval = compound_convert(get_value(), unit_system,
                                       numerator_units, n_to_units);

    const auto d_to_units = fill_units(units_of_other, denominator_units);
    const auto dval = compound_convert(1.0, unit_system,
                                       denominator_units, d_to_units);

    const auto new_value = nval / dval * other.get_value();

    const auto new_numerator_units = units_union(numerator_units,
                                                 other.numerator_units);
    const auto new_denominator_units = units_union(denominator_units,
                                                   other.denominator_units);

    const auto simplified_numerator = units_difference(new_numerator_units,
                                                       new_denominator_units);
    const auto simplified_denominator = units_difference(new_denominator_units,
                                                         new_numerator_units);

    return Primary(new_value, unit_system,
                   to_units_list(simplified_numerator),
                   to_units_list(simplified_denominator));
}

Primary Primary::operator/(const Primary &other) const
{
    if (unit_system != other.unit_system)
    {
        throw Incompatible_units{
            "Primaries of different unit systems can't be multiplied."};
    }

    if ((int)other.get_value() == 0)
    {
        throw Division_by_zero{"Division by 0 is not allowed."};
    }

    return (*this) * Primary(1.0 / other.get_value(), unit_system,
                             to_units_list(other.denominator_units),
                             to_units_list(other.numerator_units));
}

Primary Primary::operator%(const Primary &other) const
{
    if (unit_system != other.unit_system)
    {
        throw Incompatible_units{
            "Primaries of different unit systems can't be operated on by mod."};
    }

    if ((int)other.get_value() == 0)
    {
        throw Division_by_zero{"Can't take mod with 0."};
    }

    if (!addition_compatible(numerator_units, other.numerator_units) ||
        !addition_compatible(denominator_units, other.denominator_units))
    {
        throw Incompatible_units{
            "Primaries measuring different quantities can't be operated on by mod."};
    }

    const auto nval = compound_convert(value, unit_system,
                                       numerator_units, other.numerator_units);
    const auto dval = compound_convert(1.0, unit_system,
                                       denominator_units, other.denominator_units);
    const auto val = fmod(nval / dval, other.get_value());

    return Primary(val, unit_system,
                   to_units_list(other.numerator_units),
                   to_units_list(other.denominator_units));
}

Primary Primary::operator^(const Primary &other) const
{
    if (unit_system != other.unit_system)
    {
        throw Incompatible_units{
            "Primaries of different unit systems can't be operated on by exponentiation."};
    }

    return power(get_value(), other.get_value());
}

Primary Primary::factorial() const
{
    return Primary(tgamma(get_value() + 1), unit_system);
}

Primary Primary::operator+() const
{
    return (*this);
}

Primary Primary::operator-() const
{
    return Primary(-get_value(), unit_system,
                   to_units_list(numerator_units),
                   to_units_list(denominator_units));
}
