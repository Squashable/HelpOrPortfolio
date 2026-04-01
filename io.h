#ifndef IO_H
#define IO_H  // Removed the space and the dot

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

int collect(const char *filename, PowerData *data_array, int max_records);
#endif