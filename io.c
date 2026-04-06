#include "io.h"
#include <stdio.h>

int csv_open(CsvReader *reader, const char *filename) {
    reader->fp = fopen(filename, "r");
    if (!reader->fp) {
        perror("Could not open file");
        return 0;
    }
    char line[256];
    // Skip header line
    if (fgets(line, sizeof(line), reader->fp) == NULL) {
        fclose(reader->fp);
        return 0;
    }
    return 1;
}

int csv_next(CsvReader *reader, PowerData *out) {
    char line[256];
    while (fgets(line, sizeof(line), reader->fp)) {
        int fields = sscanf(line, "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf",
            &out->timestamp,
            &out->phase_A_voltage,
            &out->phase_B_voltage,
            &out->phase_C_voltage,
            &out->line_current,
            &out->frequency,
            &out->power_factor,
            &out->thd_percent);
        if (fields == 8) return 1;
    }
    return 0;
}

void csv_close(CsvReader *reader) {
    if (reader->fp) {
        fclose(reader->fp);
        reader->fp = NULL;
    }
}