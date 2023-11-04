#include <set>
#include <string>
#include <map>

#include "primary.hpp"
#include "primary_helpers.hpp"

using std::map;
using std::multiset;
using std::string;

bool addition_compatible(const Unit_system &sys,
                         std::multiset<std::string> &u1,
                         const std::multiset<std::string> &u2)
{
    if (u1.size() != u2.size())
    {
        return false;
    }

    map<Unit_type, size_t> unit_type_counts_1;
    for (const auto &u : u1)
    {
        ++unit_type_counts_1[sys.get_base(u)];
    }

    map<Unit_type, size_t> unit_type_counts_2;
    for (const auto &u : u2)
    {
        ++unit_type_counts_2[sys.get_base(u)];
    }

    for (const auto &[base, count] : unit_type_counts_1)
    {
        if (unit_type_counts_2[base] != count)
        {
            return false;
        }
    }

    for (const auto &[base, count] : unit_type_counts_2)
    {
        if (unit_type_counts_1[base] != count)
        {
            return false;
        }
    }

    return true;
}
