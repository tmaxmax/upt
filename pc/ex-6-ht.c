#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "hash_table/hash_table.h"
#include "hash_table/hashers.h"
#include "hash_table/values.h"

typedef struct {
    char name[256];
    float quantity;
    float unit_price;
} Product;

Product read_product(FILE *in) {
    Product p;

    fgets(p.name, sizeof p.name, in);
    const size_t newline_index = strcspn(p.name, "\r\n");
    if (p.name[newline_index] == '\0') {
        perror("Numele produsului are mai mult de 255 de caractere\n");
        exit(1);
    }
    p.name[newline_index] = '\0';

    fscanf(in, "%f %f ", &p.quantity, &p.unit_price);

    return p;
}

bool print_total_prices_entry(void *out, const void *key, void *value) {
    fprintf(out, "Pretul pentru %s: %.2f\n", (const char *)key,
            ht_v_float(value));

    return true;
}

// Pentru rulare (presupunand ca au fost compilate dependentele):
// $ gcc -I../c/ -L../c/lib -lht -lxxhash -o ex-6-h.out ex-6-ht.c
// $ ./ex-6-ht.out < ex-6-ht.in
// Fisierul "ex-6-ht.in" este in repository.

int main(void) {
    // Un dictionar cu chei de tip char* (string) si valori float.
    HashTable product_prices = ht_new(ht_hasher_string, strcmp, strdup, free);
    float global_total_price = 0.0;

    size_t n;
    scanf("%zu ", &n);

    for (size_t i = 0; i < n; i++) {
        const Product p = read_product(stdin);
        const float p_price = p.quantity * p.unit_price;
        float price_in_table = ht_v_float(ht_get(product_prices, p.name));
        price_in_table += p_price;
        ht_upsert(product_prices, p.name, ht_v_from_float(price_in_table));
        global_total_price += p_price;
    }

    ht_for_each(product_prices, stdout, print_total_prices_entry);
    printf("Pret global: %.2f\n", global_total_price);

    ht_free(product_prices, NULL);

    return 0;
}
