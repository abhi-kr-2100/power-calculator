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

enum class Check_nesting { yes, no };

/**
 * Find between s and e (exclusive), from the right side, the characters given.
 * If not found, return {s, 0}.
 * 
 * Also, follow the following conditions:
 * - Ignore if the characters occur inside parentheses (default)
 * - Ignore if any operator occurs before the characters (default)
 *  - AND always ignore if the preceding operator matches a given list
 * 
 * chk_nesting determines if characters occuring inside parentheses are ignored.
 * prev_op_ignore contains the list of operators that should be ignored even
 * if they occur before a searching-for character.
 * 
 * Return a pair in which the first element is an iterator that points to where
 * between s and e, the character was found. The second element is the found
 * character itself.
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

    ull nesting = 0;    // the level of nesting
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
            auto prev_tok = (e - 1);
            bool ignoreable = contains(prev_op_ignore, prev_tok->op);

            if (prev_tok->kind != Token_type::operator_type || ignoreable)
            {
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
    return expression(tokens.begin(), tokens.end());
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
    if (s->kind == Token_type::number)
    {
        // more than just a number
        if ((s + 1) != e)
        {
            throw Syntax_error{"Only a number was expected."};
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
        return primary(s + 1, e);
    case '-':
        return -primary(s + 1, e);
    }

    throw Syntax_error{"Primary expected."};
}
