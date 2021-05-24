/**
 * pcalc, or Power Calculator, is a feature-heavy command-line calculator.
*/

#include <iostream>
#include <string>

using std::cout;
using std::cin;
using std::string;

int main()
{
    constexpr auto prompt = "> ";
    constexpr auto answer = "= ";
    constexpr auto error = "! ";

    cout << "Welcome to Power Calculator!\n";

    while (true)
    {
        cout << prompt;
        string expr;
        cin >> expr;

        if (cin.eof())
        {
            cout << "\nBye!\n";
            break;
        }
        if (cin.bad())
        {
            cout << error << "Can't read input! Terminating...\n";
            break;
        }
        
        cout << answer << expr << "\n";
    }
}
