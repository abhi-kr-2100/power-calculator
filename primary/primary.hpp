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
#include <map>
#include <utility>
#include <ostream>

#include <boost/uuid/uuid.hpp>

enum class Unit_type
{
    length,
    mass,
    time,
    electricCurrent,
    temperature,
    amountOfSubstance,
    lightIntensity
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
    Unit_system(const Unit_system &other) = delete;

    void add_new_unit(const Unit_information &new_unit);

    double convert(
        double v,
        const std::string &from_unit, const std::string &to_unit) const;

    Unit_type get_base(const std::string &u) const;

    bool operator==(const Unit_system &other) const;
    bool operator!=(const Unit_system &other) const;

private:
    std::set<Unit_information> units;
    const boost::uuids::uuid tag;
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
    Primary(double v, const Unit_system &system);
    Primary(double v, const Unit_system &system, const std::string &unit);
    Primary(
        double v,
        const Unit_system &system,
        const std::multiset<std::string> &nunits,
        const std::multiset<std::string> &dunits);

    Primary operator+() const;
    Primary operator-() const;
    Primary operator+(const Primary &other) const;
    Primary operator-(const Primary &other) const;
    Primary operator*(const Primary &other) const;
    Primary operator/(const Primary &other) const;
    Primary operator%(const Primary &other) const;
    Primary operator^(const Primary &other) const;
    Primary factorial() const;

    friend std::ostream &operator<<(std::ostream &out, const Primary &self);

    double get_value() const;

private:
    double value;
    const Unit_system &unit_system;
    std::map<Unit_type, std::pair<std::string, size_t>> numerator_units;
    std::map<Unit_type, std::pair<std::string, size_t>> denominator_units;
};

#endif
