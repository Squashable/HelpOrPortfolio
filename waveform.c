#include "waveform.h"
#include <stdio.h>
#include "io.h"

int check() {
    PowerData myRecords[1000];  //because there is 1000 lines in the csv

    int alive = collect("ReadFrom.csv", myRecords, 1000);

    if (alive > 0) {
        printf("Successfully read %d records.\n", alive);
    } else {
        printf("No data found or error reading file.\n");
        return 2;
    }

    return 0;
}
