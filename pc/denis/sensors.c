#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int gen_rand_number(int min, int max) {
    return (rand() % (max - min + 1) + min);
}

void write_file(char *out_path) {
    FILE *output = fopen(out_path, "wb");
    uint32_t nr_elem = gen_rand_number(10, 1000);
    uint16_t year = 0;
    int16_t value = 0;
    uint8_t month = 0, day = 0, index = 0;
    fwrite(&nr_elem, sizeof(uint32_t), 1, output);
    while (nr_elem--) {
        year = gen_rand_number(1950, 2024);
        month = gen_rand_number(1, 11);
        day = gen_rand_number(1, 30);
        index = gen_rand_number(0, 4);
        value = gen_rand_number(-2000, 2000);
        fwrite(&year, sizeof(uint16_t), 1, output);
        fwrite(&month, sizeof(uint8_t), 1, output);
        fwrite(&day, sizeof(uint8_t), 1, output);
        fwrite(&index, sizeof(uint8_t), 1, output);
        fwrite(&value, sizeof(int16_t), 1, output);
    }
    fclose(output);
}

typedef enum { Working, With_Problems, Broken } State;

typedef struct {
    uint16_t year;
    uint8_t month;
    uint8_t day;
} Date;

typedef struct {
    int16_t value;
    Date data_senzor;
} Register_value;

typedef struct {
    State status;
    int nr_inregistrari;
    Register_value *inregistrari;
} Sensor_list;

void show_date(Date data) {
    printf("%d/%d/%d ", data.day, data.month, data.year);
}

void show_sensor_info(Register_value *senzor, int elem) {
    for (int j = 0; j < elem; j++) {
        show_date(senzor[j].data_senzor);
        printf("-> %d\n", senzor[j].value);
    }
}

void print_state(Sensor_list senzor) {
    switch (senzor.status) {
    case Working:
        printf("Working\n");
        break;
    case With_Problems:
        printf("Working with problems\n");
        break;
    case Broken:
        printf("Broken\n");
        break;
    default:
        printf("Unssuported status\n");
        break;
    }
}

void show_sensors(Sensor_list *senzori) {
    for (int i = 0; i < 5; i++) {
        printf("Senzori[%d] : ", i + 1);
        print_state(senzori[i]);
        show_sensor_info(senzori[i].inregistrari, senzori[i].nr_inregistrari);
    }
}

void show_data_senzor(uint16_t year, uint8_t month, Register_value *valori,
                      int nr_inregistrari, int index, int *count) {
    for (int i = 0; i < nr_inregistrari; i++) {
        if (valori[i].data_senzor.year == year &&
            valori[i].data_senzor.month == month) {
            *count += 1;
            printf("%d : ", index);
            show_sensor_info(valori + i, 1);
        }
    }
}

void search_by_month_year(Sensor_list *senzori) {
    uint16_t an = 0;
    uint8_t luna = 0;
    int count = 0;
    scanf("%hd", &an);
    scanf("%hhd", &luna);
    for (int i = 0; i < 5; i++) {
        show_data_senzor(an, luna, senzori[i].inregistrari,
                         senzori[i].nr_inregistrari, i, &count);
    }
    if (count == 0) {
        printf("Nici o inregistrare\n");
    }
}

Date get_date(FILE *buffer) {
    Date date;
    fread(&date.year, sizeof(uint16_t), 1, buffer);
    fread(&date.month, sizeof(uint8_t), 1, buffer);
    fread(&date.day, sizeof(uint8_t), 1, buffer);
    return date;
}

Register_value add_data(FILE *buffer, Date date, Register_value senzor_data) {
    senzor_data.data_senzor = date;
    uint16_t value = 0;
    fread(&value, sizeof(int16_t), 1, buffer);
    senzor_data.value = value;
    return senzor_data;
}

Register_value *retrieve_sensor(FILE *buffer, Sensor_list senzor, Date date) {
    senzor.inregistrari = realloc(
        senzor.inregistrari, senzor.nr_inregistrari * sizeof(Register_value));
    senzor.inregistrari[senzor.nr_inregistrari - 1] =
        add_data(buffer, date, senzor.inregistrari[senzor.nr_inregistrari - 1]);
    return senzor.inregistrari;
}

Sensor_list *read_file(char *buffer_path, Sensor_list *senzori) {
    Date date;
    uint32_t element_number = 0;
    FILE *buffer = fopen(buffer_path, "rb");
    fread(&element_number, 4, 1, buffer);
    uint8_t index = 0;
    for (int i = 0; i < element_number; i++) {
        date = get_date(buffer);
        fread(&index, sizeof(uint8_t), 1, buffer);
        senzori[index].nr_inregistrari += 1;
        senzori[index].inregistrari =
            retrieve_sensor(buffer, senzori[index], date);
    }
    fclose(buffer);
    return senzori;
}

Sensor_list *initialize_array(Sensor_list *senzori) {
    for (int i = 0; i < 5; i++) {
        senzori[i].status = Working;
        senzori[i].nr_inregistrari = 0;
        senzori[i].inregistrari = NULL;
    }
    return senzori;
}

int get_state(Register_value *valori, int nr_inregistrari) {
    int State = Working;
    for (int i = 0; i < nr_inregistrari; i++) {
        if (valori[i].value > 1000 || valori[i].value < -1000) {
            if (State == Working && i > 0) {
                return With_Problems;
            } else {
                State = With_Problems;
            }
        } else if (State == Working) {
            if (i == nr_inregistrari - 1) {
                return Working;
            }
        } else {
            if (State == With_Problems) {
                return With_Problems;
            }
        }
    }
    return Broken;
}

void set_sensor_state(Sensor_list *senzori) {
    for (int i = 0; i < 5; i++) {
        senzori[i].status =
            get_state(senzori[i].inregistrari, senzori[i].nr_inregistrari);
    }
}

void eliberare(Sensor_list *senzori) {
    for (int i = 0; i < 5; i++) {
        free(senzori[i].inregistrari);
    }
    free(senzori);
}

int main(void) {
    srand(time(NULL));
    char test_file[] = "senzor.bin";
    write_file(test_file);
    Sensor_list *senzori = malloc(5 * sizeof(Sensor_list));
    senzori = initialize_array(senzori);
    senzori = read_file(test_file, senzori);
    set_sensor_state(senzori);
    show_sensors(senzori);
    for (int i = 0; i < 5; i++) {
        print_state(senzori[i]);
    }
    search_by_month_year(senzori);
    eliberare(senzori);
    return 0;
}
