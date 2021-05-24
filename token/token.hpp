#ifndef A2100_PCALC_TOKEN
#define A2100_PCALC_TOKEN   1

/**
 * This library provides:
 * - Token UDT to represent a calculator token
 * - Token_type UDT to differentiate between different types of Token
 * - Token_stream UDT to enable tokenized input
 * - The tokenize() function to get a vector of tokens from a string
*/

#include <istream>
#include <string>
#include <vector>

enum class Token_type
{
    operator_type, number
};

struct Token
{
    Token_type kind {};
    char op {};    // in case the token is an operator
    double val {}; // in case the token is a number
};

class Token_stream
{
private:
    std::istream& reader;
    Token buffer;
    bool full;  // is the buffer full?

public:
    Token_stream(std::istream& in)
        : reader{in}
    {
    }

    Token get();
    void putback(Token t);
};

std::vector<Token> tokenize(const std::string& expr);

#endif
