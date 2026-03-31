#include "waveform.h"
#include <stdio.h>
#include "io.h"

int main() {
    PowerData myRecords[1000];  //because there is 100 lines in the csv

    int num_read = collect("ReadFrom.csv", myRecords, 1000);

    if (num_read > 0) {
        printf("Successfully read %d records.\n", num_read);
    } else {
        printf("No data found or error reading file.\n");
        return 2;
    }

    return 0;
}
