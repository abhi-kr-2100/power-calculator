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
