#include <utility>
#include <algorithm>
#include "parser.hpp"
#include "exceptions.hpp"

using std::pair;
using std::find;

using ull = unsigned long long;

pair<Token_iter, char> reverse_search(Token_iter s, Token_iter e,
    std::vector<char> to_find)
{
    ull nesting = 0;    // the level of nesting
    for (--e; e != s; --e)
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

        if (!nesting)
        {
            // avoid cases where op is the starting character or follows an
            // operator
            if (e != s && (e - 1)->kind != Token_type::operator_type)
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
        if ((e - 1)->op != ')')
        {
            throw Unmatched_parentheses{};
        }
        return expression(s + 1, e - 1);
    case '+':
        return primary(s + 1, e);
    case '-':
        return -primary(s + 1, e);
    }

    throw Syntax_error{"Primary expected."};
}
