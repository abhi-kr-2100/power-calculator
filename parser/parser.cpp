#include "parser.hpp"
#include "exceptions.hpp"
#include "parser/parser_helpers.hpp"

double Parser::evaluate(const string& expr,
        std::map<std::string, double>& variables_table)
{
    auto tokens = tokenize(expr);
    if (tokens.size() == 0)
    {
        throw Syntax_error{"Empty expression."};
    }

    if (is_variable_declaration(tokens))
    {
        return variable_declaration(
            tokens.begin(), tokens.end(), variables_table);
    }
    
    return assignment(tokens.begin(), tokens.end(), variables_table);
}

double Parser::variable_declaration(const Token_iter& s, const Token_iter& e,
        std::map<std::string, double>& variables_table)
{
    if (!is_valid_variable_declaration_syntax(s, e))
    {
        throw Syntax_error{"Invalid variable declaration syntax."};
    }

    auto var_name_iter = s + 1;
    auto var_name = var_name_iter->name;
    if (variables_table.find(var_name) != variables_table.end())
    {
        throw Runtime_error{"Redeclaration of variable."};
    }

    auto exp_iter = s + 3;
    auto val = expression(exp_iter, e, variables_table);
    variables_table[var_name] = val;

    return val;
}

double Parser::assignment(const Token_iter& s, const Token_iter& e,
        std::map<std::string, double>& variables_table)
{
    auto i = find_forward(s, e, '=');

    if (i == e)
    {
        return expression(s, e, variables_table);
    }

    if (!is_valid_variable_assignment_syntax(s, e, i))
    {
        throw Syntax_error{"Not a valid assignment."};
    }

    auto var = variables_table.find(s->name);
    if (var == variables_table.end())
    {
        throw Runtime_error{"Variable not defined."};
    }

    double val = assignment(i + 1, e, variables_table);
    variables_table[s->name] = val;

    return val;
}

double Parser::expression(const Token_iter& s, const Token_iter& e,
        std::map<std::string, double>& variables_table)
{
    auto i = reverse_search(s, e, {'+', '-'});
    if (i.first != s)
    {
        switch (i.second)
        {
        case '+':
            return expression(s, i.first, variables_table) + \
                term(i.first + 1, e, variables_table);
        case '-':
            return expression(s, i.first, variables_table) - \
                term(i.first + 1, e, variables_table);
        }
    }

    return term(s, e, variables_table);
}

double Parser::term(const Token_iter& s, const Token_iter& e,
        std::map<std::string, double>& variables_table)
{
    auto i = reverse_search(s, e, {'*', '/', '%'});
    if (i.first != s)
    {
        switch (i.second)
        {
        case '*':
            return term(s, i.first, variables_table) * \
                exponent(i.first + 1, e, variables_table);
        case '/':
        case '%':
        {
            auto divisor = exponent(i.first + 1, e, variables_table);
            if (divisor == 0)
            {
                throw Runtime_error{"Division or mod by 0."};
            }

            auto t = term(s, i.first, variables_table);
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

    return exponent(s, e, variables_table);
}

double Parser::exponent(const Token_iter& s, const Token_iter& e,
        std::map<std::string, double>& variables_table)
{
    switch (s->op)
    {
    case '-':
        return -exponent(s + 1, e, variables_table);
    case '+':
        return +exponent(s + 1, e, variables_table);
    }

    auto exp_pos = find_forward(s, e, '^');
    if (exp_pos != e)
    {
        auto base = primary(s, exp_pos, variables_table);
        auto exp = exponent(exp_pos + 1, e, variables_table);

        return power(base, exp);
    }

    return primary(s, e, variables_table);
}

double Parser::primary(const Token_iter& s, const Token_iter& e,
        std::map<std::string, double>& variables_table)
{
    if ((e - 1)->op == '!')
    {
        if (s == (e - 1))
        {
            throw Syntax_error{"Argument for '!' not provided."};
        }

        auto arg = primary(s, e - 1, variables_table);
        if (arg < 0)
        {
            throw Runtime_error{
                "Factorial is only defined for non-negative numbers."};
        }

        // tgamma(x + 1) == x!
        return tgamma(arg + 1);
    }

    if ((e - 1)->kind == Token_type::identifier && s != (e - 1))
    {
        return primary(s, e - 1, variables_table);
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

        return assignment(s + 1, e - 1, variables_table);
    }

    throw Syntax_error{"Primary expected."};
}
