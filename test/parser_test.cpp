#include <gtest/gtest.h>
#include <cmath>
#include <map>
#include <string>

#include "parser/parser.hpp"
#include "parser/exceptions.hpp"
#include "primary/primary.hpp"
#include "primary/exceptions.hpp"

using std::cbrt;
using std::map;
using std::pow;
using std::string;
using std::tgamma;

TEST(ParserPrimaryTest, Numbers)
{
    Parser calc;

    EXPECT_DOUBLE_EQ(calc.evaluate("12").get_value(), 12);
    EXPECT_DOUBLE_EQ(calc.evaluate("-12").get_value(), -12);
    EXPECT_DOUBLE_EQ(calc.evaluate("+12").get_value(), 12);

    EXPECT_DOUBLE_EQ(calc.evaluate(".69").get_value(), .69);
    EXPECT_DOUBLE_EQ(calc.evaluate("+.69").get_value(), .69);
    EXPECT_DOUBLE_EQ(calc.evaluate("-.69").get_value(), -.69);

    EXPECT_DOUBLE_EQ(calc.evaluate("1e3").get_value(), 1e3);
    EXPECT_DOUBLE_EQ(calc.evaluate("1E3").get_value(), 1E3);
    EXPECT_DOUBLE_EQ(calc.evaluate("1e+3").get_value(), 1e3);
    EXPECT_DOUBLE_EQ(calc.evaluate("1e-3").get_value(), 1e-3);
    EXPECT_DOUBLE_EQ(calc.evaluate("1E+3").get_value(), 1e3);
    EXPECT_DOUBLE_EQ(calc.evaluate("1E-3").get_value(), 1e-3);

    EXPECT_DOUBLE_EQ(calc.evaluate("+1e3").get_value(), 1e3);
    EXPECT_DOUBLE_EQ(calc.evaluate("+1E3").get_value(), 1E3);
    EXPECT_DOUBLE_EQ(calc.evaluate("+1e+3").get_value(), 1e3);
    EXPECT_DOUBLE_EQ(calc.evaluate("+1e-3").get_value(), 1e-3);
    EXPECT_DOUBLE_EQ(calc.evaluate("+1E+3").get_value(), 1e3);
    EXPECT_DOUBLE_EQ(calc.evaluate("+1E-3").get_value(), 1e-3);

    EXPECT_DOUBLE_EQ(calc.evaluate("-1e3").get_value(), -1e3);
    EXPECT_DOUBLE_EQ(calc.evaluate("-1E3").get_value(), -1E3);
    EXPECT_DOUBLE_EQ(calc.evaluate("-1e+3").get_value(), -1e3);
    EXPECT_DOUBLE_EQ(calc.evaluate("-1e-3").get_value(), -1e-3);
    EXPECT_DOUBLE_EQ(calc.evaluate("-1E+3").get_value(), -1e3);
    EXPECT_DOUBLE_EQ(calc.evaluate("-1E-3").get_value(), -1e-3);
}

TEST(ParserPrimaryTest, Expressions)
{
    Parser calc;

    EXPECT_DOUBLE_EQ(calc.evaluate("(12)").get_value(), 12);
    EXPECT_DOUBLE_EQ(calc.evaluate("(-12)").get_value(), -12);
    EXPECT_DOUBLE_EQ(calc.evaluate("(+12)").get_value(), 12);

    EXPECT_DOUBLE_EQ(calc.evaluate("(.69)").get_value(), .69);
    EXPECT_DOUBLE_EQ(calc.evaluate("(+.69)").get_value(), .69);
    EXPECT_DOUBLE_EQ(calc.evaluate("(-.69)").get_value(), -.69);

    EXPECT_DOUBLE_EQ(calc.evaluate("(1e3)").get_value(), 1e3);
    EXPECT_DOUBLE_EQ(calc.evaluate("(1E3)").get_value(), 1E3);
    EXPECT_DOUBLE_EQ(calc.evaluate("(1e+3)").get_value(), 1e3);
    EXPECT_DOUBLE_EQ(calc.evaluate("(1e-3)").get_value(), 1e-3);
    EXPECT_DOUBLE_EQ(calc.evaluate("(1E+3)").get_value(), 1e3);
    EXPECT_DOUBLE_EQ(calc.evaluate("(1E-3)").get_value(), 1e-3);

    EXPECT_DOUBLE_EQ(calc.evaluate("(+1e3)").get_value(), 1e3);
    EXPECT_DOUBLE_EQ(calc.evaluate("(+1E3)").get_value(), 1E3);
    EXPECT_DOUBLE_EQ(calc.evaluate("(+1e+3)").get_value(), 1e3);
    EXPECT_DOUBLE_EQ(calc.evaluate("(+1e-3)").get_value(), 1e-3);
    EXPECT_DOUBLE_EQ(calc.evaluate("(+1E+3)").get_value(), 1e3);
    EXPECT_DOUBLE_EQ(calc.evaluate("(+1E-3)").get_value(), 1e-3);

    EXPECT_DOUBLE_EQ(calc.evaluate("(-1e3)").get_value(), -1e3);
    EXPECT_DOUBLE_EQ(calc.evaluate("(-1E3)").get_value(), -1E3);
    EXPECT_DOUBLE_EQ(calc.evaluate("(-1e+3)").get_value(), -1e3);
    EXPECT_DOUBLE_EQ(calc.evaluate("(-1e-3)").get_value(), -1e-3);
    EXPECT_DOUBLE_EQ(calc.evaluate("(-1E+3)").get_value(), -1e3);
    EXPECT_DOUBLE_EQ(calc.evaluate("(-1E-3)").get_value(), -1e-3);
}

TEST(ParserPrimaryTest, BadPrimary)
{
    Parser calc;

    EXPECT_THROW(calc.evaluate("12+").get_value(), Syntax_error);
    EXPECT_THROW(calc.evaluate("4.2.3").get_value(), Syntax_error);
    EXPECT_THROW(calc.evaluate("1e5.2").get_value(), Syntax_error);
    EXPECT_THROW(calc.evaluate("1E5.2").get_value(), Syntax_error);
    EXPECT_THROW(calc.evaluate("1e+5.2").get_value(), Syntax_error);
    EXPECT_THROW(calc.evaluate("1e-5.2").get_value(), Syntax_error);
    EXPECT_THROW(calc.evaluate("1E+5.2").get_value(), Syntax_error);
    EXPECT_THROW(calc.evaluate("1E-5.2").get_value(), Syntax_error);
}

TEST(ParserPrimaryTest, BadParens)
{
    Parser calc;

    EXPECT_THROW(calc.evaluate("(5").get_value(), Syntax_error);
    EXPECT_THROW(calc.evaluate("(5)+(5").get_value(), Syntax_error);
    EXPECT_THROW(calc.evaluate("5)").get_value(), Syntax_error);
    EXPECT_THROW(calc.evaluate("(5)+5)").get_value(), Syntax_error);
    EXPECT_THROW(calc.evaluate("()").get_value(), Syntax_error);
    EXPECT_THROW(calc.evaluate("5(").get_value(), Syntax_error);
    EXPECT_THROW(calc.evaluate("(5)(5)").get_value(), Syntax_error);
}

TEST(StatementTest, VariableDeclaration)
{
    Parser calc;

    EXPECT_DOUBLE_EQ(calc.evaluate("let pi = 3.14").get_value(), 3.14);
    EXPECT_THROW(calc.evaluate("let let = 2").get_value(), Syntax_error);
    EXPECT_THROW(calc.evaluate("let").get_value(), Syntax_error);
    EXPECT_THROW(calc.evaluate("let x").get_value(), Syntax_error);
    EXPECT_THROW(calc.evaluate("let x =").get_value(), Syntax_error);
    EXPECT_THROW(calc.evaluate("let x -= 5").get_value(), Syntax_error);

    EXPECT_THROW(calc.evaluate("let pi = 2.18").get_value(), Runtime_error);
}

TEST(StatementTest, ExternalVariableDeclaration)
{
    Parser calc;
    map<string, Primary> vtab;

    EXPECT_DOUBLE_EQ(calc.evaluate("let pi = 3.14", vtab).get_value(), 3.14);
    EXPECT_THROW(calc.evaluate("let let = 2", vtab), Syntax_error);
    EXPECT_THROW(calc.evaluate("let", vtab), Syntax_error);
    EXPECT_THROW(calc.evaluate("let x", vtab), Syntax_error);
    EXPECT_THROW(calc.evaluate("let x =", vtab), Syntax_error);
    EXPECT_THROW(calc.evaluate("let x -= 5", vtab), Syntax_error);

    EXPECT_THROW(calc.evaluate("let pi = 2.18", vtab), Runtime_error);
}

TEST(ExpTest, Exponents)
{
    Parser calc;

    EXPECT_DOUBLE_EQ(calc.evaluate("2 ^ 5").get_value(), pow(2, 5));
    EXPECT_DOUBLE_EQ(calc.evaluate("2 ^ -5").get_value(), pow(2, -5));
    EXPECT_DOUBLE_EQ(calc.evaluate("2.1 ^ 3").get_value(), pow(2.1, 3));
    EXPECT_DOUBLE_EQ(calc.evaluate("2.1 ^ 1.1").get_value(), pow(2.1, 1.1));
    EXPECT_DOUBLE_EQ(calc.evaluate("4 ^ 0.5").get_value(), pow(4, 0.5));

    EXPECT_DOUBLE_EQ(calc.evaluate("(2^2)").get_value(), pow(2, 2));

    // ^ binds tighter than ^, so - a ^ b == -(a ^ b)
    EXPECT_DOUBLE_EQ(calc.evaluate("-2 ^ 3").get_value(), -pow(2, 3));

    EXPECT_DOUBLE_EQ(calc.evaluate("(-2) ^ 2").get_value(), pow(-2, 2));
    EXPECT_DOUBLE_EQ(calc.evaluate("(-2) ^ (-2)").get_value(), pow(-2, -2));
    EXPECT_DOUBLE_EQ(calc.evaluate("2 ^ (-2)").get_value(), pow(2, -2));

    // complex numbers are not yet supported
    EXPECT_THROW(calc.evaluate("(-5) ^ 0.5").get_value(), Invalid_operands);
    EXPECT_DOUBLE_EQ(calc.evaluate("(-8) ^ (1/3)").get_value(), cbrt(-8));

    EXPECT_DOUBLE_EQ(calc.evaluate("2 ^ 5 ^ 2").get_value(), pow(2, pow(5, 2)));

    EXPECT_THROW(calc.evaluate("0 ^ 0").get_value(), Invalid_operands);
}

TEST(ParserPrimaryTest, VariableEvalutation)
{
    Parser calc;

    calc.evaluate("let x = 4.2");
    EXPECT_DOUBLE_EQ(calc.evaluate("x").get_value(), 4.2);
    EXPECT_DOUBLE_EQ(calc.evaluate("(x)+(x+2.)").get_value(), 10.4);

    EXPECT_THROW(calc.evaluate("x + y").get_value(), Runtime_error);
}

TEST(ParserPrimaryTest, ExternalVariableEvalutation)
{
    Parser calc;
    map<string, Primary> vtab;

    calc.evaluate("let x = 4.2", vtab);
    EXPECT_DOUBLE_EQ(calc.evaluate("x", vtab).get_value(), 4.2);
    EXPECT_DOUBLE_EQ(calc.evaluate("(x)+(x+2.)", vtab).get_value(), 10.4);

    EXPECT_THROW(calc.evaluate("x + y", vtab), Runtime_error);
}

TEST(ParserPrimaryTest, FactorialTest)
{
    Parser calc;

    EXPECT_DOUBLE_EQ(calc.evaluate("20!").get_value(), tgamma(20 + 1));
    EXPECT_DOUBLE_EQ(calc.evaluate("0!").get_value(), tgamma(0 + 1));
    EXPECT_DOUBLE_EQ(calc.evaluate("12.567!").get_value(), tgamma(12.567 + 1));

    // -a! means -(a!)
    EXPECT_DOUBLE_EQ(calc.evaluate("-12!").get_value(), -tgamma(12 + 1));
    EXPECT_DOUBLE_EQ(calc.evaluate("(-0)!").get_value(), tgamma(-0 + 1));

    EXPECT_DOUBLE_EQ(calc.evaluate("5! / 2!").get_value(), 3 * 4 * 5);
    EXPECT_DOUBLE_EQ(calc.evaluate("1! + 2!").get_value(), 3);

    EXPECT_THROW(calc.evaluate("(-1)!").get_value(), Invalid_operands);
    EXPECT_THROW(calc.evaluate("(-42)!").get_value(), Invalid_operands);

    EXPECT_THROW(calc.evaluate("!").get_value(), Syntax_error);
}

TEST(ParserTermTest, CorrectTerms)
{
    Parser calc;

    EXPECT_DOUBLE_EQ(calc.evaluate("7*6").get_value(), 42);
    EXPECT_DOUBLE_EQ(calc.evaluate("7/6").get_value(), 7. / 6);

    EXPECT_DOUBLE_EQ(calc.evaluate("0.7*0.6").get_value(), 0.42);
    EXPECT_DOUBLE_EQ(calc.evaluate("0.7/0.6").get_value(), 0.7 / 0.6);

    EXPECT_DOUBLE_EQ(calc.evaluate("1*3*2*7*1").get_value(), 42);
    EXPECT_DOUBLE_EQ(calc.evaluate("1*.3*2*.7*1").get_value(), 0.42);

    EXPECT_DOUBLE_EQ(calc.evaluate("1/2/3").get_value(), 1. / 2. / 3.);
    EXPECT_DOUBLE_EQ(calc.evaluate("1./2./3.0").get_value(), 1. / 2. / 3.);

    EXPECT_DOUBLE_EQ(calc.evaluate("5*3/3").get_value(), 5);
    EXPECT_DOUBLE_EQ(calc.evaluate("5*3/2").get_value(), 1.5 * 5);

    EXPECT_DOUBLE_EQ(calc.evaluate("3*0").get_value(), 0);

    EXPECT_DOUBLE_EQ(calc.evaluate("5 % 3").get_value(), 2);
    EXPECT_DOUBLE_EQ(calc.evaluate("5 % 3 % 2").get_value(), 0);
    EXPECT_DOUBLE_EQ(calc.evaluate("5.1 % 3.0").get_value(), 2.1);
    EXPECT_DOUBLE_EQ(calc.evaluate("-5.1 % 3.0").get_value(), -2.1);

    EXPECT_DOUBLE_EQ(calc.evaluate("-7*3").get_value(), -21);
    EXPECT_DOUBLE_EQ(calc.evaluate("-7*-3").get_value(), 21);

    EXPECT_DOUBLE_EQ(calc.evaluate("-10/2").get_value(), -5);
    EXPECT_DOUBLE_EQ(calc.evaluate("10/-2").get_value(), -5);
    EXPECT_DOUBLE_EQ(calc.evaluate("-10/-2").get_value(), 5);

    EXPECT_DOUBLE_EQ(calc.evaluate("--10/-2").get_value(), -5);
    EXPECT_DOUBLE_EQ(calc.evaluate("--10/--2").get_value(), 5);
}

TEST(ParserTermTest, DivisionByZero)
{
    Parser calc;

    EXPECT_THROW(calc.evaluate("42/0").get_value(), Division_by_zero);
    EXPECT_THROW(calc.evaluate("42/-0").get_value(), Division_by_zero);
    EXPECT_THROW(calc.evaluate("42/+0").get_value(), Division_by_zero);

    EXPECT_THROW(calc.evaluate("42 % 0").get_value(), Division_by_zero);
    EXPECT_THROW(calc.evaluate("42 % +0").get_value(), Division_by_zero);
    EXPECT_THROW(calc.evaluate("42 % -0").get_value(), Division_by_zero);
}

TEST(ParserTermTest, BadTerms)
{
    Parser calc;

    EXPECT_THROW(calc.evaluate("42//2").get_value(), Syntax_error);
    EXPECT_THROW(calc.evaluate("2**2").get_value(), Syntax_error);
    EXPECT_THROW(calc.evaluate("*2*4").get_value(), Syntax_error);
    EXPECT_THROW(calc.evaluate("/2*3").get_value(), Syntax_error);
    EXPECT_THROW(calc.evaluate("3)/2").get_value(), Syntax_error);
}

TEST(ParserExpressionTest, CorrectExpressions)
{
    Parser calc;

    EXPECT_DOUBLE_EQ(calc.evaluate("5+3.2").get_value(), 8.2);
    EXPECT_DOUBLE_EQ(calc.evaluate("5-3.2").get_value(), 1.8);

    EXPECT_DOUBLE_EQ(calc.evaluate("5-3.2/2").get_value(), 3.4);
    EXPECT_DOUBLE_EQ(calc.evaluate("5-3*2").get_value(), -1);

    EXPECT_DOUBLE_EQ(calc.evaluate("5--3.2").get_value(), 8.2);
    EXPECT_DOUBLE_EQ(calc.evaluate("5---3.2").get_value(), 1.8);

    EXPECT_DOUBLE_EQ(calc.evaluate("(5-3)*2").get_value(), 4);
    EXPECT_DOUBLE_EQ(calc.evaluate("(5-3)/2").get_value(), 1);
}

TEST(ParserExpressionTest, BadExpressions)
{
    Parser calc;

    EXPECT_THROW(calc.evaluate("").get_value(), Syntax_error);
    EXPECT_THROW(calc.evaluate("(5-3*2").get_value(), Syntax_error);
    EXPECT_THROW(calc.evaluate("(5-3).2").get_value(), Syntax_error);
    EXPECT_THROW(calc.evaluate("4+").get_value(), Syntax_error);
    EXPECT_THROW(calc.evaluate("4-").get_value(), Syntax_error);
    EXPECT_THROW(calc.evaluate("4*").get_value(), Syntax_error);
    EXPECT_THROW(calc.evaluate("4/").get_value(), Syntax_error);
    EXPECT_THROW(calc.evaluate("4(").get_value(), Syntax_error);
    EXPECT_THROW(calc.evaluate("4)").get_value(), Syntax_error);
    EXPECT_THROW(calc.evaluate("5**").get_value(), Syntax_error);
    EXPECT_THROW(calc.evaluate("5--").get_value(), Syntax_error);
    EXPECT_THROW(calc.evaluate("5++").get_value(), Syntax_error);
    EXPECT_THROW(calc.evaluate("5//").get_value(), Syntax_error);
    EXPECT_THROW(calc.evaluate("5))").get_value(), Syntax_error);
    EXPECT_THROW(calc.evaluate("5((").get_value(), Syntax_error);
    EXPECT_THROW(calc.evaluate("5***").get_value(), Syntax_error);
    EXPECT_THROW(calc.evaluate("5---").get_value(), Syntax_error);
    EXPECT_THROW(calc.evaluate("5+++").get_value(), Syntax_error);
    EXPECT_THROW(calc.evaluate("5///").get_value(), Syntax_error);
    EXPECT_THROW(calc.evaluate("5((()))").get_value(), Syntax_error);
    EXPECT_THROW(calc.evaluate("5)))").get_value(), Syntax_error);
    EXPECT_THROW(calc.evaluate("5(((").get_value(), Syntax_error);
}

TEST(ParserAssignmentTest, Assignment)
{
    Parser calc;
    calc.evaluate("let x = 42");
    calc.evaluate("let y = 28");
    calc.evaluate("let z = 0");

    EXPECT_DOUBLE_EQ(calc.evaluate("x = 28").get_value(), 28.0);
    EXPECT_DOUBLE_EQ(calc.evaluate("x = x + 2").get_value(), 30.0);
    EXPECT_DOUBLE_EQ(calc.evaluate("x").get_value(), 30.0);
    EXPECT_DOUBLE_EQ(calc.evaluate("5 + (z = 1)").get_value(), 6.0);
    EXPECT_DOUBLE_EQ(calc.evaluate("x = y = z = 42").get_value(), 42.0);
    EXPECT_DOUBLE_EQ(calc.evaluate("x").get_value(), 42.0);
    EXPECT_DOUBLE_EQ(calc.evaluate("y").get_value(), 42.0);
    EXPECT_DOUBLE_EQ(calc.evaluate("z").get_value(), 42.0);
}

TEST(ParserAssignmentTest, ExternalVariableAssignment)
{
    Parser calc;
    map<string, Primary> vtab;

    calc.evaluate("let x = 42", vtab);
    calc.evaluate("let y = 28", vtab);
    calc.evaluate("let z = 0", vtab);

    EXPECT_DOUBLE_EQ(calc.evaluate("x = 28", vtab).get_value(), 28.0);
    EXPECT_DOUBLE_EQ(calc.evaluate("x = x + 2", vtab).get_value(), 30.0);
    EXPECT_DOUBLE_EQ(calc.evaluate("x", vtab).get_value(), 30.0);
    EXPECT_DOUBLE_EQ(calc.evaluate("5 + (z = 1)", vtab).get_value(), 6.0);
    EXPECT_DOUBLE_EQ(calc.evaluate("x = y = z = 42", vtab).get_value(), 42.0);
    EXPECT_DOUBLE_EQ(calc.evaluate("x", vtab).get_value(), 42.0);
    EXPECT_DOUBLE_EQ(calc.evaluate("y", vtab).get_value(), 42.0);
    EXPECT_DOUBLE_EQ(calc.evaluate("z", vtab).get_value(), 42.0);
}

TEST(ParserAssignmentTest, BadAssignments)
{
    Parser calc;

    EXPECT_THROW(calc.evaluate("=28").get_value(), Syntax_error);
    EXPECT_THROW(calc.evaluate("x=42").get_value(), Runtime_error);
    EXPECT_THROW(calc.evaluate("x=").get_value(), Syntax_error);
    EXPECT_THROW(calc.evaluate("x 5 = 12").get_value(), Syntax_error);
    EXPECT_THROW(calc.evaluate("5 x = 42").get_value(), Syntax_error);
}

TEST(ParserAssignmentTest, ExternalVariableBadAssignments)
{
    Parser calc;
    map<string, Primary> vtab;

    EXPECT_THROW(calc.evaluate("=28", vtab), Syntax_error);
    EXPECT_THROW(calc.evaluate("x=42", vtab), Runtime_error);
    EXPECT_THROW(calc.evaluate("x=", vtab), Syntax_error);
    EXPECT_THROW(calc.evaluate("x 5 = 12", vtab), Syntax_error);
    EXPECT_THROW(calc.evaluate("5 x = 42", vtab), Syntax_error);
}
