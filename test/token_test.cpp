#include <gtest/gtest.h>
#include "token/token.hpp"

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
    
    auto paren_open = tokenize("(");
    EXPECT_EQ(paren_open.size(), 1);
    EXPECT_EQ(paren_open[0].kind, Token_type::operator_type);
    EXPECT_EQ(paren_open[0].op, '(');
    
    auto paren_close = tokenize(")");
    EXPECT_EQ(paren_close.size(), 1);
    EXPECT_EQ(paren_close[0].kind, Token_type::operator_type);
    EXPECT_EQ(paren_close[0].op, ')');
}
