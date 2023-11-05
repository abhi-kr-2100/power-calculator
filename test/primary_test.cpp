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
                0, 1}));

    EXPECT_THROW(
        usys.add_new_unit(
            Unit_information{
                "meter",
                Unit_type::length,
                0, 1}),
        Unit_already_exists);

    EXPECT_NO_THROW(
        usys.add_new_unit(
            Unit_information{
                "kilogram",
                Unit_type::mass,
                0, 1}));
}

TEST(Unit_system, CorrectConversions)
{
    auto usys = Unit_system();

    usys.add_new_unit(
        Unit_information{
            "meter",
            Unit_type::length,
            0, 1});

    usys.add_new_unit(
        Unit_information{
            "inch",
            Unit_type::length,
            0, 0.0254});

    EXPECT_NEAR(usys.convert(1, "inch", "meter"), 0.0254, 0.01);
    EXPECT_NEAR(usys.convert(8, "inch", "meter"), 0.2032, 0.01);
    EXPECT_NEAR(usys.convert(1, "meter", "inch"), 39.3701, 0.01);
    EXPECT_NEAR(usys.convert(3.14, "meter", "inch"), 123.622, 0.01);
}

TEST(Unit_system, IntermediateConversions)
{
    auto usys = Unit_system();

    usys.add_new_unit(
        Unit_information{
            "meter",
            Unit_type::length,
            0, 1});

    usys.add_new_unit(
        Unit_information{
            "inch",
            Unit_type::length,
            0, 0.0254});

    usys.add_new_unit(
        Unit_information{
            "centimeter",
            Unit_type::length,
            0, 0.01});

    EXPECT_NEAR(usys.convert(1, "inch", "centimeter"), 2.54, 0.01);
    EXPECT_NEAR(usys.convert(7.34, "inch", "centimeter"), 18.6436, 0.01);
    EXPECT_NEAR(usys.convert(1, "centimeter", "inch"), 0.393701, 0.01);
    EXPECT_NEAR(usys.convert(3.14, "centimeter", "inch"), 1.23622, 0.01);
}

TEST(Unit_system, CorrectConversionsWithOffset)
{
    auto usys = Unit_system();

    usys.add_new_unit(
        Unit_information{
            "celsius",
            Unit_type::temperature,
            0, 1});

    usys.add_new_unit(
        Unit_information{
            "fahrenheit",
            Unit_type ::temperature,
            -32.0 * 5.0 / 9.0, 5.0 / 9.0});

    EXPECT_NEAR(usys.convert(0, "celsius", "fahrenheit"), 32, 0.01);
    EXPECT_NEAR(usys.convert(1, "celsius", "fahrenheit"), 33.8, 0.01);
    EXPECT_NEAR(usys.convert(7, "celsius", "fahrenheit"), 44.6, 0.01);
    EXPECT_NEAR(usys.convert(0, "fahrenheit", "celsius"), -17.7778, 0.01);
    EXPECT_NEAR(usys.convert(3, "fahrenheit", "celsius"), -16.111, 0.01);
}

TEST(Unit_system, IntermediateConversionsWithOffset)
{
    auto usys = Unit_system();

    usys.add_new_unit(
        Unit_information{
            "celsius",
            Unit_type::temperature,
            0, 1});

    usys.add_new_unit(
        Unit_information{
            "fahrenheit",
            Unit_type ::temperature,
            -32.0 * 5.0 / 9.0, 5.0 / 9.0});

    usys.add_new_unit(
        Unit_information{
            "kelvin",
            Unit_type::temperature,
            -273.15, 1});

    EXPECT_NEAR(usys.convert(0, "fahrenheit", "kelvin"), 255.372, 0.01);
    EXPECT_NEAR(usys.convert(2.18, "fahrenheit", "kelvin"), 256.5833, 0.01);
    EXPECT_NEAR(usys.convert(0, "kelvin", "fahrenheit"), -459.67, 0.01);
    EXPECT_NEAR(usys.convert(5.14, "kelvin", "fahrenheit"), -450.418, 0.01);
}

TEST(Unit_system, IncorrectConversions)
{
    auto usys = Unit_system();

    usys.add_new_unit(
        Unit_information{
            "meter",
            Unit_type::length,
            0, 1});

    usys.add_new_unit(
        Unit_information{
            "kilogram",
            Unit_type::mass,
            0, 1});

    EXPECT_THROW(usys.convert(0, "meter", "kilogram"), Incompatible_units);
    EXPECT_THROW(usys.convert(1, "kilogram", "meter"), Incompatible_units);
}

TEST(Unit_system, UnknownUnit)
{
    auto usys = Unit_system();

    usys.add_new_unit(
        Unit_information{
            "meter",
            Unit_type::length,
            0, 1});

    EXPECT_THROW(usys.convert(0, "meter", "kilogram"), Unknown_unit);
    EXPECT_THROW(usys.convert(1, "kilogram", "meter"), Unknown_unit);
}

TEST(Primary, UnitlessAddition)
{
    Primary a = 5.3;
    Primary b = 8.2;

    EXPECT_DOUBLE_EQ((a + b).get_value(), 5.3 + 8.2);
}

TEST(Primary, UnitlessAdditionWithAUnitValue)
{
    Primary unitless = 3.14;

    auto usys = Unit_system();
    usys.add_new_unit(
        Unit_information{
            "meter",
            Unit_type::length,
            0, 1});

    Primary withUnit(2.71, usys, "meter");

    EXPECT_THROW(unitless + withUnit, Incompatible_units);
}

TEST(Primary, IncompatibleUnits)
{
    auto usys = Unit_system();
    usys.add_new_unit(
        Unit_information{
            "meter",
            Unit_type::length,
            0, 1});
    usys.add_new_unit(
        Unit_information{
            "kilogram",
            Unit_type::mass,
            0, 1});

    Primary a(3.14, usys, "meter");
    Primary b(2.17, usys, "kilogram");

    EXPECT_THROW(a + b, Incompatible_units);
}

TEST(Primary, SameUnit)
{
    auto usys = Unit_system();
    usys.add_new_unit(
        Unit_information{
            "meter",
            Unit_type::length,
            0, 1});

    Primary l1(3.14, usys, "meter");
    Primary l2(2.17, usys, "meter");

    EXPECT_DOUBLE_EQ((l1 + l2).get_value(), 3.14 + 2.17);
}

TEST(Primary, DifferentCompatibleUnits)
{
    auto usys = Unit_system();

    usys.add_new_unit(
        Unit_information{
            "celsius",
            Unit_type::temperature,
            0, 1});

    usys.add_new_unit(
        Unit_information{
            "fahrenheit",
            Unit_type ::temperature,
            -32.0 * 5.0 / 9.0, 5.0 / 9.0});

    Primary t1(314.217, usys, "celsius");
    Primary t2(42.52, usys, "fahrenheit");

    EXPECT_NEAR((t1 + t2).get_value(), 597.5906 + 42.52, 0.01);
}

TEST(Primary, SameCompoundUnit)
{
    auto usys = Unit_system();

    usys.add_new_unit(
        Unit_information{
            "meter",
            Unit_type::length,
            0, 1});

    usys.add_new_unit(
        Unit_information{
            "second",
            Unit_type::time,
            0, 1});

    Primary a(3.14, usys, {"meter"}, {"second"});
    Primary b(2.71, usys, {"meter"}, {"second"});

    EXPECT_DOUBLE_EQ((a + b).get_value(), 3.14 + 2.71);
}

TEST(Primary, DifferentCompoundUnits)
{
    auto usys = Unit_system();

    usys.add_new_unit(
        Unit_information{
            "meter",
            Unit_type::length,
            0, 1});
    usys.add_new_unit(
        Unit_information{
            "kilometer",
            Unit_type::length,
            0, 1000});

    usys.add_new_unit(
        Unit_information{
            "second",
            Unit_type::time,
            0, 1});
    usys.add_new_unit(
        Unit_information{
            "hour",
            Unit_type::time,
            0, 3600});

    Primary a(3.14, usys, {"meter"}, {"second"});
    Primary b(2.71, usys, {"kilometer"}, {"hour"});

    EXPECT_NEAR((a + b).get_value(), 11.304 + 2.71, 0.01);
}

TEST(Primary, IncompatibleCompoundUnits)
{
    auto usys = Unit_system();

    usys.add_new_unit(
        Unit_information{
            "meter",
            Unit_type::length,
            0, 1});
    usys.add_new_unit(
        Unit_information{
            "kilometer",
            Unit_type::length,
            0, 1000});

    usys.add_new_unit(
        Unit_information{
            "second",
            Unit_type::time,
            0, 1});
    usys.add_new_unit(
        Unit_information{
            "kelvin",
            Unit_type::temperature,
            0, 1});

    Primary a(3.14, usys, {"meter"}, {"second"});
    Primary b(2.71, usys, {"kilometer"}, {"kelvin"});

    EXPECT_THROW(a + b, Incompatible_units);
}

TEST(Primary, SameComplexUnit)
{
    auto usys = Unit_system();

    usys.add_new_unit(
        Unit_information{
            "meter",
            Unit_type::length,
            0, 1});

    usys.add_new_unit(
        Unit_information{
            "second",
            Unit_type::time,
            0, 1});

    Primary a(3.14, usys, {"meter", "meter"}, {"second", "meter"});
    Primary b(2.71, usys, {"meter", "meter"}, {"second", "meter"});

    EXPECT_DOUBLE_EQ((a + b).get_value(), 3.14 + 2.71);
}

TEST(Primary, IncompatibleComplexUnits)
{
    auto usys = Unit_system();

    usys.add_new_unit(
        Unit_information{
            "meter",
            Unit_type::length,
            0, 1});
    usys.add_new_unit(
        Unit_information{
            "kilometer",
            Unit_type::length,
            0, 1000});

    usys.add_new_unit(
        Unit_information{
            "second",
            Unit_type::time,
            0, 1});
    usys.add_new_unit(
        Unit_information{
            "kelvin",
            Unit_type::temperature,
            0, 1});

    Primary a(3.14, usys, {"meter", "kelvin"}, {"second", "second"});
    Primary b(2.71, usys, {"kilometer", "second"}, {"kelvin", "kelvin"});

    EXPECT_THROW(a + b, Incompatible_units);
}

TEST(Primary, DifferentCompatibleComplexUnits)
{
    auto usys = Unit_system();

    usys.add_new_unit(
        Unit_information{
            "meter",
            Unit_type::length,
            0, 1});

    usys.add_new_unit(
        Unit_information{
            "kilometer",
            Unit_type::length,
            0, 1000});

    usys.add_new_unit(
        Unit_information{
            "second",
            Unit_type::time,
            0, 1});

    usys.add_new_unit(
        Unit_information{
            "hour",
            Unit_type::time,
            0, 3600});

    usys.add_new_unit(
        Unit_information{
            "celsius",
            Unit_type::temperature,
            0, 1});

    usys.add_new_unit(
        Unit_information{
            "fahrenheit",
            Unit_type ::temperature,
            -32.0 * 5.0 / 9.0, 5.0 / 9.0});

    usys.add_new_unit(
        Unit_information{
            "kelvin",
            Unit_type::temperature,
            -273.15, 1});

    usys.add_new_unit(
        Unit_information{
            "kilogram",
            Unit_type::mass,
            0, 1});

    usys.add_new_unit(
        Unit_information{
            "gram",
            Unit_type::mass,
            0, 0.001});

    Primary a(3.14, usys, {"meter", "fahrenheit"}, {"second", "kilogram"});
    Primary b(2.71, usys, {"kelvin", "kilometer"}, {"hour", "gram"});

    EXPECT_NEAR((a + b).get_value(), 922.05628012, 0.01);
}
