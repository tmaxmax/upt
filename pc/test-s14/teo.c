#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int year;
    short month, day;
} Date;

#define PRfDate "%04d-%02hd-%02hd"
#define PRaDate(d) (d).year, (d).month, (d).day

bool date_read(FILE *in, Date *out) {
    if (fscanf(in, "%4d-%2hd-%2hd", &out->year, &out->month, &out->day) != 3) {
        return false;
    }

    if (out->year < 1 || out->month < 1 || out->month > 12 || out->day < 1) {
        return false;
    }

    if (out->month == 2) {
        const bool is_leap_year = out->year % 4 == 0 && (out->year % 100 != 0 ||
                                                         out->year % 400 == 0);
        if (out->day > 28 + is_leap_year) {
            return false;
        }
    } else if (out->day > 30 + (out->month % 2 == out->month < 8)) {
        return false;
    }

    return true;
}

typedef struct {
    char county[3];
    short number;
    char series[4];
} Plate;

#define PRfPlate "%s%02hd%s"
#define PRaPlate(p) (p).county, (p).number, (p).series

bool plate_new(const char *s, Plate *out) {
    if (strlen(s) < 6) { // strlen("B01AAA")
        return false;
    }

    bool allow_triple_digit = false;

    memset(out->county, '\0', sizeof out->county);
    if (s[0] == 'B' && (isspace(s[1]) || isdigit(s[1]))) {
        out->county[0] = s[0];
        s++;
        allow_triple_digit = true;
    } else {
        memcpy(out->county, s, 2);
        s += 2;
    }

    const long number = strtol(s, &s, 10);
    if (number < 0 || number > (99 + allow_triple_digit * 900) ||
        strchr(s, '+') != NULL) {
        return false;
    }
    out->number = number;

    for (; isspace(*s); s++)
        ;
    if (strlen(s) != 3) {
        return false;
    }
    memcpy(out->series, s, 3);
    out->series[3] = '\0';

    return true;
}

typedef struct {
    Date created_at;
    Plate plate;
    float max_load;
} Car;

bool car_prompt(Car *out) {
    printf("Data fabricatie (YYYY-MM-DD): ");
    if (!date_read(stdin, &out->created_at)) {
        return false;
    }

    assert(getchar() == '\n');

    printf("Numar de inmatriculare (format romanesc): ");

    char input[11]; // strlen("AG 120 ABC") + 1
    if (fgets(input, sizeof input, stdin) == NULL) {
        return false;
    }
    input[strcspn(input, "\r\n")] = '\0';

    if (!plate_new(input, &out->plate)) {
        return false;
    }

    printf("Tonaj (0 daca nu e cazul): ");
    return scanf("%f", &out->max_load) == 1;
}

typedef struct {
    Car *data;
    size_t len, cap;
} CarDatabase;

CarDatabase db_new() { return (CarDatabase){NULL, 0, 0}; }

void db_add(CarDatabase *db, Car car) {
    if (db->len == db->cap) {
        db->cap = db->cap == 0 ? 4 : (db->cap / 2 * 3);
        db->data = realloc(db->data, db->cap * sizeof *db->data);
    }

    db->data[db->len++] = car;
}

void db_dump(const CarDatabase *db, FILE *out) {
    fprintf(out, "Numar masini: %zu\n", db->len);
    for (size_t i = 0; i < db->len; i++) {
        const Car *car = db->data + i;

        fprintf(out, "%zu: " PRfPlate " " PRfDate, i, PRaPlate(car->plate),
                PRaDate(car->created_at));
        if (car->max_load > 0) {
            fprintf(out, " %ft", car->max_load);
        }
        fputc('\n', out);
    }
}

typedef int (*CarSorter)(const void *a, const void *b);

void db_sort_by(CarDatabase *db, CarSorter sorter) {
    qsort(db->data, db->len, sizeof *db->data, sorter);
}

int car_sorter_by_year(const void *a, const void *b) {
    return ((const Car *)a)->created_at.year -
           ((const Car *)b)->created_at.year;
}

#define HELP_MSG                                                               \
    "Parcul tau auto!\n\n"                                                     \
    "Optiuni:\n"                                                               \
    "1) Introdu un autoturism\n"                                               \
    "2) Afiseaza autoturismele\n"                                              \
    "3) Sorteaza dupa anul de fabricatie\n"                                    \
    "0) Iesi\n\n"

int main(void) {
    CarDatabase db = db_new();

    printf(HELP_MSG);

    int input;
    Car input_car;

    while (true) {
        printf("> ");
        if (scanf(" %d", &input) != 1) {
            input = -1;
        }

        switch (input) {
        case 1:
            if (!car_prompt(&input_car)) {
                printf("Input invalid! Incearca din nou.\n");
                break;
            }
            db_add(&db, input_car);
            printf("Succes!\n");
            break;
        case 2:
            db_dump(&db, stdout);
            break;
        case 3:
            db_sort_by(&db, car_sorter_by_year);
            printf("Gata!\n");
            break;
        default:
            if (input == -1) {
                putchar('\n');
            }
            printf("La revedere!\n");
            return 0;
        }
    }
}
