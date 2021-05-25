#include <utility>
#include <algorithm>
#include "parser.hpp"
#include "exceptions.hpp"

using std::pair;
using std::find;

using ull = unsigned long long;

enum class Check_nesting { yes, no };
enum class Allow_prev_op { yes, no };

pair<Token_iter, char> reverse_search(Token_iter s, Token_iter e,
    std::vector<char> to_find, Check_nesting chk_nesting = Check_nesting::yes,
    Allow_prev_op prev_op = Allow_prev_op::no)
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
            auto prev = e - 1;
            // found symbol will only be considered, if this is set to true
            bool prev_op_test = false;

            // avoid cases where op is the starting character or follows an
            // operator
            if (prev_op == Allow_prev_op::no)
            {
                if (prev->kind != Token_type::operator_type ||
                    (prev->op == ')' || prev->op == '('))
                {
                    prev_op_test = true;
                }
            }
            else
            {
                prev_op_test = true;
            }

            if (prev_op_test)
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
        return s->val;
    }

    switch (s->op)
    {
    case '(':
    {
        auto r = reverse_search(
            s, e, {')'}, Check_nesting::no, Allow_prev_op::yes);
        if (r.first == s)
        {
            throw Unmatched_parentheses{"Missing ')'"};
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
