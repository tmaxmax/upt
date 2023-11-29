for elem in filter(lambda x: x % 2 == 0, [1, 2, 3, 4, 5]):
    print(elem)

from math import sqrt

for x in map(sqrt, [1, 2, 3, 4, 5]):
    print(x)


def reduce(fn, it, acc):
    for x in it:
        acc = fn(acc, x)
    return acc


def create_list():
    return [1, 2, 3, 4, 5]
