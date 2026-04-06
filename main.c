#include "waveform.h"
#include <stdio.h>

int welcome(void) {
    printf("*******************************\n");
    printf("*    ELECTRICAL CALCULATOR    *\n");
    printf("*       Power Summary         *\n");
    printf("*      please work :|         *\n");
    printf("*******************************\n");
    return 0;
}

int main(void) {
    welcome();

    // Print to terminal
    if (analyse("ReadFrom.csv", stdout) != 0) {
        printf("Error analysing data.\n");
        return 2;
    }

    // Write to file
    FILE *fp = fopen("WriteTo.txt", "w");
    if (!fp) {
        perror("Could not open output file");
        return 5;
    }
    analyse("ReadFrom.csv", fp);
    fclose(fp);
    printf("\nData written to WriteTo.txt\n");

    return 0;
}