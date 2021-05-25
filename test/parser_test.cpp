#include <gtest/gtest.h>
#include "parser/parser.hpp"
#include "parser/exceptions.hpp"

TEST(ParserPrimaryTest, Numbers)
{
    Parser calc;
    
    EXPECT_DOUBLE_EQ(calc.evaluate("12"), 12);
    EXPECT_DOUBLE_EQ(calc.evaluate("-12"), -12);
    EXPECT_DOUBLE_EQ(calc.evaluate("+12"), 12);

    EXPECT_DOUBLE_EQ(calc.evaluate(".69"), .69);
    EXPECT_DOUBLE_EQ(calc.evaluate("+.69"), .69);
    EXPECT_DOUBLE_EQ(calc.evaluate("-.69"), -.69);

    EXPECT_DOUBLE_EQ(calc.evaluate("1e3"), 1e3);
    EXPECT_DOUBLE_EQ(calc.evaluate("1E3"), 1E3);
    EXPECT_DOUBLE_EQ(calc.evaluate("1e+3"), 1e3);
    EXPECT_DOUBLE_EQ(calc.evaluate("1e-3"), 1e-3);
    EXPECT_DOUBLE_EQ(calc.evaluate("1E+3"), 1e3);
    EXPECT_DOUBLE_EQ(calc.evaluate("1E-3"), 1e-3);

    EXPECT_DOUBLE_EQ(calc.evaluate("+1e3"), 1e3);
    EXPECT_DOUBLE_EQ(calc.evaluate("+1E3"), 1E3);
    EXPECT_DOUBLE_EQ(calc.evaluate("+1e+3"), 1e3);
    EXPECT_DOUBLE_EQ(calc.evaluate("+1e-3"), 1e-3);
    EXPECT_DOUBLE_EQ(calc.evaluate("+1E+3"), 1e3);
    EXPECT_DOUBLE_EQ(calc.evaluate("+1E-3"), 1e-3);
    
    EXPECT_DOUBLE_EQ(calc.evaluate("-1e3"), -1e3);
    EXPECT_DOUBLE_EQ(calc.evaluate("-1E3"), -1E3);
    EXPECT_DOUBLE_EQ(calc.evaluate("-1e+3"), -1e3);
    EXPECT_DOUBLE_EQ(calc.evaluate("-1e-3"), -1e-3);
    EXPECT_DOUBLE_EQ(calc.evaluate("-1E+3"), -1e3);
    EXPECT_DOUBLE_EQ(calc.evaluate("-1E-3"), -1e-3);
}

TEST(ParserPrimaryTest, Expressions)
{
    Parser calc;

    EXPECT_DOUBLE_EQ(calc.evaluate("(12)"), 12);
    EXPECT_DOUBLE_EQ(calc.evaluate("(-12)"), -12);
    EXPECT_DOUBLE_EQ(calc.evaluate("(+12)"), 12);

    EXPECT_DOUBLE_EQ(calc.evaluate("(.69)"), .69);
    EXPECT_DOUBLE_EQ(calc.evaluate("(+.69)"), .69);
    EXPECT_DOUBLE_EQ(calc.evaluate("(-.69)"), -.69);

    EXPECT_DOUBLE_EQ(calc.evaluate("(1e3)"), 1e3);
    EXPECT_DOUBLE_EQ(calc.evaluate("(1E3)"), 1E3);
    EXPECT_DOUBLE_EQ(calc.evaluate("(1e+3)"), 1e3);
    EXPECT_DOUBLE_EQ(calc.evaluate("(1e-3)"), 1e-3);
    EXPECT_DOUBLE_EQ(calc.evaluate("(1E+3)"), 1e3);
    EXPECT_DOUBLE_EQ(calc.evaluate("(1E-3)"), 1e-3);

    EXPECT_DOUBLE_EQ(calc.evaluate("(+1e3)"), 1e3);
    EXPECT_DOUBLE_EQ(calc.evaluate("(+1E3)"), 1E3);
    EXPECT_DOUBLE_EQ(calc.evaluate("(+1e+3)"), 1e3);
    EXPECT_DOUBLE_EQ(calc.evaluate("(+1e-3)"), 1e-3);
    EXPECT_DOUBLE_EQ(calc.evaluate("(+1E+3)"), 1e3);
    EXPECT_DOUBLE_EQ(calc.evaluate("(+1E-3)"), 1e-3);
    
    EXPECT_DOUBLE_EQ(calc.evaluate("(-1e3)"), -1e3);
    EXPECT_DOUBLE_EQ(calc.evaluate("(-1E3)"), -1E3);
    EXPECT_DOUBLE_EQ(calc.evaluate("(-1e+3)"), -1e3);
    EXPECT_DOUBLE_EQ(calc.evaluate("(-1e-3)"), -1e-3);
    EXPECT_DOUBLE_EQ(calc.evaluate("(-1E+3)"), -1e3);
    EXPECT_DOUBLE_EQ(calc.evaluate("(-1E-3)"), -1e-3);
}

TEST(ParserPrimaryTest, BadPrimary)
{
    Parser calc;

    EXPECT_THROW(calc.evaluate("12+"), Syntax_error);
    EXPECT_ANY_THROW(calc.evaluate("4.2.3"));
    EXPECT_ANY_THROW(calc.evaluate("1e5.2"));
    EXPECT_ANY_THROW(calc.evaluate("1E5.2"));
    EXPECT_ANY_THROW(calc.evaluate("1e+5.2"));
    EXPECT_ANY_THROW(calc.evaluate("1e-5.2"));
    EXPECT_ANY_THROW(calc.evaluate("1E+5.2"));
    EXPECT_ANY_THROW(calc.evaluate("1E-5.2"));
}
