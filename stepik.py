def remove_duplicates(ln):
    i = 0
    while i < len(ln):
        j = i + 1
        while j < len(ln):
            if ln[i] == ln[j]:
                del ln[j]
            else:
                j += 1
        i += 1

def check_subsequence(l1, l2):
    for i in range(len(l2)-len(l1)):
        if l1 == l2[i:i+len(l1)]:
            return True
    return False

def factorial(n):
    return 1 if n <= 1 else n * factorial(n - 1)

def fin_sum(x, n):
    return 0 if n == 0 else (((((-1) ** n) * (x ** (2 * n))) / factorial(2 * n)) + fin_sum(x, n - 1))

def filter_floats(l):
    res = []
    for n in l:
        try:
            res.append(float(n))
        except:
            pass
    return res

def factorial_ex(n):
    if type(n) is not int:
        raise TypeError('Factorial argument must be integer')
    if n < 0:
        raise ValueError('Factorial argument cannot be negative')
    
    return factorial(n)

from functools import reduce
from operator import add

def write_sum(in_file, out_file):
    with open(out_file, "w") as fout:
        try:
            with open(in_file, "r") as fin:
                fout.write(str(reduce(add, reduce(list.__add__, map(lambda x: [*map(int, [*x])], fin.read().strip().split(" "))))))
        except FileNotFoundError:
            fout.write("0")

l = [8, 11, 12, 12, 12, 14, 15, 12]
remove_duplicates(l)
print(l)

l1 = [12, 13, 14]
l2 = [8, 11, 12, 13, 14, 16, 15, 10]
print(check_subsequence(l1, l2))

print(fin_sum(0.5, 15))

write_sum("test.in", "test.out")