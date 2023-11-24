def ex_1(a, b):
    return [min(a, b), (a + b) / 2, a * b]

print(ex_1(5, 7))

def ex_2(x, inc):
    if inc:
        return lambda a: a + x
    return lambda a: a - x

print(ex_2(3, True)(5))
print(ex_2(3, False)(5))

def ex_3(l):
    return max(filter(lambda x: x % 2 == 0, l))

print(ex_3([4, 8, 4, 5, 9, 23, 12, 2]))