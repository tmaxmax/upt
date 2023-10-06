def valori_distincte(a, b, c):
    if a == b == c:
        print("toate argumentele sunt egale")
        return 0
    
    if a != b != c:
        print("toate argumentele sunt distincte")
        return 3
    
    print_eq = lambda n_arg1, n_arg2: print(f"argumentele {n_arg1} si {n_arg2} sunt egale")

    if a == b:
        print_eq(1, 2)
    elif b == c:
        print_eq(2, 3)
    else:
        print_eq(1, 3)
    
    return 1


def mediana(a, b, c):
    first = min(a, b, c)
    last = max(a, b, c)
    return a + b + c - first - last


def combine_funcs(f, g, op):
    return lambda x: op(f(x), g(x))


sum_funcs = lambda f, g: combine_funcs(f, g, lambda a, b: a + b)

add_15 = lambda x: x + 15