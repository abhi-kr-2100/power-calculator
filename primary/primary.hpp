#ifndef A2100_PCALC_PRIMARY
#define A2100_PCALC_PRIMARY 1

/**
 * This library provides the Primary UDT. It represents a numerical value
 * with an optional unit attached to it. Mathematical operations can be
 * performed on Primaries with compatible units. Compatibility between units
 * is determined by a UnitSystem provided to each Primary.
 */

#include <string>
#include <vector>
#include <set>

#include <boost/uuid/uuid.hpp>

enum class Unit_type
{
    length,
    mass
};

/**
 * Each unit is representable as a linear equation in one of the fundamental
 * Unit_types.
 *
 * For example, b myLenUnit may be equal to a + bx Unit_type::lengths.
 */
struct Unit_information
{
    std::string name;
    Unit_type base;

    double a;
    double x;

    bool operator<(const Unit_information &other) const
    {
        return this->name < other.name;
    }
};

class Unit_system
{
public:
    Unit_system();

    void add_new_unit(const Unit_information &new_unit);

    double convert(
        double v,
        const std::string &from_unit, const std::string &to_unit) const;

private:
    std::set<Unit_information> units;
    boost::uuids::uuid tag;
};

/**
 * A numeric value optionally with a unit.
 *
 * When the unit is compound (such as meters / second), it is dealt as two sets
 * of units: one that is the product on the numerator, the other the product on
 * the denominator.
 */
class Primary
{
public:
    Primary();
    Primary(double v);
    Primary(double v, const Unit_system &system, const std::string &unit);
    Primary(
        double v,
        const Unit_system &system,
        const std::multiset<std::string> &nunits,
        const std::multiset<std::string> &dunits);

    Primary operator+(const Primary &other) const;
    Primary operator-(const Primary &other) const;
    Primary operator*(const Primary &other) const;
    Primary operator/(const Primary &other) const;
    Primary operator%(const Primary &other) const;
    Primary operator^(const Primary &other) const;
    Primary factorial() const;

private:
    double value;
    Unit_system unit_system;
    std::multiset<std::string> numerator_units;
    std::multiset<std::string> denominator_units;
};

#endif
