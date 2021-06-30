#include <utility>
#include <algorithm>
#include <cmath>
#include <limits>
#include <iostream>

#include "parser.hpp"
#include "exceptions.hpp"

using std::pair;
using std::find;
using std::find_if;
using std::vector;
using std::string;
using std::fmod;
using std::pow;
using std::numeric_limits;
using std::fabs;
using std::cbrt;
using std::tgamma;

using ull = unsigned long long;
using ll = long long;

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
    vector<char> prev_op_ignore = {'(', ')'})
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

double Parser::evaluate(const string& expr)
{
    auto tokens = tokenize(expr);
    if (tokens.size() == 0)
    {
        throw Syntax_error{"Empty expression."};
    }

    if (tokens[0].kind == Token_type::identifier &&
        tokens[0].name == var_declaration_key)
    {
        return variable_declaration(tokens.begin(), tokens.end());
    }
    
    return assignment(tokens.begin(), tokens.end());
}

double Parser::variable_declaration(const Token_iter& s, const Token_iter& e)
{
    if (!(s->kind == Token_type::identifier && s->name == var_declaration_key))
    {
        throw Syntax_error{var_declaration_key + " was expected."};
    }

    auto var_name_iter = s + 1;
    if (var_name_iter == e)
    {
        throw Syntax_error{"Incomplete variable declaration."};
    }
    if (var_name_iter->kind != Token_type::identifier)
    {
        throw Syntax_error{"A variable's name was expected."};
    }
    if (is_keyword(var_name_iter->name))
    {
        throw Syntax_error{var_name_iter->name + " is a reserved keyword."};
    }

    auto var_name = var_name_iter->name;
    if (variables_table.find(var_name) != variables_table.end())
    {
        throw Runtime_error{"Redeclaration of variable."};
    }

    auto equal_sign_iter = s + 2;
    if (equal_sign_iter == e)
    {
        throw Syntax_error{"Incomplete variable declaration."};
    }
    if (equal_sign_iter->op != '=')
    {
        throw Syntax_error{"An equal sign ('=') was expected."};
    }

    auto exp_iter = s + 3;
    if (exp_iter == e)
    {
        throw Syntax_error{"No expression given to assign."};
    }

    auto val = expression(exp_iter, e);
    variables_table[var_name] = val;

    return val;
}

double Parser::assignment(const Token_iter& s, const Token_iter& e)
{
    auto i = find_forward(s, e, '=');

    if (i == s)     // consider: "=5"
    {
        throw Syntax_error{"No variable to assign to."};
    }

    if (i == e)
    {
        return expression(s, e);
    }

    // consider:
    // "5 x = 5" or "4 = 5"
    if (s + 1 != i || s->kind != Token_type::identifier)
    {
        throw Syntax_error{"Not a valid L-value expression."};
    }

    if (i + 1 == e)
    {
        throw Syntax_error{"Nothing to assign."};
    }

    auto var = variables_table.find(s->name);
    if (var == variables_table.end())
    {
        throw Runtime_error{"Variable not defined."};
    }

    double val = assignment(i + 1, e);
    variables_table[s->name] = val;

    return val;
}

double Parser::expression(const Token_iter& s, const Token_iter& e)
{
    auto i = reverse_search(s, e, {'+', '-'});
    if (i.first != s)
    {
        switch (i.second)
        {
        case '+':
            return expression(s, i.first) + term(i.first + 1, e);
        case '-':
            return expression(s, i.first) - term(i.first + 1, e);
        }
    }

    return term(s, e);
}

double Parser::term(const Token_iter& s, const Token_iter& e)
{
    auto i = reverse_search(s, e, {'*', '/', '%'});
    if (i.first != s)
    {
        switch (i.second)
        {
        case '*':
            return term(s, i.first) * exponent(i.first + 1, e);
        case '/':
        case '%':
        {
            auto divisor = exponent(i.first + 1, e);
            if (divisor == 0)
            {
                throw Runtime_error{"Division or mod by 0."};
            }

            auto t = term(s, i.first);
            if (i.second == '/')
            {
                return t / divisor;
            }
            else
            {
                return fmod(t, divisor);
            }
        }
        }
    }

    return exponent(s, e);
}

double Parser::exponent(const Token_iter& s, const Token_iter& e)
{
    switch (s->op)
    {
    case '-':
        return -exponent(s + 1, e);
    case '+':
        return +exponent(s + 1, e);
    }

    auto is_exp_operator = [](const Token& t) { return t.op == '^'; };
    auto exp_pos = find_if(s, e, is_exp_operator);
    if (exp_pos != e)
    {
        auto base = primary(s, exp_pos);
        auto exp = exponent(exp_pos + 1, e);

        /**
         * Rules for exponentiation:
         *  - base is -ve and exp is non-integer and exp != 1/3 -> error
         *  - base is -ve and exp == 1/3 -> std::cbrt(base)
         *  - base is 0 and exp is non-positive -> error
         *  - otherwise -> pow(base, exp)
        */
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

    return primary(s, e);
}

double Parser::primary(const Token_iter& s, const Token_iter& e)
{
    if ((e - 1)->op == '!')
    {
        if (s == (e - 1))
        {
            throw Syntax_error{"Argument for '!' not provided."};
        }

        auto arg = primary(s, e - 1);
        if (arg < 0)
        {
            throw Runtime_error{
                "Factorial is only defined for non-negative numbers."};
        }

        // tgamma(x + 1) == x!
        return tgamma(arg + 1);
    }
    
    if (s->kind == Token_type::number || s->kind == Token_type::identifier)
    {
        /**
         * A primary that is a number, should only be a number. It can't be
         * followed by anything else.
        */
        if ((s + 1) != e)
        {
            throw Syntax_error{"Only a primary was expected."};
        }

        if (s->kind == Token_type::identifier)
        {
            auto var = variables_table.find(s->name);
            if (var == variables_table.end())
            {
                throw Runtime_error{"Variable not found."};
            }

            return var->second;
        }
        
        return s->val;
    }

    if (s->op == '(')
    {
        if ((e - 1)->op != ')')
        {
            throw Syntax_error{"Missing ')'."};
        }

        return assignment(s + 1, e - 1);
    }

    throw Syntax_error{"Primary expected."};
}
