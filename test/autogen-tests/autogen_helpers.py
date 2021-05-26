"""
Helper functions to generate random test cases for Power Calculator.
"""

from random import choice, randint

def generate_primary():
    """Return a randomly generated primary."""

    if choice(range(10)) == 0:
        return "(" + generate_expression() + ")"

    sign = choice("+- ")
    
    int_part = str(randint(0, 10))
    frac_part = str(randint(0, 999))

    expo = ''
    if choice([0, 1]) == 0:
        expo = choice(['e', 'E', 'e+', 'e-', 'E+', 'E-'])
        expo += str(randint(0, 10))

    return (sign + int_part + '.' + frac_part + expo).strip()

def generate_term():
    """Return a randomly generated term."""

    primaries = [generate_primary()]
    while choice("012") != '0':
        primaries.append(generate_primary())

    ops = []
    # generate len(primaries) - 1 number of operators
    # because n primaries have (n - 1) operators between them
    for _ in range(len(primaries) - 1):
        ops.append(choice("*/"))

    term = primaries[0]
    for i in range(len(ops)):
        term += ops[i]
        term += primaries[i + 1]

    return term

def generate_expression():
    """Return a randomly generated expression."""

    terms = [generate_term()]
    while choice("012") != '0':
        terms.append(generate_term())

    ops = []
    # generate len(terms) - 1 number of operators
    # because n terms have (n - 1) operators between them
    for _ in range(len(terms) - 1):
        ops.append(choice("+-"))

    expr = terms[0]
    for i in range(len(ops)):
        expr += ops[i]
        expr += terms[i + 1]

    return expr

def generate_tests(n=10000, test_file='autogentest.tc'):
    """Generate random test cases and write them to a file."""

    with open(test_file, 'w') as tf:
        for _ in range(n):
            expr = generate_expression()
            print(expr, file=tf)

def generate_expects(test_file='autogentest.tc', expect_file='pcalc.expect'):
    """Write expected results to a file."""

    tests = open(test_file).readlines()
    with open(expect_file, 'w') as ef:
        print("Welcome to Power Calculator!", file=ef)
        for t in tests:
            try:
                result = eval(t)
                print("> = {}".format(eval(t)), file=ef)
            except ZeroDivisionError:
                print("> ! Division by 0.", file=ef)
        print(">", file=ef)
        print("Bye!", file=ef)

def generate_required_files(test_file='autogentest.tc', expect_file='pcalc.expect'):
    generate_tests()
    generate_expects()

def check(expect_file='pcalc.expect', results_file='pcalc.out'):
    expects = open(expect_file).readlines()
    results = open(results_file).readlines()

    if len(expects) != len(results):
        print("Lengths don't match!")
        return

    for e, r in zip(expects[1:-3], results[1:-3]):
        if e == r:
            continue
        if abs(float(e[3:]) / float(r[3:]) - 1) > 1e-5:
            print("Mismatch: {} != {}".format(e, r))
