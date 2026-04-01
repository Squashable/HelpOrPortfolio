#include "waveform.h"
#include <stdio.h>
#include <math.h>

int check(int argc, char *argv[]) {
    PowerData myRecords[1000];

    int alive = collect("ReadFrom.csv", myRecords, 1000);

    if (alive > 0) {
        printf("Successfully read %d records.\n", alive);
        for (int i = 0; i < 1; i++) {
            printf("Record %d:\n", i);
            printf("  timestamp:       %f\n", myRecords[i].timestamp);
            printf("  phase_A_voltage: %f\n", myRecords[i].phase_A_voltage);
            printf("  phase_B_voltage: %f\n", myRecords[i].phase_B_voltage);
            printf("  phase_C_voltage: %f\n", myRecords[i].phase_C_voltage);
            printf("  line_current:    %f\n", myRecords[i].line_current);
            printf("  frequency:       %f\n", myRecords[i].frequency);
            printf("  power_factor:    %f\n", myRecords[i].power_factor);
            printf("  thd_percent:     %f\n", myRecords[i].thd_percent);
        }
    } else {
        printf("No data found or error reading file.\n");
        return 2;
    }

    return 0;
}
int calculations(PowerData *myRecords, int alive) {
    double sum_sq_A = 0.0, sum_sq_B = 0.0, sum_sq_C = 0.0;
    double sum_v_A  = 0.0, sum_v_B  = 0.0, sum_v_C  = 0.0;
    double vA_max = myRecords[0].phase_A_voltage, vA_min = myRecords[0].phase_A_voltage;
    double vB_max = myRecords[0].phase_B_voltage, vB_min = myRecords[0].phase_B_voltage;
    double vC_max = myRecords[0].phase_C_voltage, vC_min = myRecords[0].phase_C_voltage;
    double freq_min = myRecords[0].frequency,     freq_max = myRecords[0].frequency;
    double pf_min   = myRecords[0].power_factor,  pf_max  = myRecords[0].power_factor;
    double thd_min  = myRecords[0].thd_percent,   thd_max = myRecords[0].thd_percent;
    int    clipped  = 0;
    double N = alive;

    for (int i = 0; i < alive; i++) {
        double vA = myRecords[i].phase_A_voltage;
        double vB = myRecords[i].phase_B_voltage;
        double vC = myRecords[i].phase_C_voltage;

        sum_sq_A += vA * vA;
        sum_sq_B += vB * vB;
        sum_sq_C += vC * vC;
        sum_v_A  += vA;
        sum_v_B  += vB;
        sum_v_C  += vC;

        if (vA > vA_max) vA_max = vA;
        if (vA < vA_min) vA_min = vA;
        if (vB > vB_max) vB_max = vB;
        if (vB < vB_min) vB_min = vB;
        if (vC > vC_max) vC_max = vC;
        if (vC < vC_min) vC_min = vC;

        if (fabs(vA) >= 324.9 || fabs(vB) >= 324.9 || fabs(vC) >= 324.9) clipped++;

        if (myRecords[i].frequency    < freq_min) freq_min = myRecords[i].frequency;
        if (myRecords[i].frequency    > freq_max) freq_max = myRecords[i].frequency;
        if (myRecords[i].power_factor < pf_min)   pf_min   = myRecords[i].power_factor;
        if (myRecords[i].power_factor > pf_max)   pf_max   = myRecords[i].power_factor;
        if (myRecords[i].thd_percent  < thd_min)  thd_min  = myRecords[i].thd_percent;
        if (myRecords[i].thd_percent  > thd_max)  thd_max  = myRecords[i].thd_percent;
    }

    double vA_rms = sqrt(sum_sq_A / N);
    double vB_rms = sqrt(sum_sq_B / N);
    double vC_rms = sqrt(sum_sq_C / N);
    double vA_dc  = sum_v_A / N;
    double vB_dc  = sum_v_B / N;
    double vC_dc  = sum_v_C / N;
    double vA_pp  = vA_max - vA_min;
    double vB_pp  = vB_max - vB_min;
    double vC_pp  = vC_max - vC_min;

    printf("\n--- Power Quality Analysis ---\n");
    printf("Phase A RMS: %.1f V", vA_rms);
    if (vA_rms >= 207.0 && vA_rms <= 253.0)
        printf(" (within 207-253 V tolerance band - COMPLIANT)\n");
    else
        printf(" (NON-COMPLIANT)\n");
    printf("Phase A peak-to-peak: %.1f V\n", vA_pp);
    printf("Phase A DC offset: %.2f V\n", vA_dc);

    printf("Phase B RMS: %.1f V", vB_rms);
    if (vB_rms >= 207.0 && vB_rms <= 253.0)
        printf(" (COMPLIANT)\n");
    else
        printf(" (NON-COMPLIANT)\n");
    printf("Phase B peak-to-peak: %.1f V\n", vB_pp);
    printf("Phase B DC offset: %.2f V\n", vB_dc);

    printf("Phase C RMS: %.1f V", vC_rms);
    if (vC_rms >= 207.0 && vC_rms <= 253.0)
        printf(" (COMPLIANT)\n");
    else
        printf(" (NON-COMPLIANT)\n");
    printf("Phase C peak-to-peak: %.1f V\n", vC_pp);
    printf("Phase C DC offset: %.2f V\n", vC_dc);

    printf("Clipped samples (|V| >= 324.9 V, any phase): %d samples total\n", clipped);
    printf("Frequency range: %.3f Hz to %.3f Hz\n", freq_min, freq_max);
    printf("Power factor range: %.3f to %.3f\n", pf_min, pf_max);
    printf("THD range: %.2f%% to %.2f%%\n", thd_min, thd_max);

    return 0;
}
