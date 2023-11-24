from functools import reduce
from operator import and_, or_

def stringify_set(s):
    return "{" + ",".join(map(str, s)) + "}"

print(stringify_set({1, 2, 3}))

def get_first_unique_from_pairs(l):
    return set(map(lambda p: p[0], l))

print(get_first_unique_from_pairs([(1, 2), (3, 4)]))

def my_partition(f, s):
    if len(s) == 0:
        return set(), set()
    
    elem = s.pop()
    left, right = my_partition(f, s)
    if f(elem):
        left.add(elem)
    else:
        right.add(elem)
    
    return left, right

print(my_partition(lambda x: x % 2 == 0, {1, 2, 3, 4}))

def my_filter(f, s):
    return my_partition(f, s)[0]

print(my_filter(lambda x: x % 2 == 0, {1, 2, 3, 4}))

def join_sets(l):
    return reduce(or_, l, set())

print(join_sets([{1, 2, 3}, {1, 2, 3, 4}, {3, 5, 6, 7}]))

def intersect_sets(l):
    try:
        return reduce(and_, l)
    except TypeError:
        return set()

print(intersect_sets([]))
print(intersect_sets([{1, 2, 3}, {1, 2, 3, 4}, {3, 5, 6, 7}]))

def digits_of_num(n, is_first_call=True):
    match (n, is_first_call):
        case (0, True): return {0}
        case (0, False): return set()
        case _:
            follows = digits_of_num(n // 10, False)
            follows.add(n % 10)
            return follows

print(digits_of_num(1234))

def digits_of_num_list(l):
    return join_sets(map(digits_of_num, l))

print(digits_of_num_list([1234, 123, 127]))