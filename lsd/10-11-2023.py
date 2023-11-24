from functools import reduce


def problema_mayei(nr):
    un = set()
    inte = None
    for n in nr:
        d = set(int(digit) for digit in str(n))
        if not un:
            un = d  # haha referinte go brr
        else:
            un.update(d)

        if inte is None:
            inte = d
        else:
            inte.intersection_update(d)

    return un, inte


print(problema_mayei({1234, 123, 127}))


def sum_add_to_dict(d, p):
    k, v = p
    if k in d:
        d[k] += v
    else:
        d[k] = v
    return d


def sum_values(d):
    return reduce(sum_add_to_dict, d, {})


print(sum_values([("a", 7), ("b", 5), ("c", 2), ("a", 3), ("b", 3)]))


def count_chars(l):
    def count_chars_in_string(v):
        def accumulator(acc, c):
            if c in acc:
                acc[c] += 1
            else:
                acc[c] = 1
            return acc

        return reduce(accumulator, v, {})

    return reduce(
        lambda acc, v: reduce(sum_add_to_dict, v.items(), acc),
        map(count_chars_in_string, l),
        {},
    )


print(count_chars(["aaa", "bbb", "aabbb"]))


def dict_filter(d, cond):
    def accumulator(acc, p):
        k, v = p
        if cond(k, v):
            acc[k] = v
        return acc

    return reduce(accumulator, d.items(), {})


print(dict_filter({"a": 5, "b": 7, "c": 1}, lambda _, v: v >= 5))


def exists_in_dict(d, cond):
    return reduce(lambda acc, p: acc or cond(p[0], p[1]), d.items(), False)


print(exists_in_dict({"a": 5, "b": 7, "c": 1}, lambda _, v: v >= 5))


def for_all_dict(d, cond):
    return reduce(lambda acc, p: acc and cond(p[0], p[1]), d.items(), True)


print(for_all_dict({"a": 5, "b": 7, "c": 1}, lambda _, v: v >= 5))


def dict_map(d, val_mapper):
    def accumulator(acc, p):
        k, v = p
        acc[k] = val_mapper(v)
        return acc

    return reduce(accumulator, d.items(), {})


print(dict_map({"a": 5, "b": 7, "c": 6}, lambda x: x + 1))


def vals_corresponding_to_keys(d, keys):
    return set(dict_filter(d, lambda k, _: k in keys).values())


print(vals_corresponding_to_keys({"aa": 5, "bb": 7, "ca": 6}, ["aa", "bb", "c"]))


def max_by_key(d, key):
    return max(dict_map(d, key).values())


print(max_by_key({"a": 5, "b": 3, "c": 6}, lambda v: v * 2))
