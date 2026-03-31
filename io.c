#include "io.h"
#include <string.h>
#include <stdio.h>

int collect(const char *filename, PowerData *data_array, int max_records) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Could not open file");
        return 1;
    }

    char line[1024];
    int count = 0;

    // Skip the header line
    if (fgets(line, sizeof(line), fp) == NULL) {
        fclose(fp);
        return 0;
    }

    // Read each line and parse using sscanf (comma-separated)
    while (fgets(line, sizeof(line), fp) && count < max_records) {
        int fields = sscanf(line, "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf",
            &data_array[count].timestamp,
            &data_array[count].phase_A_voltage,
            &data_array[count].phase_B_voltage,
            &data_array[count].phase_C_voltage,
            &data_array[count].line_current,
            &data_array[count].frequency,
            &data_array[count].power_factor,
            &data_array[count].thd_percent);

        if (fields == 8) {
            count++;
        }
    }

    fclose(fp);
    return count;
}