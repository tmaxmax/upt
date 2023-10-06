from math import pi, ceil

def interval(a, b, c):
    return a <= c <= b


def sort(a, b, c):
    first = min(a, b, c)
    last = max(a, b, c)
    middle = a + b + c - first - last

    return first, middle, last


def timp_in_secunde(ts):
    h = int(ts[0:2])
    m = int(ts[5:7])
    s = int(ts[10:12])

    return h * 60 * 60 + m * 60 + s


def diferenta_timp(a, b):
    return abs(timp_in_secunde(a) - timp_in_secunde(b))


def arie_lungime_cerc(r):
    return 2 * pi * r, pi * r * r


# Calculează diferența dintre doi ani calendaristici,
# de la 1 ianuarie al unui an până la 1 ianuarie
# al celuilalt an, [1 ian a, 1 ian b). Ordinea
# calendaristică a parametrilor nu este relevantă.
def numar_zile_dintre_ani(a, b):
    if a == b:
        return 0

    if a > b:
        a, b = b, a
    
    dif = b - a
    ani_bisecti = 0

    mod_4_candidate = a if a % 4 == 0 else (a + (4 - a % 4))
    mod_100_candidate = a if a % 100 == 0 else (a + (100 - a % 100))
    mod_400_candidate = a if a % 400 == 0 else (a + (400 - a % 400))

    if mod_4_candidate < b:
        ani_bisecti += ceil((b - mod_4_candidate) / 4)
    
    if mod_100_candidate < b:
        ani_bisecti -= ceil((b - mod_100_candidate) / 100)
    
    if mod_400_candidate < b:
        ani_bisecti += ceil((b - mod_400_candidate) / 400)

    return dif * 365 + ani_bisecti


if __name__ == '__main__':
    print(f'{numar_zile_dintre_ani(2023, 2025)} = 731')
    print(f'{numar_zile_dintre_ani(2023, 2050)} = 9862')
    print(f'{numar_zile_dintre_ani(2021, 2401)} = 138792')
    print(f'{numar_zile_dintre_ani(3, 3000)} = 1094632')