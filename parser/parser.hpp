#ifndef A2100_PCALC_PARSER
#define A2100_PCALC_PARSER  1

/**
 * This library provides:
 * - The Parser UDT to evaluate calculator expressions
*/

#include <string>
#include <vector>
#include "token/token.hpp"

using Token_iter = std::vector<Token>::const_iterator;

/**
 * The Parser class provides an evaluate method that evaluates a given
 * expression (expression is given as a string).
 * 
 * It uses the following grammar:
 * 
 * Statement:
 *      VariableDeclaration
 *      Expression
 * VariableDeclaration:
 *      "let" VariableName "=" Expression
 * VariableName:
 *      any valid C++ identifier
 * Expression:
 *      Expression + Term
 *      Expression - Term
 *      Term
 * Term
 *      Term * Primary
 *      Term / Primary
 *      Primary
 * Primary
 *      ( Expression )
 *      Number
 *      VariableName
 *      - Primary
 *      + Primary
*/
class Parser
{
public:
    double evaluate(const std::string& expr);
private:
    double expression(const Token_iter& s, const Token_iter& e);
    double term(const Token_iter& s, const Token_iter& e);
    double primary(const Token_iter& s, const Token_iter& e);
};

#endif
