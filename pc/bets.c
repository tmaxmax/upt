#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "enums.h"
#include "ticker/ticker.h"
#define TRY_READ_IMPLEMENTATION
#include "try_read.h"

enum BetKind { BetKindColor, BetKindNumber, BetKindParity };

static const ENUM_VARIANTS(enum BetKind) bet_kind_variants[] = {
    {"color", BetKindColor},
    {"number", BetKindNumber},
    {"parity", BetKindParity}};

GEN_ENUM_TO_STRING(enum BetKind, bet_kind, bet_kind_variants);

enum Parity { ParityEven, ParityOdd };

static const ENUM_VARIANTS(enum Parity) parity_variants[] = {
    {"even", ParityEven}, {"odd", ParityOdd}};

GEN_ENUM_PARSE(enum Parity, parity, parity_variants);
GEN_ENUM_TO_STRING(enum Parity, parity, parity_variants);

enum Color { ColorRed, ColorBlack, ColorGreen };

static const ENUM_VARIANTS(enum Color) color_variants[] = {
    {"red", ColorRed}, {"black", ColorBlack}, {"green", ColorGreen}};

GEN_ENUM_PARSE(enum Color, color, color_variants);
GEN_ENUM_TO_STRING(enum Color, color, parity_variants);

typedef struct {
    int stake;
    enum BetKind kind;
    union {
        uint8_t number;
        enum Parity parity;
        enum Color color;
    };
} Bet;

const char *bet_parse(const char *s, Bet *out) {
    const char *next;
    const long maybe_number = strtol(s, (char **)&next, 10);
    if (errno == 0 && maybe_number >= 0 && maybe_number <= 36) {
        out->kind = BetKindNumber;
        out->number = (uint8_t)maybe_number;
    } else if ((next = parity_parse(s, &out->parity)) != NULL) {
        out->kind = BetKindParity;
    } else if ((next = color_parse(s, &out->color)) != NULL) {
        out->kind = BetKindColor;
    } else {
        return NULL;
    }

    if (*next != ' ') {
        return NULL;
    }

    const long maybe_money = strtol(next, (char **)&next, 10);
    if (maybe_money <= 0) {
        return NULL;
    }

    out->stake = (int)maybe_money;

    return next;
}

static int parse_bets(const char *s, Bet *out, const size_t out_max_size) {
    int i = 0;
    while (i < out_max_size) {
        for (; *s == ' '; s++)
            ;
        if (*s == '\0') {
            return i;
        }

        s = bet_parse(s, &out[i]);
        if (s == NULL) {
            break;
        }
        i++;

        if (*s == '\0') {
            return i;
        } else if (*s == ',') {
            s++;
        } else {
            break;
        }
    }

    return -1; // either a parse error occured or too many bets were placed
}

static void ticker_display(void *data, const int second) {
    int num_seconds = *(int *)data;
    if (second == 1) {
        printf("  :  \x1b[s");
    }

    printf("\r%2d : \x1b[u", num_seconds - second + 1);
    fflush(stdout);
    printf("\x1b[s");
}

// Returns:
// - >0: successful input, the number of bets
// - 0: timeout or no input
// - -1: input error
// - -2: unrecoverable error (e.g. EOF)
static int read_bets(struct Ticker *t, Bet *out, const size_t out_max_size) {
    static int num_seconds = 10;

    if (ticker_start(t, ticker_display, &num_seconds) != 0) {
        return -2;
    }

    char buf[2048];
    ssize_t read_line_result =
        try_read_line(num_seconds * 1000, buf, sizeof buf);

    ticker_stop(t);

    switch (read_line_result) {
    case 0:  // EOF
    case -2: // Line too long
    case -3: // Error:
        return -2;
    case -1: // Timeout
        return 0;
    };

    return parse_bets(buf, out, out_max_size);
}

static enum Color get_color_for_number(const uint8_t num) {
    assert(num >= 0 && num <= 36);
    switch (num) {
    case 0:
        return ColorGreen;
    case 1:
    case 3:
    case 5:
    case 7:
    case 9:
    case 12:
    case 14:
    case 16:
    case 18:
    case 19:
    case 21:
    case 23:
    case 25:
    case 27:
    case 30:
    case 32:
    case 34:
    case 36:
        return ColorRed;
    default:
        return ColorBlack;
    }
}

static int play_round(uint8_t expected_num, const Bet *bets, size_t num_bets) {
    const enum Color expected_color = get_color_for_number(expected_num);
    const enum Parity expected_parity =
        expected_num % 2 == 0 ? ParityEven : ParityOdd;

    int balance = 0;

    for (const Bet *b = bets; b != bets + num_bets; b++) {
        switch (b->kind) {
        case BetKindNumber:
            if (b->number == expected_num) {
                balance += 36 * b->stake;
            } else {
                balance -= b->stake;
            }
            break;
        case BetKindColor:
            if (b->color == expected_color) {
                balance += (b->color == ColorGreen ? 14 : 2) * b->stake;
            } else {
                balance -= b->stake;
            }
            break;
        case BetKindParity:
            if (b->parity == expected_parity) {
                balance += 2 * b->stake;
            } else {
                balance -= b->stake;
            }
            break;
        }
    }

    return balance;
}

static void print_bet(const Bet *b, FILE *out) {
    if (b->stake == 0) {
        fprintf(out, "free, ");
    } else {
        fprintf(out, "%d$, ", b->stake);
    }
    switch (b->kind) {
    case BetKindNumber:
        fprintf(out, "number %d", b->number);
        break;
    case BetKindParity:
        fprintf(out, "%s", parity_to_string(b->parity));
        break;
    case BetKindColor:
        fprintf(out, "%s", color_to_string(b->color));
        break;
    default:
        assert(!"unreachable");
    }
}

// To compile (from repository root):
// $ gcc -lpthread -I./c -o bets.out pc/bets.c c/ticker/ticker.c

int main(int argc, char **argv) {
    printf("CASINO\nPress ENTER to play . . . ");
    fflush(stdout);
    char c;
    if (try_read_line(-1, &c, 1) != 1) {
        return 1;
    }

    srand(time(NULL));
    struct Ticker *t = ticker_new();
    Bet bets[128];

    int balance = 100;
    if (argc > 1) {
        const int parsed_balance = atoi(argv[1]);
        if (parsed_balance > 0) {
            balance = parsed_balance;
        }
    }

    printf("\nInput your bets (balance: %d$):\n", balance);

    while (true) {
        const int res = read_bets(t, bets, sizeof bets / sizeof *bets);
        switch (res) {
        case 0:
            printf("\nNo bets.\n");
            break;
        case -1:
            printf("Invalid input, try again:\n");
            continue;
        case -2:
            perror("\nSomething went terribly wrong");
            ticker_free(t);
            return 1;
        }

        int bets_required_balance = 0;

        if (res > 0) {
            printf("\nYour bets:\n");
            for (const Bet *b = bets; b != bets + res; b++) {
                bets_required_balance += b->stake;
                print_bet(b, stdout);
                printf("\n");
            }
        }

        if (bets_required_balance > balance) {
            printf("You don't have enough money to place these bets, try "
                   "again (%d$ > %d$):\n",
                   bets_required_balance, balance);
            continue;
        }

        uint8_t expected_num = rand() % 37;
        printf(
            "\nCorrect bet: number %hhu (%s, %s)\n", expected_num,
            color_to_string(get_color_for_number(expected_num)),
            parity_to_string(expected_num % 2 == 0 ? ParityEven : ParityOdd));

        balance += play_round(expected_num, bets, res);

        printf("Balance: %d$\n", balance);
        if (balance <= 0) {
            printf("You lost all your money, game over.\n");
            break;
        }

        printf(
            "Still in the game! Press ENTER to play again, or 'q' to quit: ");
        fflush(stdout);
        char input[2];
        if (try_read_line(-1, input, sizeof input) <= 0) {
            ticker_free(t);
            return 1;
        }

        if (input[0] == '\0') {
            printf("\n\nInput your bets:\n");
        } else {
            printf("\nThank you for playing! We're expecting you back any time "
                   "soon :)\n");
            break;
        }
    }

    ticker_free(t);

    return 0;
}
