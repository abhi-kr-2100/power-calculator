/**
 * pcalc, or Power Calculator, is a feature-heavy command-line calculator.
*/

#include <iostream>
#include <string>
#include <cstdlib>
#include "parser/parser.hpp"

using std::cout;
using std::cin;
using std::string;
using std::exit;

void calculate(Parser& calc)
{
    constexpr auto prompt = "> ";
    constexpr auto answer = "= ";
    constexpr auto error = "! ";

    cout << prompt;
    string expr;
    cin >> expr;

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
    
    cout << answer << calc.evaluate(expr) << "\n";
}

int main()
{
    cout << "Welcome to Power Calculator!\n";
    Parser calc;

    while (true)
    {
        calculate(calc);
    }
}
