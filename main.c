#include "waveform.h"
#include <stdio.h>

int welcome(void) {
    printf("*******************************\n");
    printf("*    ELECTRICAL CALCULATOR    *\n");
    printf("*       Power Summary         *\n");
    printf("*******************************\n");
    return 0;
}

int main(void) {
    welcome();

    // Print to terminal
    int status = analyse("ReadFrom.csv", stdout);
    if (status != 0) {
        fprintf(stderr, "Analysis failed with error code %d\n", status);
        return status;
    }

    // Write to file
    FILE *fp = fopen("WriteTo.txt", "w");
    if (!fp) {
        perror("Could not open output file");
        return 5;
    }

    status = analyse("ReadFrom.csv", fp);
    fclose(fp);

    if (status != 0) {
        fprintf(stderr, "Failed writing to file, error code %d\n", status);
        return status;
    }

    printf("Data written to WriteTo.txt\n");
    return 0;
}