#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct nod {
    struct nod *next;
    char key[11];
    char val[11];
} Nod; // avem o lista cu cheie si valoare, dar si pointer la urmatorul element
       // din lista

typedef Nod *
    *List_t; // aceasta declaratie ne va creea un tablou de liste, care va salva
             // pe fiecare pozitie, o lista cu chei astfel incat prima litera sa
             // fie specirfica pozitiei din vectior.

List_t
init_list(void) // Functie pentru initializarea tabloului de liste. Initial
                // fiecare lista este nula pana la adaugarea unui element.
{
    List_t data_list = malloc(26 * sizeof(Nod *));
    for (int i = 0; i < 26; i++) {
        data_list[i] = NULL;
    }
    return data_list;
}

void show_element(
    Nod *element) // Aceasta functie este folosita pentru afisarwea fiecarui
                  // element din liste sub forma cheie -> valoare
{
    printf("%s -> %s\n", element->key, element->val);
}

Nod *find_key(
    Nod *list_letter,
    char *key) // Aceasta functie va gasi elementul cu cheia key, daca nu exista
               // il va creea si va returna un pointer la acest element pentru
               // adaugarea valorii. Aceasta abordare este folositoare pentru
               // aduagarea elementelor deoarece adaugarea unui element intr-o
               // lista este mai rapid decat adaugarea unui element in vector,
               // deoarece nu mai este nevoie sa mutam toate elementele cu o
               // pozitie in dreapta. Functia va adauga in ordine crescatoare
               // cheia.
{
    while ((list_letter->next != NULL) && (strcmp(list_letter->key, key) < 0)) {
        list_letter = list_letter->next;
    }
    if (list_letter != NULL) {
        if (strcmp(list_letter->key, key) ==
            0) { // In cazul in care cheia a fost gasita vom returna pointer la
                 // acest element pentru a-i schimba valoarea
            return list_letter;
        }
    }

    Nod *new_elem =
        malloc(sizeof(Nod)); // In cazul in care cheia nu exista in lista, se va
                             // creea un element nou cu cheia key, si va fi
                             // returnat pointer la acest element

    assert(new_elem != NULL);

    strcpy(new_elem->key, key);

    if (list_letter->next == NULL) {
        list_letter->next = new_elem;
        new_elem->next = NULL;
    } else {
        new_elem->next = list_letter->next;
        list_letter->next = new_elem;
    }

    return new_elem;
}

Nod *get_pos(List_t data_list,
             char *key) // Aceasta functie verifica in tabloul de liste daca
                        // lista este nula, caz in care nu se mai cauta cheia in
                        // lista, se creeaza elementul nou, se adauga in tablou,
                        // si se returneaza pointer la acest element
{
    size_t pozitie_litera = tolower(key[0]) - 'a';
    if (data_list[pozitie_litera] ==
        NULL) { // Daca rezultatul este null atunci lista specifica literei nu
                // exista, asa ca evident elementul va fi adaugat pe prima
                // pozitie
        Nod *new_elem = malloc(sizeof(Nod));
        assert(new_elem != NULL);
        strcpy(new_elem->key, key);
        data_list[pozitie_litera] =
            new_elem; // adaugam elementul pe prima pozitie
        new_elem->next = NULL;
        return new_elem;
    } else { // Daca rezultatul nu este null atunci litera respectiva exista si
             // incepem cautarea in lista dupa cheia key.
        Nod *new_elem = find_key(data_list[pozitie_litera], key);
        if (new_elem->next ==
            data_list[pozitie_litera]) { // Verificam daca pointerul la
                                         // elementul nou este inaintea
                                         // elementului de pe pozitia din
                                         // tablou, iar daca este va fi inlocuit
                                         // inceputul
            data_list[pozitie_litera] = new_elem;
        }
        return new_elem;
    }
}

Nod *read_line(
    FILE *buffer) // Aceasta functie este folosita pentru citirea unei linii din
                  // fisier de forma cheie (n spatii) valoare
{
    Nod *new_node = malloc(sizeof(Nod));
    assert(new_node != NULL);
    char c = 0;
    size_t poz = 0;
    while (((c = fgetc(buffer)) != EOF) &&
           (c !=
            ' ')) { // citirea va fi realizata caracter cu caracter deoarece nu
                    // stim ce dimensiune are cheia, stim doar ca are maxim 10
                    // caractere, asa ca vom citi pana la intalnirea unui spatiu
        new_node->key[poz] = c;
        poz++;
    }

    if (c == EOF) { // se verifica daca este gata fisierul, caz in care evident
                    // informatia din fisier este introdusa eronat, astfel ca va
                    // fi eliberat si va returna null functia
        free(new_node);
        return NULL;
    }

    new_node->key[poz] = '\0';

    while (((c = fgetc(buffer)) != EOF) &&
           (c == ' ')) { // Se citesc toate spatiile deoarece nu stim ce numar
                         // de spatii este
        continue;
    }

    if (c == EOF) { // daca finalul de fisier este intalnit dupa spatii,
                    // informatia este eronat introdusa astfel ca va fi eliberat
                    // elementul iar functia va returna null
        free(new_node);
        return NULL;
    }

    poz = 0;

    do {
        new_node->val[poz] = c;
        poz++;
    } while (((c = fgetc(buffer)) != EOF) && (c != '\n'));

    new_node->val[poz] = '\0';
    return new_node;
}

void show_list(List_t data_list) // Functia va fi folosita pentru afisarea
                                 // tuturor listelor din tabloul de liste,
                                 // incepand de la pozitia specifica literei a
{
    Nod *element = NULL;
    ;
    for (int i = 0; i < 26; i++) {
        element = data_list[i];
        while (element != NULL) {
            show_element(element);
            element = element->next;
        }
    }
}

void free_list(List_t data_list) {
    Nod *element = NULL;
    Nod *urm = NULL;
    for (int i = 0; i < 26; i++) {
        element = data_list[i];
        while (element != NULL) {
            urm = element->next;
            free(element);
            element = urm;
        }
    }
    free(data_list);
}

int main(
    int argc,
    char *
        *argv) // Merita specificat faptul ca daca num arul de elemente citite
               // este foarte mic nu avem nevoie de o astfel de implementare,
               // deoarece timpul de executie este oricum foarte mic, iar
               // memoria utilizata este mai mica decat in cazul acestei
               // implementari, dar cum nu cunoastem numarul total de elemente
               // suntem nevoiti sa ne gandim si la un numar foarte mare de
               // elemente, astfel ca aceasta implementare va salva mai mult
               // timp deoarece vom parcurge doar elementele specifice unei
               // litere, iar adaugarea elementului pe o pozitie anume nu pierde
               // timp inutil. Pentru abordarea cu vector s-ar parcurge toate
               // elementele din vector pentru fiecare element nou, iar apoi se
               // vor muta elementele in dreapta cu o pozitie, lucru care
               // sugereaza ca ar trebui sa avem un tablou alocat dinamic. Daca
               // tabloul este alocat dinamic ar trebui sa folosim si functia de
               // realloc, care va pierde timp intr-un caz nefavorabil. In
               // implementarea cu liste nu folosim realocare deloc.
{
    assert((argc == 2) && ("Programul se apeleaza sub forma: './executabil "
                           "input_file_path'")); // assert este folosit pentru a
                                                 // verifica ficare caz care
                                                 // poate provoca erori. Partea
                                                 // cea mai buna etse faptul ca
                                                 // arata si linia la care codul
                                                 // a intalnit o eroare grava.
    FILE *buffer = fopen(argv[1], "r");
    assert(buffer != NULL);
    List_t data_list = init_list();
    Nod *new_element = NULL;
    Nod *position = NULL;

    while (
        (new_element = read_line(buffer)) !=
        NULL) { // O alta varianta de citire ar putrea fi o functie care citeste
                // intai intregul text, iar apoi preia informatia de pe fiecare
                // linie, si returneaza un element nou. Noi nu avem nevoie de
                // intreg textul dupa executarea functiei asa ca nu ar fi o idee
                // buna deoarece ar implica multe realocari, necunoscand
                // dimensiunea. Din aceste motive vom citi doar linie cu linie
                // informatia din text, iar apoi vom elibera informatia nou
                // citita. Aceasta varianta nu implica realocari deoarece avem
                // dimensiuni maxime pentru informatiile citite
        position = get_pos(data_list, new_element->key);
        strcpy(position->val, new_element->val);
        free(new_element); // elementul citit va fi eliberat deoarece a fost
                           // adaugat in lista specifica primei litere a cheii
    }

    show_list(data_list);
    free_list(data_list);
    fclose(buffer);
    return 0;
}
