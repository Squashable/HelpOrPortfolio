#ifndef WAVEFORM_H
#define WAVEFORM_H

#include <stdio.h>
#include "io.h"

int check(int argc, char *argv[]);
int calculations(PowerData *myRecords, int alive, FILE *out);

#endif // WAVEFORM_H