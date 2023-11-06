# Power Calculator

Power calculator or `pcalc` is a powerful, feature-heavy console calculator.

## Features

Version 2.0.1 of `pcalc` supports the following features:

* Basic arithmetic alongside exponentiation, float mod, and float factorial
* User-defined variables
* Units - meter, foot, kilogram, pound, second, celsius, fahrenheit, and more

Future releases will implement the following additional features:

* Predefined constants
* Functions

## Installation

`pcalc` is built using `bazel`: <https://bazel.build/>.

Ensure that `bazel` is installed alongside a C++ build compiler. Run the
following command to build `pcalc`:

```sh
bazel build //main:pcalc
```

`bazel` will install the required dependencies and output a binary as
`bazel-bin/main/pcalc`.

You can copy it to another location as per your convenience.

## Usage

If you run `pcalc`, you should be greeted with the following prompt:

```sh
Welcome to Power Calculator!
> _
```

You can begin typing arithmetic expressions. Pressing Enter will display the
result or an error. Here are some examples:

```sh
Welcome to Power Calculator!
> 5 * (1 + 4) / 8
= 3.125
> 2 ^ 10 
= 1024
> 625 ^ 0.5
= 25
> 64 ^ (1 / 3)
= 4
> 16 % 7
= 2
> 4.1 % 0.6
= 0.5
> 3!
= 6
> 4.2!
= 32.5781
> let x = 42
= 42
> x * x
= 1764
> x = 5
= 5
> x ^ 5
= 3125
> 1 meter + 2 foot
= 5.28084 foot
> 42 meter / 1 second + 0 kilometer / 1 hour
= 151.2 kilometer / hour
```

A few things to note:

* You can take square root and cube root of a number using exponentiation.
* `a % b` gives the remainder when `b` is divided by `a`. `a` and `b` can be real numbers.
* You can create variables, and reassign to them.
* Variable assignment can be part of an expression: `(x = 5) * 3` gives `15`.
* When units are involved, the output uses the units of the last operand.
* Units lose their meaning in the context of exponentiation and factorial.
