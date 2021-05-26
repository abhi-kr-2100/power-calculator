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

TEST(ParserPrimaryTest, BadParens)
{
    Parser calc;

    EXPECT_THROW(calc.evaluate("(5"), Unmatched_parentheses);
    EXPECT_THROW(calc.evaluate("(5)+(5"), Unmatched_parentheses);
    EXPECT_THROW(calc.evaluate("5)"), Unmatched_parentheses);
    EXPECT_THROW(calc.evaluate("(5)+5)"), Unmatched_parentheses);
    EXPECT_THROW(calc.evaluate("()"), Syntax_error);
    EXPECT_THROW(calc.evaluate("5("), Syntax_error);
    EXPECT_THROW(calc.evaluate("(5)(5)"), Syntax_error);
}

TEST(ParserTermTest, CorrectTerms)
{
    Parser calc;

    EXPECT_DOUBLE_EQ(calc.evaluate("7*6"), 42);
    EXPECT_DOUBLE_EQ(calc.evaluate("7/6"), 7./6);

    EXPECT_DOUBLE_EQ(calc.evaluate("0.7*0.6"), 0.42);
    EXPECT_DOUBLE_EQ(calc.evaluate("0.7/0.6"), 0.7/0.6);

    EXPECT_DOUBLE_EQ(calc.evaluate("1*3*2*7*1"), 42);
    EXPECT_DOUBLE_EQ(calc.evaluate("1*.3*2*.7*1"), 0.42);
    
    EXPECT_DOUBLE_EQ(calc.evaluate("1/2/3"), 1./2./3.);
    EXPECT_DOUBLE_EQ(calc.evaluate("1./2./3.0"), 1./2./3.);
    
    EXPECT_DOUBLE_EQ(calc.evaluate("5*3/3"), 5);
    EXPECT_DOUBLE_EQ(calc.evaluate("5*3/2"), 1.5*5);
    
    EXPECT_DOUBLE_EQ(calc.evaluate("3*0"), 0);

    EXPECT_DOUBLE_EQ(calc.evaluate("-7*3"), -21);
    EXPECT_DOUBLE_EQ(calc.evaluate("-7*-3"), 21);

    EXPECT_DOUBLE_EQ(calc.evaluate("-10/2"), -5);
    EXPECT_DOUBLE_EQ(calc.evaluate("10/-2"), -5);
    EXPECT_DOUBLE_EQ(calc.evaluate("-10/-2"), 5);

    EXPECT_DOUBLE_EQ(calc.evaluate("--10/-2"), -5);
    EXPECT_DOUBLE_EQ(calc.evaluate("--10/--2"), 5);
}

TEST(ParserTermTest, DivisionByZero)
{
    Parser calc;

    EXPECT_THROW(calc.evaluate("42/0"), Divide_by_zero);
    EXPECT_THROW(calc.evaluate("42/-0"), Divide_by_zero);
    EXPECT_THROW(calc.evaluate("42/+0"), Divide_by_zero);
}

TEST(ParserTermTest, BadTerms)
{
    Parser calc;

    EXPECT_ANY_THROW(calc.evaluate("42//2"));
    EXPECT_ANY_THROW(calc.evaluate("2**2"));
    EXPECT_ANY_THROW(calc.evaluate("*2*4"));
    EXPECT_ANY_THROW(calc.evaluate("/2*3"));
    EXPECT_ANY_THROW(calc.evaluate("3)/2"));
}

TEST(ParserExpressionTest, CorrectExpressions)
{
    Parser calc;

    EXPECT_DOUBLE_EQ(calc.evaluate("5+3.2"), 8.2);
    EXPECT_DOUBLE_EQ(calc.evaluate("5-3.2"), 1.8);

    EXPECT_DOUBLE_EQ(calc.evaluate("5-3.2/2"), 3.4);
    EXPECT_DOUBLE_EQ(calc.evaluate("5-3*2"), -1);
    
    // EXPECT_DOUBLE_EQ(calc.evaluate("5--3.2"), 8.2);
    // EXPECT_DOUBLE_EQ(calc.evaluate("5---3.2"), 1.8);

    EXPECT_DOUBLE_EQ(calc.evaluate("(5-3)*2"), 4);
    EXPECT_DOUBLE_EQ(calc.evaluate("(5-3)/2"), 1);
}

TEST(ParserExpressionTest, BadExpressions)
{
    Parser calc;

    EXPECT_ANY_THROW(calc.evaluate("(5-3*2"));
    EXPECT_ANY_THROW(calc.evaluate("(5-3).2"));
}
