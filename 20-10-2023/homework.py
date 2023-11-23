from functools import reduce
from operator import mul

def my_filter(rev):
    def impl(l, pred):
        match l:
            case []: return []
            case [e, *rest]:
                other = impl(rest, pred)
                curr = [e] if pred(e) else []
                return other + curr if rev else curr + other
    return impl

filter_normal = my_filter(False)
filter_reverse = my_filter(True)

def este_numar_prim(n, d=2):
    return True if d >= n else (False if n % d == 0 else este_numar_prim(n, d + 1))

print((filter_normal([1, 2, 3, 4], lambda n: n % 2 == 0)))
print((filter_normal(["sar", "ma", "le", "le mele"], lambda s: len(s) < 3)))
print((sorted([10, 23, 95, 74], key=lambda n: n % 10)))
print(reduce(mul, [1, 2, 3, 4]))
print(([*map(lambda n: n ** 3, [1, 2, 3, 4])]))
print(([*filter(lambda v: este_numar_prim(v), [1, 2, 3, 4, 5, 6, 7, 8])]))

def reduce_digits(num, acc, init, acc_called=False):
    match num:
        case 0: return init if acc_called else acc(num, init)
        case _ if num > 0: return reduce_digits(num // 10, acc, acc(init, num % 10), True)
        case _: raise ValueError("Numarul nu poate fi negativ")

get_digits = lambda num, pred: reduce_digits(num, lambda acc, d: acc + [d] if pred(d) else acc, [])

def make_number(digits, pred):
    match digits:
        case []: return 0
        case [d, *rest]:
            follows = make_number(rest, pred)
            return d + follows * 10 if pred(d) else follows

def make_number_builtin(digits, pred):
    return reduce(lambda acc, d: acc * 10 + d, filter(pred, digits))

def from_to(start, end, div=1):
    if start == end:
        return []

    match start % div:
        case 0: return [start] + from_to(start + div, end, div)
        case _: return from_to(start + (div - start % div), end, div)

def nth(l, n):
    match (n, l):
        case (0, [v]): return v
        case (_, []): raise ValueError("List is too small")
        case (n, [_, *rest]): return nth(rest, n - 1)

def first_n(l, n):
    match (n, l):
        case (0, _) | (_, []): return []
        case (n, [v, *rest]): return [v] + first_n(rest, n - 1)

exists = lambda l, e: reduce(lambda acc, v: acc or v == e, l, False)
count_if = lambda l, pred: reduce(lambda acc, v: acc + int(pred(v)), l, 0)

def split(l):
    match l:
        case []:
            return [], []
        case [(a, b), *rest]:
            left, right = split(rest)
            return [a] + left, [b] + right
        case _:
            raise ValueError("List is not of pairs")

def combine(left, right):
    match (left, right):
        case ([], []):
            return []
        case ([a, *rl], [b, *rr]):
            return [(a, b)] + combine(rl, rr)
        case ([_, *_], []) | ([], [_, *_]):
            raise ValueError("Lists do not have the same length")

def partition(e, pred):
    match e:
        case []: return [], []
        case [e, *rest]:
            satisfies, not_satisfying = partition(rest, pred)
            return ([e] + satisfies, not_satisfying) if pred(e) else (satisfies, [e] + not_satisfying)

def remove_consecutive_duplicates(l):
    match l:
        case [] | [_]: return l
        case [a, b, *rest]: return ([a] if a == b else [a, b]) + remove_consecutive_duplicates(rest)

def compare_lists(a, b):
    match (a, b):
        case ([], []): return 0
        case ([], _): return 1
        case (_, []): return -1
        case ([va, *ra], [vb, *rb]):
            return compare_lists(ra, rb) if va == vb else ((va > vb) - (va < vb))

def interclasare(a, b):
    match (a, b):
        case ([], []): return []
        case (v, []) | ([], v):
            return v
        case ([a, *ra], b) if a < b[0]:
            return [a] + interclasare(ra, b)
        case (a, [b, *rb]) if b < a[0]:
            return [b] + interclasare(a, rb)
        case ([a, *ra], [b, *rb]):
            return [a, b] + interclasare(ra, rb)

print(interclasare([1, 3, 5], [2, 4, 6]))
print(interclasare([3], [1]))

def split_in_halves(l):
    def impl(slow, fast):
        front_slow, rest_slow = (None, []) if len(slow) == 0 else (slow[0], slow[1:])
        rest_fast = [] if len(fast) < 2 else fast[2:]
        match rest_fast:
            case []: return ([] if front_slow is None else [front_slow]), rest_slow
            case _:
                follows_slow, follows_fast = impl(rest_slow, rest_fast)
                return ([front_slow] + follows_slow), follows_fast
    return impl(l, l)

print(split_in_halves([1, 2, 3, 4, 5, 6, 7, 8, 9]))

def merge_sort(l):
    match l:
        case [] | [_]: return l
        case _:
            left, right = split_in_halves(l)
            return interclasare(merge_sort(left), merge_sort(right))

print((merge_sort([832, 23, 1, 342, 1231, 54, 34, 123, 4])))