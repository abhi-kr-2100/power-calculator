#ifndef A2100_PCALC_TOKEN
#define A2100_PCALC_TOKEN 1
#pragma once

/**
 * This library provides:
 * - Token UDT to represent a calculator token
 * - Token_type UDT to differentiate between different types of Token
 * - The tokenize() function to get a vector of tokens from a string
 */

#include <istream>
#include <string>
#include <vector>

enum class Token_type
{
    operator_type,
    number,
    identifier
};

struct Token
{
    Token_type kind{};
    char op{};          // in case the token is an operator
    double val{};       // in case the token is a number
    std::string name{}; // in case the token is an identifier
};

std::vector<Token> tokenize(const std::string &expr);

#endif
