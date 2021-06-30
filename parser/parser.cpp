#include "parser.hpp"
#include "exceptions.hpp"
#include "parser/parser_helpers.hpp"

using ll = long long;

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
