#include <gtest/gtest.h>
#include <cmath>

#include "parser/parser.hpp"
#include "parser/exceptions.hpp"

using std::pow;
using std::cbrt;
using std::tgamma;

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
    EXPECT_THROW(calc.evaluate("4.2.3"), Syntax_error);
    EXPECT_THROW(calc.evaluate("1e5.2"), Syntax_error);
    EXPECT_THROW(calc.evaluate("1E5.2"), Syntax_error);
    EXPECT_THROW(calc.evaluate("1e+5.2"), Syntax_error);
    EXPECT_THROW(calc.evaluate("1e-5.2"), Syntax_error);
    EXPECT_THROW(calc.evaluate("1E+5.2"), Syntax_error);
    EXPECT_THROW(calc.evaluate("1E-5.2"), Syntax_error);
}

TEST(ParserPrimaryTest, BadParens)
{
    Parser calc;

    EXPECT_THROW(calc.evaluate("(5"), Syntax_error);
    EXPECT_THROW(calc.evaluate("(5)+(5"), Syntax_error);
    EXPECT_THROW(calc.evaluate("5)"), Syntax_error);
    EXPECT_THROW(calc.evaluate("(5)+5)"), Syntax_error);
    EXPECT_THROW(calc.evaluate("()"), Syntax_error);
    EXPECT_THROW(calc.evaluate("5("), Syntax_error);
    EXPECT_THROW(calc.evaluate("(5)(5)"), Syntax_error);
}

TEST(StatementTest, VariableDeclaration)
{
    Parser calc;

    EXPECT_DOUBLE_EQ(calc.evaluate("let pi = 3.14"), 3.14);
    EXPECT_THROW(calc.evaluate("let let = 2"), Syntax_error);
    EXPECT_THROW(calc.evaluate("let"), Syntax_error);
    EXPECT_THROW(calc.evaluate("let x"), Syntax_error);
    EXPECT_THROW(calc.evaluate("let x ="), Syntax_error);
    EXPECT_THROW(calc.evaluate("let x -= 5"), Syntax_error);

    EXPECT_THROW(calc.evaluate("let pi = 2.18"), Runtime_error);
}

TEST(ExpTest, Exponents)
{
    Parser calc;

    EXPECT_DOUBLE_EQ(calc.evaluate("2 ^ 5"), pow(2, 5));
    EXPECT_DOUBLE_EQ(calc.evaluate("2 ^ -5"), pow(2, -5));
    EXPECT_DOUBLE_EQ(calc.evaluate("2.1 ^ 3"), pow(2.1, 3));
    EXPECT_DOUBLE_EQ(calc.evaluate("2.1 ^ 1.1"), pow(2.1, 1.1));
    EXPECT_DOUBLE_EQ(calc.evaluate("4 ^ 0.5"), pow(4, 0.5));

    // ^ binds tighter than ^, so - a ^ b == -(a ^ b)
    EXPECT_DOUBLE_EQ(calc.evaluate("-2 ^ 3"), -pow(2, 3));

    EXPECT_DOUBLE_EQ(calc.evaluate("(-2) ^ 2"), pow(-2, 2));
    EXPECT_DOUBLE_EQ(calc.evaluate("(-2) ^ (-2)"), pow(-2, -2));
    EXPECT_DOUBLE_EQ(calc.evaluate("2 ^ (-2)"), pow(2, -2));

    // complex numbers are not yet supported
    EXPECT_THROW(calc.evaluate("(-5) ^ 0.5"), Runtime_error);
    EXPECT_DOUBLE_EQ(calc.evaluate("(-8) ^ (1/3)"), cbrt(-8));

    EXPECT_DOUBLE_EQ(calc.evaluate("2 ^ 5 ^ 2"), pow(2, pow(5, 2)));

    EXPECT_THROW(calc.evaluate("0 ^ 0"), Runtime_error);
}

TEST(ParserPrimaryTest, VariableEvalutation)
{
    Parser calc;

    calc.evaluate("let x = 4.2");
    EXPECT_DOUBLE_EQ(calc.evaluate("x"), 4.2);
    EXPECT_DOUBLE_EQ(calc.evaluate("(x)+(x+2.)"), 10.4);

    EXPECT_THROW(calc.evaluate("x + y"), Runtime_error);
}

TEST(ParserPrimaryTest, FactorialTest)
{
    Parser calc;

    EXPECT_DOUBLE_EQ(calc.evaluate("20!"), tgamma(20+1));
    EXPECT_DOUBLE_EQ(calc.evaluate("0!"), tgamma(0+1));
    EXPECT_DOUBLE_EQ(calc.evaluate("12.567!"), tgamma(12.567+1));

    // -a! means -(a!)
    EXPECT_DOUBLE_EQ(calc.evaluate("-12!"), -tgamma(12 + 1));
    EXPECT_DOUBLE_EQ(calc.evaluate("(-0)!"), tgamma(-0 + 1));

    EXPECT_THROW(calc.evaluate("(-1)!"), Runtime_error);
    EXPECT_THROW(calc.evaluate("(-42)!"), Runtime_error);
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

    EXPECT_DOUBLE_EQ(calc.evaluate("5 % 3"), 2);
    EXPECT_DOUBLE_EQ(calc.evaluate("5 % 3 % 2"), 0);
    EXPECT_DOUBLE_EQ(calc.evaluate("5.1 % 3.0"), 2.1);
    EXPECT_DOUBLE_EQ(calc.evaluate("-5.1 % 3.0"), -2.1);

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

    EXPECT_THROW(calc.evaluate("42/0"), Runtime_error);
    EXPECT_THROW(calc.evaluate("42/-0"), Runtime_error);
    EXPECT_THROW(calc.evaluate("42/+0"), Runtime_error);

    EXPECT_THROW(calc.evaluate("42 % 0"), Runtime_error);
    EXPECT_THROW(calc.evaluate("42 % +0"), Runtime_error);
    EXPECT_THROW(calc.evaluate("42 % -0"), Runtime_error);
}

TEST(ParserTermTest, BadTerms)
{
    Parser calc;

    EXPECT_THROW(calc.evaluate("42//2"), Syntax_error);
    EXPECT_THROW(calc.evaluate("2**2"), Syntax_error);
    EXPECT_THROW(calc.evaluate("*2*4"), Syntax_error);
    EXPECT_THROW(calc.evaluate("/2*3"), Syntax_error);
    EXPECT_THROW(calc.evaluate("3)/2"), Syntax_error);
}

TEST(ParserExpressionTest, CorrectExpressions)
{
    Parser calc;

    EXPECT_DOUBLE_EQ(calc.evaluate("5+3.2"), 8.2);
    EXPECT_DOUBLE_EQ(calc.evaluate("5-3.2"), 1.8);

    EXPECT_DOUBLE_EQ(calc.evaluate("5-3.2/2"), 3.4);
    EXPECT_DOUBLE_EQ(calc.evaluate("5-3*2"), -1);
    
    EXPECT_DOUBLE_EQ(calc.evaluate("5--3.2"), 8.2);
    EXPECT_DOUBLE_EQ(calc.evaluate("5---3.2"), 1.8);

    EXPECT_DOUBLE_EQ(calc.evaluate("(5-3)*2"), 4);
    EXPECT_DOUBLE_EQ(calc.evaluate("(5-3)/2"), 1);
}

TEST(ParserExpressionTest, BadExpressions)
{
    Parser calc;

    EXPECT_THROW(calc.evaluate(""), Syntax_error);
    EXPECT_THROW(calc.evaluate("(5-3*2"), Syntax_error);
    EXPECT_THROW(calc.evaluate("(5-3).2"), Syntax_error);
    EXPECT_THROW(calc.evaluate("4+"), Syntax_error);
    EXPECT_THROW(calc.evaluate("4-"), Syntax_error);
    EXPECT_THROW(calc.evaluate("4*"), Syntax_error);
    EXPECT_THROW(calc.evaluate("4/"), Syntax_error);
    EXPECT_THROW(calc.evaluate("4("), Syntax_error);
    EXPECT_THROW(calc.evaluate("4)"), Syntax_error);
    EXPECT_THROW(calc.evaluate("5**"), Syntax_error);
    EXPECT_THROW(calc.evaluate("5--"), Syntax_error);
    EXPECT_THROW(calc.evaluate("5++"), Syntax_error);
    EXPECT_THROW(calc.evaluate("5//"), Syntax_error);
    EXPECT_THROW(calc.evaluate("5))"), Syntax_error);
    EXPECT_THROW(calc.evaluate("5(("), Syntax_error);
    EXPECT_THROW(calc.evaluate("5***"), Syntax_error);
    EXPECT_THROW(calc.evaluate("5---"), Syntax_error);
    EXPECT_THROW(calc.evaluate("5+++"), Syntax_error);
    EXPECT_THROW(calc.evaluate("5///"), Syntax_error);
    EXPECT_THROW(calc.evaluate("5((()))"), Syntax_error);
    EXPECT_THROW(calc.evaluate("5)))"), Syntax_error);
    EXPECT_THROW(calc.evaluate("5((("), Syntax_error);
}
