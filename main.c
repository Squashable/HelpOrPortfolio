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

static int write_data_file(const PowerData *myRecords, int alive) {
    FILE *fp = fopen("WriteTo.txt", "w");
    if (!fp) {
        perror("Could not open output file");
        return 5;
    }

    // write to .txt file
    fprintf(fp, "timestamp,phase_A_voltage,phase_B_voltage,phase_C_voltage,line_current,frequency,power_factor,thd_percent\n");

    // Write each record
    for (int i = 0; i < alive; i++) {
        fprintf(fp, "%f,%f,%f,%f,%f,%f,%f,%f\n",
            myRecords[i].timestamp,
            myRecords[i].phase_A_voltage,
            myRecords[i].phase_B_voltage,
            myRecords[i].phase_C_voltage,
            myRecords[i].line_current,
            myRecords[i].frequency,
            myRecords[i].power_factor,
            myRecords[i].thd_percent);
    }

    fclose(fp);
    printf("Data written to WriteTo.csv\n");
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

    calculations(myRecords, alive);
    write_data_file(myRecords, alive);

    return 0;
}
