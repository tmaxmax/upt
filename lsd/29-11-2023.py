cheie = "cheie"
stanga = "stanga"
dreapta = "dreapta"

arbore = {
    cheie: "1",
    stanga: {
        cheie: "2",
        stanga: {cheie: "4", stanga: None, dreapta: None},
        dreapta: {cheie: "5", stanga: None, dreapta: None},
    },
    dreapta: {
        cheie: "3",
        stanga: {cheie: "6", stanga: None, dreapta: None},
        dreapta: None,
    },
}


def afiseaza_fara_frunze(a):
    if a is None or (a[stanga] is None and a[dreapta] is None):
        return

    print(a[cheie], end=" ")
    afiseaza_fara_frunze(a[stanga])
    afiseaza_fara_frunze(a[dreapta])


afiseaza_fara_frunze(arbore)
