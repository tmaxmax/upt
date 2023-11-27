#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char name[256];
    float quantity;
    float unit_price;
} Product;

static Product read_product(FILE *in) {
    Product p;

    fgets(p.name, sizeof p.name, in);
    // https://en.cppreference.com/w/cpp/string/byte/strcspn
    const size_t newline_index = strcspn(p.name, "\r\n");
    if (p.name[newline_index] == '\0') {
        perror("Numele produsului are mai mult de 255 de caractere\n");
        exit(1);
    }
    p.name[newline_index] = '\0';

    // Spatiul dintre format specifiers determina (f)scanf sa ignore
    // orice caracter de tip whitespace dintre numere. Sunt imporante
    // pentru a elimina newline-urile din input (tinand cont ca fiecare
    // numar si nume sunt pe un rand nou).
    fscanf(in, "%f %f ", &p.quantity, &p.unit_price);

    return p;
}

typedef struct {
    char product_name[256];
    float value;
} TotalPrice;

static TotalPrice new_total_price(const char *const name,
                                  const float inital_value) {
    TotalPrice p;

    // https://en.cppreference.com/w/cpp/string/byte/strncpy
    strncpy(p.product_name, name, sizeof p.product_name - 1);
    p.product_name[sizeof p.product_name - 1] = '\0';

    p.value = inital_value;

    return p;
}

static TotalPrice *find_by_product_name(TotalPrice *prices,
                                        const size_t len_prices,
                                        const char *const name) {
    for (size_t i = 0; i < len_prices; i++) {
        if (strcmp(prices[i].product_name, name) == 0) {
            return &prices[i];
        }
    }

    return NULL;
}

static void print_total_price(const TotalPrice *price, FILE *out) {
    // Parametrul price este un pointer si nu o valoare pentru a nu copia
    // toata valoarea (260 de bytes) pe stiva. "const" ii spune compilatorului
    // sa nu ne permita sa modificam valoarea – altfel, dat fiind ca e pointer,
    // am putea-o schimba, ceea ce nu ne dorim intr-o functie de afisare.
    fprintf(out, "Pretul total pentru %s: %.2f", price->product_name,
            price->value);
}

// Pentru rulare:
// $ gcc -o ex-6 ex-6.c
// $ ./ex-6 < ex-6.in
// Fisierul "ex-6.in" este in repository.

int main(void) {
    size_t len_product_prices = 0, cap_product_prices = 2;
    TotalPrice *product_prices =
        malloc(sizeof(TotalPrice) * cap_product_prices);
    float global_total_price = 0.0;

    size_t n;
    // La fel si aici e important spatiul, vezi explicatia de mai sus.
    scanf("%zu ", &n);

    for (size_t i = 0; i < n; i++) {
        const Product p = read_product(stdin);
        const float p_price = p.quantity * p.unit_price;

        TotalPrice *total =
            find_by_product_name(product_prices, len_product_prices, p.name);
        if (total) {
            total->value += p_price;
        } else {
            if (len_product_prices == cap_product_prices) {
                cap_product_prices *= 2;
                product_prices = realloc(
                    product_prices, sizeof(TotalPrice) * cap_product_prices);
            }

            // Chestia asta merge pentru ca ++ postfixat returneaza lungimea
            // veche, iar la acel index vrem sa introducem noua valoare.
            // Echivalent cu list.push() din Python.
            product_prices[len_product_prices++] =
                new_total_price(p.name, p_price);
        }

        global_total_price += p_price;
    }

    for (size_t i = 0; i < len_product_prices; i++) {
        print_total_price(&product_prices[i], stdout);
        putc('\n', stdout);
    }

    printf("Pret global: %.2f\n", global_total_price);

    free(product_prices);

    return 0;
}
