#ifndef A2100_PCALC_PARSER_HELPERS
#define A2100_PCALC_PARSER_HELPERS  1

#include <limits>
#include <utility>
#include <cmath>
#include <algorithm>
#include <vector>
#include <string>

#include "token/token.hpp"

using std::fabs;
using std::numeric_limits;
using std::find;
using std::pair;
using std::vector;
using std::string;
using std::cbrt;
using std::pow;

using ull = unsigned long long;
using ll = long long;

/**
 * Is the given identifier a reserved keyword?
*/
bool is_keyword(const string& s)
{
    if (s == "let")
    {
        return true;
    }

    return false;
}

/**
 * Is the variable assignment spanned by [s:e) syntactically correct?
 * 
 * op_pos is the position of the assignment operator.
 * 
 * A valid assignment syntax is:
 * VariableName "=" Expression
*/
bool is_valid_variable_assignment_syntax(const Token_iter& s,
    const Token_iter& e, const Token_iter& op_pos)
{
    return (
        s->kind == Token_type::identifier &&
        (s + 1) != e && (s + 1) == op_pos &&
        (s + 2) != e
    );
}

bool is_valid_variable_declaration_syntax(const Token_iter& s,
    const Token_iter& e)
{
    return (
        s->name == Parser::var_declaration_key &&
        (s + 1) != e && (s + 1)->kind == Token_type::identifier &&
        !is_keyword((s + 1)->name) &&
        (s + 2) != e && (s + 2)->op == '=' &&
        (s + 3) != e
    );
}

/**
 * Does the given tokens look like the start of a variable declaration?
*/
bool is_variable_declaration(const vector<Token>& tokens)
{
    return tokens[0].name == Parser::var_declaration_key;
}

/**
 * Are the two given doubles almost equal?
*/
bool doubles_equal(double a, double b)
{
    return fabs(a - b) < numeric_limits<double>::epsilon();
}

template <class T>
bool contains(const vector<T>& v, const T& e)
{
    return find(v.begin(), v.end(), e) != v.end();
}

/**
 * Like std::pow but can take cube roots.
 * Also, throws Runtime_error on uncomputable powers.
 * 
 * Rules for exponentiation:
 *  - base is -ve and exp is non-integer and exp != 1/3 -> error
 *  - base is -ve and exp == 1/3 -> std::cbrt(base)
 *  - base is 0 and exp is non-positive -> error
 *  - otherwise -> pow(base, exp)
*/
double power(double base, double exp)
{
    if (base < 0)
    {
        if (doubles_equal(exp, 1.0 / 3.0))
        {
            return cbrt(base);
        }

        if (!doubles_equal(ll(exp), exp))
        {
            throw Runtime_error{
                "Can't compute fractional exponent of negative base."};
        }
    }
    if (base == 0 && exp <= 0)
    {
        throw Runtime_error{"Undefined exponent."};
    }

    return pow(base, exp);
}

/** Used to indicate whether characters occuring inside parentheses should be
 * ignored.
 */
enum class Check_nesting { yes, no };

/**
 * Find in [s:e), from the right side, any character given in to_find. The
 * position of the character found, and the character found is returned (in that
 * order).
 * 
 * If none of the characters are found, return {s, 0}.
 * 
 * Even when a character from to_find has been found in [s:e), it must
 * satisfy the following conditions to be considered:
 *  - must not occur inside parentheses (default behavior; can be overridden)
 *  - must not occur after an operator
 *      - however, the client code can provide a vector of operators which
 *        can occur befor the character under consideration
 *          - by default, '(', ')', and '!' can occur before the operators
 * 
 * chk_nesting can override whether to_find characters occuring inside
 * parentheses will be considered. If chk_nesting is set to Check_nesting::yes,
 * the default condition will apply, i.e., the characters occuring inside
 * parentheses will not be considered.
 * 
 * prev_op_ignore contains the list of operators that should be ignored even
 * if they occur before a to_find character.
 * 
 * - Why such a specific function? -
 * 
 * The parsing code uses reverse_search to find operators of the same
 * precedence. For example, in the expression "4 * 3 + 2 * 4 - 5," the parsing
 * code will want to know the position of "+" or "-" in the expression from
 * the right. And it'll also want to know whether the found operator is a "+"
 * or a "-."
 * 
 * Why from the right? Because then, the expression can be broken down into its
 * constituents: "4 * 3 + 2 * 4 - 5" = "4 * 3 + 2 * 4" - "5."
 * 
 * reverse_search, by default, would not find a "+" or "-" if it occured inside
 * parentheses. Consider: "4 * 3 + 2 * (4 - 5)." This expression is different
 * from the previous one.
 * 
 * Also, "+" and "-" can be used as unary operators. For example, "4*-2." The
 * identification of a unary operator is that it occurs after another operator
 * ("*" in this case) when it occurs in the middle of an expression. Hence, by
 * default, if an operator occurs before the characters we are searching for,
 * the particular instance of the character is not considered.
 * 
 * Although parentheses are considered operators, the above assertion is not
 * true when the preceding operator is a parentheses. This is an example:
 * "(4)+2." To take care of this situation, reverse_search, by default, doesn't
 * consider "(" or ")" as operators.
 * 
 * All of this can be overriden or extended in new ways through the appropriate
 * parameters.
*/
pair<Token_iter, char> reverse_search(Token_iter s, Token_iter e,
    vector<char> to_find, Check_nesting chk_nesting = Check_nesting::yes,
    vector<char> prev_op_ignore = {'(', ')', '!'})
{
    // nothing to search
    if (e == s)
    {
        return {s, 0};
    }

    ull nesting = 0;    // the level of nesting; only updated if chk_nesting is
                        // Check_nesting::yes
    for (--e; e != s; --e)
    {
        if (chk_nesting == Check_nesting::yes)
        {
            switch (e->op)
            {
            case ')':
                ++nesting;
                break;
            case '(':
                --nesting;
                break;
            }
        }

        if (!nesting)
        {
            /**
             * Generally, a character that occurs after an operator is not
             * considered. However, the client code can provide a vector of
             * ignorable operators. Characters that occur after these ignorale
             * operators are considered.
             * 
             * In other words, operators given in prev_op_ignore are not
             * considered operators for the purposes of this function.
            */
            auto prev_tok = (e - 1);
            bool ignoreable = contains(prev_op_ignore, prev_tok->op);

            if (ignoreable || prev_tok->kind != Token_type::operator_type)
            {
                // which to_find character have we found, if any?
                auto pos = find(to_find.begin(), to_find.end(), e->op);
                if (pos != to_find.end())
                {
                    return {e, *pos};
                }
            }
        }
    }

    return {s, 0};
}

/**
 * Return the location of `to_find` in [s:e). If not found, return `e`.
 * Additionally, unless `chk_nesting` is `Check_nesting::no`, find only those
 * instances which don't occur inside parentheses.
*/
Token_iter find_forward(Token_iter s, Token_iter e, char to_find,
    Check_nesting chk_nesting = Check_nesting::yes)
{
    // nothing to search
    if (e == s)
    {
        return s;
    }

    ull nesting = 0;    // how many levels deep are we nested between '(' ')'
                        // only affected if chk_nesting is Check_nesting::yes
    for (; s != e; ++s)
    {
        if (chk_nesting == Check_nesting::yes)
        {
            switch (s->op)
            {
            case '(':
                ++nesting;
                break;
            case ')':
                --nesting;
                break;
            }
        }

        if (s->op == to_find && !nesting)
        {
            return s;
        }
    }

    return e;
}

#endif
