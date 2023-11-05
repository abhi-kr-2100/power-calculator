#ifndef A2100_PCALC_PRIMARY_HELPERS
#define A2100_PCALC_PRIMARY_HELPERS 1

#include <set>
#include <string>
#include <map>
#include <utility>

#include "primary.hpp"

using std::map;
using std::multiset;
using std::string;

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

#endif
