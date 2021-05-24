#include <sstream>
#include "token.hpp"
#include "exceptions.hpp"

using std::istringstream;

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
        case '*':
        case '/':
        case '(':
        case ')':
            toks.push_back(
                Token{
                    .kind = Token_type::operator_type,
                    .op = ch
                }
            );
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
            ss.putback(ch);
            double v;
            ss >> v;

            toks.push_back(
                Token{
                    .kind = Token_type::number,
                    .val = v
                }
            );
            break;
        }
        default:
            throw Unknown_token{ch};
        }
    }

    return toks;
}
