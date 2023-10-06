def valori_distincte(a, b, c):
    if a == b == c:
        print("toate argumentele sunt egale")
        return 0
    
    if a != b != c != a:
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

def valori_distincte_alt(a, b, c):
    locs = list(locals().values())
    vals = {x: [] for x in locs}
    [vals[x].append(i + 1) for (i, x) in enumerate(locs)]
    by_len = sorted(vals.values(), key=len)
    out, ret = [
        (lambda: "toate valorile sunt distincte", 3),
        (lambda: f"argumentele {' si '.join(map(str, by_len[1]))} sunt egale", 1),
        (lambda: "toate valorile sunt egale", 0)
    ][len(by_len)-1]
    print(out())
    return ret

def valori_distincte_generic(*args):
    vals = {x: [] for x in args}
    [vals[x].append(i + 1) for (i, x) in enumerate(args)]
    by_len = sorted(vals.values(), key=len)
    print([
        "argumentele %s sunt egale" % "; ".join(
            map(lambda l: ", ".join(map(str, l)), filter(lambda x: len(x) > 1, by_len))
        ),
        "toate valorile sunt distincte",
        "toate valorile sunt egale",
    ][int(len(by_len) == len(args) or len(by_len) == 1) + int(len(by_len) == 1)])
    return sum(int(len(x) == 1) for x in by_len)


print(valori_distincte(1, 2, 3))
print(valori_distincte(2, 2, 3))
print(valori_distincte(3, 2, 3))
print(valori_distincte(2, 3, 3))
print(valori_distincte(2, 2, 2))
print()
print(valori_distincte_alt(1, 2, 3))
print(valori_distincte_alt(2, 2, 3))
print(valori_distincte_alt(3, 2, 3))
print(valori_distincte_alt(2, 3, 3))
print(valori_distincte_alt(2, 2, 2))
print()
print(valori_distincte_generic(2, 3, 2, 3, 3, 5, 10, 17, 17))
print(valori_distincte_generic(2, 2, 2, 2, 2, 2))
print(valori_distincte_generic(2, 4, 5, 7, 9, 1))


def mediana(a, b, c):
    first = min(a, b, c)
    last = max(a, b, c)
    return a + b + c - first - last


def combine_funcs(f, g, op):
    return lambda x: op(f(x), g(x))


sum_funcs = lambda f, g: combine_funcs(f, g, lambda a, b: a + b)

add_15 = lambda x: x + 15