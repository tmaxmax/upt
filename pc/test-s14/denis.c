#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *open_file(const void *dest_file, const char *op) {
    // dest_file, fiind void*, nu spune absolut nimic despre ce ar trebui
    // sa fie dest_file. cumva trebuie documentate tipurile de date suportate,
    // ori prin numele parametrului (vezi cum am redenumit in citire), ori prin
    // comentarii.
    //
    // de altfel ai uitat de const pt ambii parametri. compilatorul meu imi da
    // warning la apelul functiei, pentru ca atunci cand scrii un string
    // literal, precum "r+", tipul sau este const char*, nu char*, si deci cand
    // apelezi open_file const-ul se duce. open_file nu modifica op, deci nu se
    // poate intampla nimic rau, insa daca ar fi sa schimbi functia sa modifice
    // datele spre care arata op, atunci iti va da segmentation fault cu un
    // string literal.
    //
    // corectitudinea const-urilor e esentiala pentru programator – daca eu
    // citesc numai antetul functiei si vad ca e char*, nu const char*, atunci
    // ma gandesc, luandu-ma dupa tipuri, ca nu pot sa dau ca parametru niciun
    // string literal, niciun const char*, ca poate mi-l modifica.
    //
    // nu uita de const-uri!!!
    //
    // P.S. acel const face constante valorile spre care arata adresa,
    // nu variabila adresei. op = ... este in continuare permis; ce nu se poate
    // (si ce ne intereseaza) este *op = ... sa nu se poata si fie clar si
    // pentru apelant ca nu se poate. daca parametrii nu sunt pointeri (de ex.
    // int) const-ul nu conteaza asa de mult – valoarea apelantului nu poate
    // fi modificata oricum.

    if (dest_file == stdin) {
        if (strcmp(op, "r") == 0) {
            return stdin;
        }
        return NULL;
    } else if (dest_file == stdout) {
        if (strcmp(op, "w") == 0) {
            return stdout;
        }
        return NULL;
    }
    // flag-urile a, b, e, x? combinatiile si permutarile lor?
    if (strcmp(op, "w") != 0 && strcmp(op, "r") != 0 && strcmp(op, "w+") != 0 &&
        strcmp(op, "r+") != 0) {
        return NULL;
    }
    // ce se intampla aici daca dest_file e un FILE* altul decat stdin sau
    // stdout?
    return fopen(dest_file, op);
}

void *aloc_memory(void *dest, size_t nr_bytes, size_t size) {
    // size_t pt marimi, realloc primeste size_t, malloc primeste size_t.
    // in plus, nu are sens sa dai numere negative aici, nu?
    // tipurile de date sunt cea mai buna forma de documentare. in scenarii ca
    // acesta iti dai seama ca size nu ar trebui sa fie negativ, insa atunci
    // cand ai functii necunoscute, mai complexe, cand valorile sunt campuri
    // ale unei structuri de date, proprietatile implicite nu mai sunt evidente.
    // este esential ca tipurile de date sa specifice cat mai exact
    // "domeniul de definitie" al functiei – la fel cum in matematica e gresit
    // sa spui "log : R -> R", asa e si aici gresit sa nu folosesti tipul
    // potrivit.

    if ((dest = realloc(dest, nr_bytes * size)) == NULL) {
        free(dest);
        exit(-1);
    }
    return dest;
}

int get_new_vector_size(int old_size) {
    if (old_size < 4) {
        return /* old_size + 1 */ 4; // cel mai probabil vei avea mai mult de 4
                                     // elemente, nu are sens sa faci alocari
                                     // din 1 in 1 acolo.
    }
    return ((old_size / 2) * 3);
}

// de altfel eu as combina aloc_memory si get_new_vector_size intr-o singura
// functie

void invert_ascii_letter_casing(char *text) {
    // numele anterior era "modificare_text". cum modifica functia textul?
    // o functie cu astfel de denumire generica s-ar putea numi la fel de bine
    // "foo", "functie", "sarmale"...
    //
    // functiile sunt cele mai utile atunci cand nu trebuie sa le citesti
    // implementarea. functiile sunt o unealta pentru abstractizare;
    // abstractizarea inseamna sa ascunzi detalii; a ascunde detalii inseamna
    // a-ti face viata mai usoara. altfel n-are valoare functia.
    //
    // de altfel am scapat de parametrul size pentru ca text e un C string
    // care ar trebui sa se termine cu caracterul nul si ne putem folosi de
    // asta.
    for (/* int */ size_t i = 0; text[i] != '\0'; i++) {
        // am schimbat putin structura aici ca sa fie mai putin nesting.
        // incearca cat poti sa nu treci de 2 nivele.
        if (isalpha(text[i])) {
            // o mica schema cu ternary :)
            text[i] = (islower(text[i]) ? toupper : tolower)(text[i]);
        } else if (!isalnum(text[i])) {
            text[i] = '-';
        }
    }
    // e inutil sa returnezi – tu modifici sirul de caractere deja existent.
    // merita sa returnezi numai daca faci o copie a sirului si o modifici pe
    // aia sau schimbi pointer-ul (de exemplu avansezi in sir ca sa "stergi"
    // niste caractere de la inceput). nici una, nici alta nu se intampla in
    // functia ta.
    /* return text; */
}

char *citire(int *size, void *filename_or_handle) {
    // problema cu functia open_file si parametrul void* este ca el devine
    // "viral", adica toate functiile care il folosesc vor trebui sa primeasca
    // un void*. problema nedocumentarii se transmite astfel tuturor functiilor
    // apelante.

    // pune pe aceeasi linie declararea si initializarea de fiecare data cand e
    // posibil. de altfel uita-te in codul meu cum fac alocarea – cu schimbari
    // minore poti evita alocarea asta initiala. in plus, daca faci alocarea
    // intr-un singur loc nu mai ai nevoie de functie separata, deci e mai putin
    // cod :D
    char *linie = aloc_memory(NULL, sizeof(char), 1);
    int max_size = 1;
    // mai degraba ai da un FILE* valid direct ca parametru si ai deschide
    // fisierul in main, nu vad de ce citirea s-ar ocupa de asta.
    // daca citesti de mai multe ori din acelasi fisier?
    FILE *f = open_file(filename_or_handle, "r");
    if (f == stdout) {
        return NULL;
    }
    // de altfel eu aici as face loop-ul asta cu fgets, ca sa citesc mai multe
    // caractere deodata. avantajul ar fi ca ai face mai putine alocari si
    // cel mai probabil nu s-ar face mai mult de un singur fgets (daca ii dai
    // un buffer size de 256 de exemplu, cat de des ai avea input-uri mai lungi?
    // ai face o singura alocare de 256 bytes la inceput si o singura citire,
    // mai rapid de atat nu se poate). tema pentru tine sa vezi cum se face.
    while ((linie[*size] = fgetc(f)) != EOF) {
        *size += 1;
        if (*size == max_size) {
            max_size = get_new_vector_size(max_size);
            linie = aloc_memory(linie, sizeof(char), max_size);
        }
    }
    // open_file mai are o problema. este o abstracie "leaky" – adica trebuie
    // sa stii detaliile de implementare ca sa o folosesti corect. daca nu stiai
    // ca open_file poate primi si stdin, ca e un caz special, ai fi inchis
    // in mod eronat si stdin-ul.
    // daca ai deschide si inchide in main fisierele ai stii ce sunt
    // (daca e stdin/stdout sau altceva) si, pe langa asta, functiei de citire
    // nu ar mai trebui sa ii pese de fisier, facand-o mai simpla si mai utila.
    // functiile sunt mai generale si mai utile cu cat le pasa de mai putine
    // lucruri.
    if (f != stdin) {
        fclose(f);
    }
    if (*size == 0) {
        free(linie);
        return NULL;
    } else {
        *size -= 1;
        linie[*size] = '\0';
        // nu m-as mai chinui sa ii dau downsize. eu stiu ca iti dau cerinta
        // aia cretina ca trebuie sa ocupe atat spatiu cat e necesar; in
        // realitate nu te-ai chinui in modul asta – alocarile si dealocarile
        // sunt scumpe, iau mult timp, asa ca trebuie atins un balans intre
        // overhead-ul de memorie si cel de timp. de cele mai multe ori vei
        // prefera timpul, ca memorie e destula (acest singur byte nu face
        // diferenta, crede-ma). in plus, in locuri unde memoria e putina si
        // foarte importanta nu ai folosi heap oricum, ci buffer-uri pe stack
        // de marime fixa, deci nici nu ai avea problema asta..
        //
        // P.S. mi-am dat seama mai tarziu ca size e folosit si ca si lungimea
        // string-ului. avand in vedere ca ii pui terminatorul nul nu mai e
        // nevoie de size, deci el ramane practic util numai ca sa iti dai seama
        // la citire cand trebuie sa realoci.
        /* linie = aloc_memory(linie, sizeof(char), *size + 1); */
        /* linie = */ invert_ascii_letter_casing(linie);
        return linie;
    }
}

/*
Daca ar fi sa fie util programul asta, eu as face in felul urmator:

// Reads until EOF or newline. Returns NULL if no characters were read.
// The returned string must be deallocated by the caller with free().
char* read_line(FILE* in);

void invert_ascii_letter_casing(char* text);

int main(void) {
    char* line = read_line(stdin);
    if (line == NULL) {
        fprintf(stderr, "No characters were read\n");
    } else {
        invert_ascii_letter_casing(line);
        printf("%s\n", line);
        free(line);
    }
    return 0;
}
*/

int main(void) {
    // ai putea declara size in citire direct, ca nu ai nevoie de el afara.
    // de altfel ar trebui sa fie size_t.
    int size = 0;

    /*
    per total eu as renunta complet la functia aia de open_file
    si as face deschiderea si inchiderea in main. de exemplu:

    FILE* f = fopen("input.txt", "r");
    if (f == NULL) {
        fprintf(stderr, "Could not open input.txt\n");
        return 1;
    }

    // ...

    (void)fclose(f); // (void) ca nu ai ce sa faci cu eroarea oricum.

    Daca ai nevoie sa inlocuiesti cu stdin/stdout, ii foarte simplu:

    FILE* f = stdin;

    si comentezi codul cu NULL si cu inchiderea.
    */

    // de altfel ce cerinta cretina, sa faci modificarea in citire – practic
    // functia citire e main-ul tau acum. ce valoare are sa mai ai doua functii?
    char *linie = citire(&size, stdin);
    if (linie == NULL) {
        printf("Nu s-a citit nici un caractere\n");
    } else {
        printf("%s\n", linie);
        free(linie);
    }
    return 0;
}
