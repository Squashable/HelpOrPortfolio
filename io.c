#include "io.h"
#include <stdio.h>
#include <stdlib.h>

CsvReader *csv_open(const char *filename) {
    CsvReader *reader = malloc(sizeof(CsvReader));
    if (!reader) {
        fprintf(stderr, "Error: could not allocate CsvReader\n");
        return NULL;
    }

    reader->fp = fopen(filename, "r");
    if (!reader->fp) {
        perror("Could not open file");
        free(reader);
        return NULL;
    }

    // Skip header line
    char line[256];
    if (fgets(line, sizeof(line), reader->fp) == NULL) {
        fclose(reader->fp);
        free(reader);
        return NULL;
    }

    return reader;
}

// Return values:
//  1 = record read successfully
//  0 = end of file, clean exit
// -1 = read error
int csv_next(CsvReader *reader, PowerData *out) {
    char line[256];
    while (1) {
        if (fgets(line, sizeof(line), reader->fp) == NULL) {
            if (feof(reader->fp))   return 0;
            if (ferror(reader->fp)) return -1;
        }
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
        // fields != 8 means malformed line, skip and try next
    }
}

void csv_close(CsvReader *reader) {
    if (reader) {
        if (reader->fp) {
            fclose(reader->fp);
            reader->fp = NULL;
        }
        free(reader);
    }
}