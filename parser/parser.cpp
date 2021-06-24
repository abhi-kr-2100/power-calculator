#include <utility>
#include <algorithm>

#include "parser.hpp"
#include "exceptions.hpp"

using std::pair;
using std::find;
using std::vector;
using std::string;

using ull = unsigned long long;

template <class T>
bool contains(const vector<T>& v, const T& e)
{
    return find(v.begin(), v.end(), e) != v.end();
}

/**
 * Does the given string represent a valid C++ identifier?
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

double Parser::evaluate(const string& expr)
{
    auto tokens = tokenize(expr);
    if (tokens.size() == 0)
    {
        // TODO: throw an exception instead
        return 0;
    }

    if (tokens[0].kind == Token_type::identifier &&
        tokens[0].name == var_declaration_key)
    {
        return variable_declaration(tokens.begin(), tokens.end());
    }
    
    return expression(tokens.begin(), tokens.end());
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
    auto i = reverse_search(s, e, {'*', '/'});
    if (i.first != s)
    {
        switch (i.second)
        {
        case '*':
            return term(s, i.first) * primary(i.first + 1, e);
        case '/':
        {
            auto divisor = primary(i.first + 1, e);
            if (divisor == 0)
            {
                throw Runtime_error{"Division by 0."};
            }
            return term(s, i.first) / divisor;
        }
        }
    }

    return primary(s, e);
}

double Parser::primary(const Token_iter& s, const Token_iter& e)
{
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

    switch (s->op)
    {
    case '(':
    {
        if ((e - 1)->op != ')')
        {
            throw Syntax_error{"Missing ')'."};
        }

        return expression(s + 1, e - 1);
    }
    case '+':
        return +primary(s + 1, e);
    case '-':
        return -primary(s + 1, e);
    }

    throw Syntax_error{"Primary expected."};
}
