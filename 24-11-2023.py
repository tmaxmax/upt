def suma_cifre_impare(n):
    return sum(filter(lambda d: d % 2 == 1, map(int, str(n))))


print(suma_cifre_impare(3756283))


def aduna_numar_la_set(s, n):
    return [*map(lambda x: x + n, s)]


print(aduna_numar_la_set({1, 2, 3, 4}, 5))

bt = {
    "value": 5,
    "left": {
        "value": 4,
        "left": {
            "value": 6,
            "left": {"value": 22, "left": None, "right": None},
            "right": {"value": 3, "left": None, "right": None},
        },
        "right": None,
    },
    "right": {
        "value": 10,
        "left": None,
        "right": {"value": 1, "left": None, "right": None},
    },
}

val = lambda bt: bt["value"]
left = lambda bt: bt["left"]
right = lambda bt: bt["right"]


def numara_noduri_div_cu_3(bt):
    if bt is None:
        return 0

    return (
        int(val(bt) % 3 == 0)
        + numara_noduri_div_cu_3(left(bt))
        + numara_noduri_div_cu_3(right(bt))
    )


print(numara_noduri_div_cu_3(bt))
