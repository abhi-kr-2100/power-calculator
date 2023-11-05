#ifndef A2100_PCALC_PRIMARY_HELPERS
#define A2100_PCALC_PRIMARY_HELPERS 1

#include <set>
#include <string>
#include <map>
#include <utility>

#include "primary.hpp"
#include "exceptions.hpp"

bool addition_compatible(
    const std::map<Unit_type, std::pair<std::string, size_t>> &u1,
    const std::map<Unit_type, std::pair<std::string, size_t>> &u2)
{
    for (const auto &[base, unit] : u1)
    {
        const auto [_, count] = unit;
        if (u2.find(base) == u2.end())
        {
            return false;
        }
        if (u2.at(base).second != count)
        {
            return false;
        }
    }

    for (const auto &[base, unit] : u2)
    {
        const auto [_, count] = unit;
        if (u1.find(base) == u1.end())
        {
            return false;
        }
        if (u1.at(base).second != count)
        {
            return false;
        }
    }

    return true;
}

std::multiset<std::string> to_units_list(
    const std::map<Unit_type, std::pair<std::string, size_t>> &units)
{
    std::multiset<std::string> units_list;
    for (const auto &[base, unit] : units)
    {
        const auto [unit_name, reps] = unit;
        for (size_t i = 0; i < reps; ++i)
        {
            units_list.insert(unit_name);
        }
    }

    return units_list;
}

std::map<Unit_type, std::pair<std::string, size_t>> to_units_map(
    const std::multiset<std::string> &units,
    const Unit_system &usys)
{
    std::map<Unit_type, std::pair<std::string, size_t>> units_map;

    for (const auto &unit : units)
    {
        const auto base = usys.get_base(unit);
        if (units_map.find(base) == units_map.end())
        {
            units_map[base] = {unit, 1};
        }
        else if (units_map[base].first != unit)
        {
            throw Different_units_for_same_base{
                unit + " and " + units_map[base].first +
                " measure the same quantities but are different. " +
                "Presently, we require one unit per base."};
        }
        else
        {
            ++units_map[base].second;
        }
    }

    return units_map;
}

double compound_convert(
    double value,
    const Unit_system &usys,
    const std::map<Unit_type, std::pair<std::string, size_t>> &from_units,
    const std::map<Unit_type, std::pair<std::string, size_t>> &to_units)
{
    for (const auto &[base, unit_desc] : from_units)
    {
        const auto [from_unit, rep] = unit_desc;
        const auto to_unit = to_units.at(base).first;

        for (size_t i = 0; i < rep; ++i)
        {
            value = usys.convert(value, from_unit, to_unit);
        }
    }

    return value;
}

/**
 * Combine two sets of units into one. Ignore repetition information. Assumes
 * that one base has only one unit.
 */
std::map<Unit_type, std::pair<std::string, size_t>> get_combined_units(
    const std::map<Unit_type, std::pair<std::string, size_t>> &u1,
    const std::map<Unit_type, std::pair<std::string, size_t>> &u2)
{
    std::map<Unit_type, std::pair<std::string, size_t>> units;

    for (const auto &[base, unit_desc] : u1)
    {
        units[base] = {unit_desc.first, 0};
    }
    for (const auto &[base, unit_desc] : u2)
    {
        units[base] = {unit_desc.first, 0};
    }

    return units;
}

/**
 * Augment hole_units with filler_units.
 *
 * Filler units that don't exist in hole units are added to hole units.
 * Existing hole unit's repetition number is replaced with the filler's
 * repetition number.
 *
 * Note, existence is determined by base, not unit name.
 */
std::map<Unit_type, std::pair<std::string, size_t>> fill_units(
    const std::map<Unit_type, std::pair<std::string, size_t>> &hole_units,
    const std::map<Unit_type, std::pair<std::string, size_t>> &filler_units)
{
    std::map<Unit_type, std::pair<std::string, size_t>> filled = hole_units;

    for (const auto &[base, unit_desc] : filler_units)
    {
        if (hole_units.find(base) == hole_units.end())
        {
            filled[base] = unit_desc;
            continue;
        }

        filled[base].second = unit_desc.second;
    }

    return filled;
}

/**
 *
 */
std::map<Unit_type, std::pair<std::string, size_t>> units_union(
    const std::map<Unit_type, std::pair<std::string, size_t>> &u1,
    const std::map<Unit_type, std::pair<std::string, size_t>> &u2)
{
    std::map<Unit_type, std::pair<std::string, size_t>> unioned;

    for (const auto &[base, unit] : u1)
    {
        unioned[base] = unit;
    }

    for (const auto &[base, unit] : u2)
    {
        unioned[base] = {unit.first, unioned[base].second + unit.second};
    }

    return unioned;
}

std::map<Unit_type, std::pair<std::string, size_t>> units_difference(
    const std::map<Unit_type, std::pair<std::string, size_t>> &u1,
    const std::map<Unit_type, std::pair<std::string, size_t>> &u2)
{
    std::map<Unit_type, std::pair<std::string, size_t>> diffed;

    for (const auto &[base, unit] : u1)
    {
        if (u2.find(base) != u2.end())
        {
            continue;
        }

        diffed[base] = unit;
    }

    return diffed;
}

#endif
