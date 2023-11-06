/**
 * pcalc, or Power Calculator, is a feature-heavy command-line calculator.
 */

#include <iostream>
#include <string>
#include <cstdlib>
#include <stdexcept>

#include "parser/parser.hpp"
#include "parser/exceptions.hpp"
#include "token/exceptions.hpp"

using std::cerr;
using std::cin;
using std::cout;
using std::exception;
using std::exit;
using std::getline;
using std::string;

void calculate(Parser &calc);

void add_units_to_parser(Parser &calc);

int main()
{
    cout << "Welcome to Power Calculator!\n";

    Parser calc;
    add_units_to_parser(calc);

    while (true)
    {
        calculate(calc);
    }
}

/**
 * Display prompt -> take input -> print result/error.
 */
void calculate(Parser &calc)
{
    constexpr auto prompt = "> ";
    constexpr auto answer = "= ";
    constexpr auto error = "! ";

    cout << prompt;
    string expr;
    getline(cin, expr);

    if (cin.eof())
    {
        cout << "\nBye!\n";
        exit(EXIT_SUCCESS);
    }
    if (cin.bad())
    {
        cout << error << "Can't read input! Terminating...\n";
        exit(EXIT_FAILURE);
    }

    try
    {
        const auto result = calc.evaluate(expr);
        cout << answer << result << "\n";
    }
    catch (exception &ex)
    {
        cerr << error << ex.what() << "\n";
    }
}

void add_units_to_parser(Parser &calc)
{
    calc.unit_system.add_new_unit(
        Unit_information{
            "meter",
            Unit_type::length,
            0, 1});
    calc.unit_system.add_new_unit(
        Unit_information{
            "kilometer",
            Unit_type::length,
            0, 1000});
    calc.unit_system.add_new_unit(
        Unit_information{
            "inch",
            Unit_type::length,
            0, 0.0254});
    calc.unit_system.add_new_unit(
        Unit_information{
            "foot",
            Unit_type::length,
            0, 0.3048});
    calc.unit_system.add_new_unit(
        Unit_information{
            "mile",
            Unit_type::length,
            0, 1609.344});
    calc.unit_system.add_new_unit(
        Unit_information{
            "league",
            Unit_type::length,
            0, 4800});

    calc.unit_system.add_new_unit(
        Unit_information{
            "kilogram",
            Unit_type::mass,
            0, 1});
    calc.unit_system.add_new_unit(
        Unit_information{
            "gram",
            Unit_type::mass,
            0, 0.001});
    calc.unit_system.add_new_unit(
        Unit_information{
            "tonne",
            Unit_type::mass,
            0, 1000});
    calc.unit_system.add_new_unit(
        Unit_information{
            "pound",
            Unit_type::mass,
            0, 0.45});

    calc.unit_system.add_new_unit(
        Unit_information{
            "second",
            Unit_type::time,
            0, 1});
    calc.unit_system.add_new_unit(
        Unit_information{
            "minute",
            Unit_type::time,
            0, 60});
    calc.unit_system.add_new_unit(
        Unit_information{
            "hour",
            Unit_type::time,
            0, 3600});

    calc.unit_system.add_new_unit(
        Unit_information{
            "kelvin",
            Unit_type::temperature,
            -273.15, 1});
    calc.unit_system.add_new_unit(
        Unit_information{
            "celsius",
            Unit_type::temperature,
            0, 1});
    calc.unit_system.add_new_unit(
        Unit_information{
            "fahrenheit",
            Unit_type::temperature,
            -32.0 * 5.0 / 9.0, 5.0 / 9.0});
}
