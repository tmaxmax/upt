#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *read_line(FILE *in) {
    size_t len = 0, cap = 0;
    char *line = NULL;

    // Inceputul ultimei citiri. Folosim acest pointer pentru a sterge
    // newline-ul de la final si pentru a evita sa parcurgem toata linia citita.
    // Fara sa facem asta ar trebui ori sa parcurgem toata linia la final,
    // ori in while sa parcurgem fiecare citire sa vedem daca are newline.
    // Acest pointer va arata spre memorie care apartine lui line.
    char *last_read_start;

    // Citim intr-un buffer si nu direct pe heap. Am incercat sa fac direct pe
    // heap insa mi-a rezultat intotdeauna o alocare in plus inainte de ultimul
    // apel de fgets, care returneaza NULL si nu citeste nimic. Control flow-ul
    // devenea prea complicat daca incercam sa evit. In plus, asa evitam o
    // alocare pe heap nenecesara daca input-ul e gol.
    char buf[256];

    while (fgets(buf, sizeof buf, in) != NULL) {
        if (len + sizeof buf > cap) {
            cap = cap == 0 ? sizeof buf : (cap / 2 * 3);
            line = realloc(line, cap);
        }

        last_read_start = line + len;
        memcpy(line + len, buf, sizeof buf - 1); // -1 pt ca fgets pune \0
        len += sizeof buf - 1;
    }

    if (line == NULL) {
        return NULL;
    }

    const size_t last_read_len = strcspn(last_read_start, "\r\n");
    last_read_start[last_read_len] = '\0';

    return line;
}

void invert_ascii_letter_casing(char *text) {
    for (size_t i = 0; text[i] != '\0'; i++) {
        if (isalpha(text[i])) {
            text[i] = (islower(text[i]) ? toupper : tolower)(text[i]);
        } else if (!isalnum(text[i])) {
            text[i] = '-';
        }
    }
}

int main(void) {
    // functia main a fost scrisa de github copilot ;)
    // daca ai functii bine denumite si AI-ul se prinde ce vrei sa faci.

    char *line = read_line(stdin);
    if (line == NULL) {
        fprintf(stderr, "Nu s-a putut citi linia\n");
        return 1;
    }

    invert_ascii_letter_casing(line);
    printf("%s\n", line);
    free(line);

    return 0;
}
