from operator import mul
from math import sqrt

def progresie_aritmetica(n):
    return 2 if n <= 0 else (2 * progresie_aritmetica(n - 1) - 3)

print(f'Progresie aritmetica (n=5): {progresie_aritmetica(5)}')

def fibonacci(n):
    match n:
        case 0: return 0
        case 1: return 1
        case _ if n > 0: return fibonacci(n - 1) + fibonacci(n - 2)
        case _: raise ValueError("Rangul nu poate fi negativ")

print(f'Fibonacci (3): {fibonacci(3)}')

def suma_consecutive(n):
    return 0 if n <= 1 else n - 1 + suma_consecutive(n - 1)

print(f'Suma consecutive (5): {suma_consecutive(5)}')

def reduce_digits(num, acc, init, acc_called=False):
    match num:
        case 0: return init if acc_called else acc(num, init)
        case _ if num > 0: return reduce_digits(num // 10, acc, acc(init, num % 10), True)
        case _: raise ValueError("Numarul nu poate fi negativ")

digits_product = lambda num: reduce_digits(num, mul, 1)
print(f'Produs cifre (123): {digits_product(123)}')
digits_count = lambda num: reduce_digits(num, lambda acc, _: acc + 1, 0)
print(f'Numar cifre (1234): {digits_count(1234)}')
digits_max = lambda num: reduce_digits(num, max, 0)
print(f'Cifra maxima (14096): {digits_max(14096)}')
digits_count_even = lambda num: reduce_digits(num, lambda acc, d: acc + int(d % 2 == 0), 0)
print(f'Numar cifre pare (134789): {digits_count_even(134789)}')

def exponential(a, n):
    return 1 if n <= 0 else a * exponential(a, n - 1)

print(f'5^3 = {exponential(5, 3)}')

def este_numar_prim(n, d=2):
    return True if d == n else (False if n % d == 0 else este_numar_prim(n, d + 1))

print(f'5 este prim? {este_numar_prim(5)}')

def cmmdc(a, b):
    return a if b == 0 else cmmdc(b, a % b)

print(f'cmmdc(100, 35) = {cmmdc(100, 35)}')

def my_reverse(s):
    return '' if s == '' else s[-1] + my_reverse(s[0:len(s)-1])

print(f'my_reverse(mama) = {my_reverse("mama")}')

def print_nats_in_interval(min_value, max_value):
    if min_value > max_value:
        return

    print(min_value)
    print_nats_in_interval(min_value + 1, max_value)

print(f'Numere naturale de la 5 la 10:')
print_nats_in_interval(5, 10)

count_digit_occurrence = lambda num, digit: reduce_digits(num, lambda acc, d: acc + int(d == digit), 0)
print(f'Numar aparitii cifra 5 in 1234545: {count_digit_occurrence(1234545, 5)}')
has_digit = lambda num, digit: count_digit_occurrence(num, digit) > 0
print(f'Numarul 73 are cifra 3? {has_digit(73, 3)}')

def este_palindrom(num):
    def impl(num, rev, i):
        return num == rev if i == 0 else impl(num, rev * 10 + i % 10, i // 10)
    
    return impl(num, 0, num)

print(f'32123 este palindrom? {este_palindrom(32123)}')

def apply_n(f, n, x):
    return x if n == 0 else apply_n(f, n - 1, f(x))

print(f'5 inmultit cu 2 de 5 ori: {apply_n(lambda x: x * 2, 5, 5)}')

def compute_series(n, term_acc):
    return 0 if n < 0 else term_acc(n, compute_series(n - 1, term_acc))

compute_fraction_series = lambda n: compute_series(n, lambda n, follows: (1 / (n + 1)) + follows)
print(f'Valoare suma partiala fractii (n=5): {compute_fraction_series(5)}')

def taylor_e_term(x, n, memo):
    if n == 0:
        return 1

    computed = memo[n]
    if computed is not None:
        return computed
    
    val = taylor_e_term(x, n - 1, memo) * x / n
    memo[n] = val

    return val

def compute_exp(x, n):
    memo = [None] * (n + 1)
    return compute_series(n, lambda n, follows: taylor_e_term(x, n, memo) + follows)

print(f'Valoare e^5 calculat cu serie Taylor (n = 20): {compute_exp(5, 20)}')

compute_sqrt_series = lambda n: compute_series(n, lambda _, follows: sqrt(1 + follows))
print(f'Valoare suma partiala serie radicali (n=5): {compute_sqrt_series(5)}')

def decimal_to_binary(n, first_call=True):
    return ('0' if first_call else '') if n == 0 else decimal_to_binary(n // 2, False) + str(n % 2)

print(f'Reprezentare binara a 21: {decimal_to_binary(21)}')

def print_triunghi(n):
    if n == 0:
        return
    
    print_triunghi(n - 1)
    print((str(n) + ' ') * n)

print(f'Triunghi din numere (n=5):')
print_triunghi(5)

def modulo_p_remainder(a, p, n=1):
    return 0 if a % p == 0 else n if pow(a, n, p) == 1 else modulo_p_remainder(a, p, n + 1)

print(f'Pentru ce exponent 4 la putere mod 7 este 1? {modulo_p_remainder(4, 7)}')