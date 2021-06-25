#include <sstream>
#include <string>
#include <cctype>

#include "token.hpp"
#include "exceptions.hpp"

using std::istringstream;
using std::isalpha;
using std::isalnum;
using std::string;

std::vector<Token> tokenize(const std::string& expr)
{
    istringstream ss{expr};
    std::vector<Token> toks;
    for (char ch {}; ss >> ch; )
    {
        switch (ch)
        {
        case '+':
        case '-':
        case '%':
        case '*':
        case '/':
        case '(':
        case ')':
        case '=':
            toks.push_back({.kind = Token_type::operator_type, .op = ch});
            break;
        case '.':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        {
            // read entire number
            ss.putback(ch);
            double v;
            ss >> v;

            if (ss.fail())
            {
                throw Bad_number{"Not a valid number."};
            }

            toks.push_back({.kind = Token_type::number, .val = v});
            break;
        }
        default:
            /**
             * Read in a variable name or the name of a command, such as `let`
            */
            if (ch == '_' || isalpha(ch))
            {
                string identifier(1, ch);
                char next;
                while (ss.get(next) && (next == '_' || isalnum(next)))
                {
                    identifier += next;
                }

                /**
                 * The above loop has ended due to one of the following reasons:
                 *  - ss has run out of characters
                 *  - an illegal character for a variable name was read
                 * 
                 * If an illegal character for a variable name was read, it must
                 * be putback because it might be useful to another part of the
                 * program.
                */
                if (ss)
                {
                    ss.putback(next);
                }

                toks.push_back(
                    {.kind = Token_type::identifier, .name = identifier}
                );

                break;
            }
            throw Unknown_token{"Unknown token."};
        }
    }

    return toks;
}
