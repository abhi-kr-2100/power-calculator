#ifndef A2100_PCALC_PARSER
#define A2100_PCALC_PARSER  1

/**
 * This library provides:
 * - The Parser UDT to evaluate calculator expressions
*/

#include <string>
#include <vector>
#include <map>

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
 * Assignment: (see "Why is Assignment defined like this?" below.)
 *      Assignment "=" Assignment
 *      Expression
 * Expression:
 *      VariableName "=" Expression
 *      Expression + Term
 *      Expression - Term
 *      Term
 * Term:
 *      Term % Exponent
 *      Term * Exponent
 *      Term / Exponent
 *      Exponent
 * Exponent: (see "Why is Exponent defined like this?" below.)
 *      - Exponent
 *      + Exponent
 *      Primary ^ Exponent
 *      Primary
 * Primary:
 *      Primary!
 *      ( Assignment )
 *      Number
 *      VariableName
 * Number:
 *      a valid C++ double literal
 * 
 * -- Why is Assignment defined like this? --
 * 
 * Assignment must be able to handle cases of chain assignments: "x = y = 4*3".
 * Moreover, "5 + (x = 1)" is also meaningful.
 * 
 * -- Why is Exponent defined like this? --
 * 
 * Exponents in pcalc behave similarly to the exponents in Python, i.e.,
 * -2 ^ 5 is -(2^5) [not (-2) ^ 5], hence, Exponent must handle the signs.
 * 
 * Additionally, the associativity of ^ is from left-to-right. That's why, it's
 * Primary ^ Exponent instead of Exponent ^ Primary.
*/
class Parser
{
public:
    double evaluate(const std::string& expr) {
        return evaluate(expr, variables_table);
    }
    
    double evaluate(const std::string& expr,
        std::map<std::string, double>& variables_table);

    // the keyword used to introduce a new variable
    inline static const std::string var_declaration_key = "let";
private:
    std::map<std::string, double> variables_table;
    
    double variable_declaration(const Token_iter& s, const Token_iter& e,
        std::map<std::string, double>& variables_table);
    double assignment(const Token_iter& s, const Token_iter& e,
        std::map<std::string, double>& variables_table);
    double expression(const Token_iter& s, const Token_iter& e,
        std::map<std::string, double>& variables_table);
    double term(const Token_iter& s, const Token_iter& e,
        std::map<std::string, double>& variables_table);
    double exponent(const Token_iter& s, const Token_iter& e,
        std::map<std::string, double>& variables_table);
    double primary(const Token_iter& s, const Token_iter& e,
        std::map<std::string, double>& variables_table);
};

#endif
