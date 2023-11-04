#include <gtest/gtest.h>

#include "primary/primary.hpp"
#include "primary/exceptions.hpp"

TEST(Unit_system, AddNewUnit)
{
    auto usys = Unit_system();

    EXPECT_NO_THROW(
        usys.add_new_unit(
            Unit_information{
                "meter",
                Unit_type::length,
                0, 1
            }
        )
    );

    EXPECT_THROW(
        usys.add_new_unit(
            Unit_information{
                "meter",
                Unit_type::length,
                0, 1
            }
        ),
        Unit_already_exists
    );

    EXPECT_NO_THROW(
        usys.add_new_unit(
            Unit_information{
                "kilogram",
                Unit_type::mass,
                0, 1
            }
        )
    );
}
