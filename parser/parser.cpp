#include <utility>
#include <algorithm>
#include "parser.hpp"
#include "exceptions.hpp"

using std::pair;
using std::find;

using ull = unsigned long long;

bool is_paren(Token t)
{
    return t.kind == Token_type::operator_type && (t.op == '(' || t.op == ')');
}

enum class Check_nesting { yes, no };

/**
 * Find between s and e (exclusive), from the right side, the characters given.
 * Also, follow the following conditions:
 * - Ignore if the characters occur inside parentheses (default)
 * - Ignore if any operator occurs before the characters (default)
 *  - OR only ignore if the preceding operator matches a given list
 * 
 * chk_nesting determines if characters occuring inside parentheses are ignored.
 * prev_op_ignore contains the list of operators that should be ignored even
 * if they occur before a searching-for character.
*/
pair<Token_iter, char> reverse_search(Token_iter s, Token_iter e,
    std::vector<char> to_find, Check_nesting chk_nesting = Check_nesting::yes,
    std::vector<char> prev_op_ignore = {'(', ')'})
{
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

        if (!nesting && e != s)
        {
            auto prev_op = (e - 1);
            // can the previous op be ignored?
            bool ignoreable = \
                find(
                    prev_op_ignore.begin(), prev_op_ignore.end(), prev_op->op
                ) != prev_op_ignore.end();

            if (prev_op->kind != Token_type::operator_type || ignoreable)
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

double Parser::evaluate(const std::string& expr)
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
                throw Divide_by_zero{"Division by 0."};
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
        auto nxt = s + 1;

        // more than just a number: "54)", or two consecutive numbers
        if (nxt != e)
        {
            // two consecutive numbers: "5.2 4.8" or 23(23)
            if (nxt->kind == Token_type::number || nxt->op == '(')
            {
                throw Syntax_error{"Missing operator between operands."};
            }

            throw Unmatched_parentheses{"Missing '('"};
        }
        
        return s->val;
    }

    switch (s->op)
    {
    case '(':
    {
        auto r = reverse_search(
            s, e, {')'}, Check_nesting::no);
        
        if (r.first == s)
        {
            throw Unmatched_parentheses{"Missing ')'"};
        }

        // check for empty parens: ()
        if (r.first == (s + 1))
        {
            throw Syntax_error{"Nothing inside parentheses!"};
        }
        
        return expression(s + 1, r.first);
    }
    case '+':
        return primary(s + 1, e);
    case '-':
        return -primary(s + 1, e);
    }

    throw Syntax_error{"Primary expected."};
}
