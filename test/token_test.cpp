#include <gtest/gtest.h>
#include "token/token.hpp"
#include "token/exceptions.hpp"

TEST(TokenizeTest, Operators)
{
    auto plus = tokenize("+");
    EXPECT_EQ(plus.size(), 1);
    EXPECT_EQ(plus[0].kind, Token_type::operator_type);
    EXPECT_EQ(plus[0].op, '+');

    auto minus = tokenize("-");
    EXPECT_EQ(minus.size(), 1);
    EXPECT_EQ(minus[0].kind, Token_type::operator_type);
    EXPECT_EQ(minus[0].op, '-');

    auto multiply = tokenize("*");
    EXPECT_EQ(multiply.size(), 1);
    EXPECT_EQ(multiply[0].kind, Token_type::operator_type);
    EXPECT_EQ(multiply[0].op, '*');
    
    auto divide = tokenize("/");
    EXPECT_EQ(divide.size(), 1);
    EXPECT_EQ(divide[0].kind, Token_type::operator_type);
    EXPECT_EQ(divide[0].op, '/');

    auto mod = tokenize("%");
    EXPECT_EQ(mod.size(), 1);
    EXPECT_EQ(mod[0].kind, Token_type::operator_type);
    EXPECT_EQ(divide[0].op, '%');
    
    auto paren_open = tokenize("(");
    EXPECT_EQ(paren_open.size(), 1);
    EXPECT_EQ(paren_open[0].kind, Token_type::operator_type);
    EXPECT_EQ(paren_open[0].op, '(');
    
    auto paren_close = tokenize(")");
    EXPECT_EQ(paren_close.size(), 1);
    EXPECT_EQ(paren_close[0].kind, Token_type::operator_type);
    EXPECT_EQ(paren_close[0].op, ')');
}

TEST(TokenizeTest, Numbers)
{
    auto integer = tokenize("42");
    EXPECT_EQ(integer.size(), 1);
    EXPECT_EQ(integer[0].kind, Token_type::number);
    EXPECT_DOUBLE_EQ(integer[0].val, 42.0);

    auto pure_float = tokenize(".69");
    EXPECT_EQ(pure_float.size(), 1);
    EXPECT_EQ(pure_float[0].kind, Token_type::number);
    EXPECT_DOUBLE_EQ(pure_float[0].val, .69);

    auto mixed = tokenize("42.69");
    EXPECT_EQ(mixed.size(), 1);
    EXPECT_EQ(mixed[0].kind, Token_type::number);
    EXPECT_DOUBLE_EQ(mixed[0].val, 42.69);

    auto exp = tokenize("1e5");
    EXPECT_EQ(exp.size(), 1);
    EXPECT_EQ(exp[0].kind, Token_type::number);
    EXPECT_DOUBLE_EQ(exp[0].val, 1e5);

    auto EXP = tokenize("1E5");
    EXPECT_EQ(EXP.size(), 1);
    EXPECT_EQ(EXP[0].kind, Token_type::number);
    EXPECT_DOUBLE_EQ(EXP[0].val, 1E5);
    
    auto exp_plus = tokenize("1e+5");
    EXPECT_EQ(exp_plus.size(), 1);
    EXPECT_EQ(exp_plus[0].kind, Token_type::number);
    EXPECT_DOUBLE_EQ(exp_plus[0].val, 1e+5);

    auto exp_minus = tokenize("1e-5");
    EXPECT_EQ(exp_minus.size(), 1);
    EXPECT_EQ(exp_minus[0].kind, Token_type::number);
    EXPECT_DOUBLE_EQ(exp_minus[0].val, 1e-5);
    
    auto EXP_plus = tokenize("1E+5");
    EXPECT_EQ(EXP_plus.size(), 1);
    EXPECT_EQ(EXP_plus[0].kind, Token_type::number);
    EXPECT_DOUBLE_EQ(EXP_plus[0].val, 1E+5);

    auto EXP_minus = tokenize("1E-5");
    EXPECT_EQ(EXP_minus.size(), 1);
    EXPECT_EQ(EXP_minus[0].kind, Token_type::number);
    EXPECT_DOUBLE_EQ(EXP_minus[0].val, 1E-5);
}

TEST(TokenizeTest, Identifiers)
{
    auto identifier = tokenize("varName");
    EXPECT_EQ(identifier.size(), 1);
    EXPECT_EQ(identifier[0].kind, Token_type::identifier);
    EXPECT_EQ(identifier[0].name, "varName");

    auto var_decl = tokenize("let x = 42");
    EXPECT_EQ(var_decl.size(), 4);
    EXPECT_EQ(var_decl[0].kind, Token_type::identifier);
    EXPECT_EQ(var_decl[0].name, "let");
    EXPECT_EQ(var_decl[1].kind, Token_type::identifier);
    EXPECT_EQ(var_decl[1].name, "x");
    EXPECT_EQ(var_decl[2].kind, Token_type::operator_type);
    EXPECT_EQ(var_decl[2].op, '=');
    EXPECT_EQ(var_decl[3].kind, Token_type::number);
    EXPECT_DOUBLE_EQ(var_decl[3].val, 42);

    auto id_underscore = tokenize("_");
    EXPECT_EQ(id_underscore.size(), 1);
    EXPECT_EQ(id_underscore[0].kind, Token_type::identifier);
    EXPECT_EQ(id_underscore[0].name, "_");

    auto id_with_number = tokenize("_123 x123y");
    EXPECT_EQ(id_with_number.size(), 2);
    EXPECT_EQ(id_with_number[0].kind, Token_type::identifier);
    EXPECT_EQ(id_with_number[0].name, "_123");
    EXPECT_EQ(id_with_number[1].kind, Token_type::identifier);
    EXPECT_EQ(id_with_number[1].name, "x123y");

    auto id_and_number = tokenize("123_");
    EXPECT_EQ(id_and_number.size(), 2);
    EXPECT_EQ(id_and_number[0].kind, Token_type::number);
    EXPECT_DOUBLE_EQ(id_and_number[0].val, 123);
    EXPECT_EQ(id_and_number[1].kind, Token_type::identifier);
    EXPECT_EQ(id_and_number[1].name, "_");

    auto id_and_exp_num = tokenize("42e2xyz");
    EXPECT_EQ(id_and_exp_num.size(), 2);
    EXPECT_EQ(id_and_exp_num[0].kind, Token_type::number);
    EXPECT_DOUBLE_EQ(id_and_exp_num[0].val, 42e2);
    EXPECT_EQ(id_and_exp_num[1].kind, Token_type::identifier);
    EXPECT_EQ(id_and_exp_num[1].name, "xyz");
}

TEST(TokenizeTest, UnknownOperator)
{
    EXPECT_THROW(tokenize("$"), Unknown_token);
}

TEST(TokenizeTest, BadNumber)
{
    EXPECT_THROW(tokenize("12$"), Unknown_token);
    EXPECT_THROW(tokenize("42e"), Bad_number);
    EXPECT_THROW(tokenize(".e"), Bad_number);
    EXPECT_THROW(tokenize(".42e"), Bad_number);
}

TEST(TokenizeTest, BadVarNames)
{
    EXPECT_THROW(tokenize("$xyz"), Unknown_token);
    EXPECT_THROW(tokenize("xyz$"), Unknown_token);
}
