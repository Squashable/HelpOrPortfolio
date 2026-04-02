#include "waveform.h"
#include "io.h"
#include <stdio.h>

int welcome(void) {
    printf("*******************************\n");
    printf("*    ELECTRICAL CALCULATOR    *\n");
    printf("*       Power Summary         *\n");
    printf("*******************************\n");
    return 0;
}

static int write_data_file(PowerData *myRecords, int alive) {
    FILE *fp = fopen("WriteTo.txt", "w");
    if (!fp) {
        perror("Could not open output file");
        return 5;
    }

    // Write the Power Quality Analysis report to the file
    calculations(myRecords, alive, fp);

    fclose(fp);
    printf("Data written to WriteTo.txt\n");
    return 0;
}

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;
    welcome();

    PowerData myRecords[1000];
    int alive = collect("ReadFrom.csv", myRecords, 1000);

    if (alive <= 0) {
        printf("No data found or error reading file.\n");
        return 2;
    }

    calculations(myRecords, alive, stdout);
    write_data_file(myRecords, alive);

    return 0;
}