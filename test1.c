#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct ReadData {
    float time;
    float temperature;
    float pressure;
};

struct WriteData {
    float time;
    float temperature;
    float pressure;
};

int collect(struct ReadData *data, int max) {
    FILE *fp = fopen("ReadFrom.csv", "r");
    if (fp == NULL) {
        printf("Error opening file\n");
        return 1;
    }

    char line[256];
    int count = 0;

    fgets(line, sizeof(line), fp);

    while (fgets(line, sizeof(line), fp) != NULL && count < max) {
        char *token;

        token = strtok(line, ",");
        if (token == NULL) continue;
        data[count].time = atof(token);

        token = strtok(NULL, ",");
        if (token == NULL) continue;
        data[count].temperature = atof(token);

        token = strtok(NULL, ",\n");
        if (token == NULL) continue;
        data[count].pressure = atof(token);

        count++;
    }

    fclose(fp);
    return count;
}

int calculation(struct ReadData *data,struct WriteData *write_data, int max) {
    
}

int main() {
    struct ReadData data[256];
    int records = collect(data, 256);

    if (records < 0) {
        printf("Failed to read file\n");
        return 2;
    }

    printf("Read %d records\n", records);
    for (int i = 0; i < records; i++) {
        printf("Time: %.2f | Temp: %.2f | Pressure: %.2f\n",
               data[i].time, data[i].temperature, data[i].pressure);
    }

    return 0;
}