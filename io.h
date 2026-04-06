#ifndef IO_H
#define IO_H

#include <stdio.h>

typedef struct {
    double timestamp;
    double phase_A_voltage;
    double phase_B_voltage;
    double phase_C_voltage;
    double line_current;
    double frequency;
    double power_factor;
    double thd_percent;
} PowerData;

typedef struct {
    FILE *fp;
} CsvReader;

CsvReader *csv_open (const char *filename);
int        csv_next (CsvReader *reader, PowerData *out);
void       csv_close(CsvReader *reader);

#endif