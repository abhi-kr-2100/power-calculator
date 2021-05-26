/**
 * pcalc, or Power Calculator, is a feature-heavy command-line calculator.
*/

#include <iostream>
#include <string>
#include <cstdlib>
#include "parser/parser.hpp"
#include "parser/exceptions.hpp"
#include "token/exceptions.hpp"

using std::cout;
using std::cin;
using std::getline;
using std::string;
using std::exit;

void calculate(Parser& calc)
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
        double result = calc.evaluate(expr);
        cout << answer << result << "\n";
    }
    catch (Unknown_token& err)
    {
        cout << error << err.what() << "\n";
    }
    catch (Divide_by_zero& err)
    {
        cout << error << err.what() << "\n";
    }
    catch (Syntax_error& err)
    {
        cout << error << err.what() << "\n";
    }
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
