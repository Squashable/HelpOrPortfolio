#include "waveform.h"
#include <stdio.h>
#include <math.h>

int analyse(const char *filename, FILE *out) {
    CsvReader reader;
    if (!csv_open(&reader, filename)) return 1;

    double sum_sq_A = 0.0, sum_sq_B = 0.0, sum_sq_C = 0.0;
    double sum_v_A  = 0.0, sum_v_B  = 0.0, sum_v_C  = 0.0;
    double vA_max, vA_min, vB_max, vB_min, vC_max, vC_min;
    double freq_min, freq_max, pf_min, pf_max, thd_min, thd_max;
    int clipped = 0, count = 0;

    PowerData r;
    while (csv_next(&reader, &r)) {
        double vA = r.phase_A_voltage;
        double vB = r.phase_B_voltage;
        double vC = r.phase_C_voltage;

        // Initialise min/max from first valid record
        if (count == 0) {
            vA_max = vA_min = vA;
            vB_max = vB_min = vB;
            vC_max = vC_min = vC;
            freq_min = freq_max = r.frequency;
            pf_min   = pf_max   = r.power_factor;
            thd_min  = thd_max  = r.thd_percent;
        }

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

        if (r.frequency    < freq_min) freq_min = r.frequency;
        if (r.frequency    > freq_max) freq_max = r.frequency;
        if (r.power_factor < pf_min)   pf_min   = r.power_factor;
        if (r.power_factor > pf_max)   pf_max   = r.power_factor;
        if (r.thd_percent  < thd_min)  thd_min  = r.thd_percent;
        if (r.thd_percent  > thd_max)  thd_max  = r.thd_percent;

        count++;
    }

    csv_close(&reader);

    if (count == 0) {
        fprintf(out, "No valid records found.\n");
        return 2;
    }

    double N      = count;
    double vA_rms = sqrt(sum_sq_A / N);
    double vB_rms = sqrt(sum_sq_B / N);
    double vC_rms = sqrt(sum_sq_C / N);
    double vA_dc  = sum_v_A / N;
    double vB_dc  = sum_v_B / N;
    double vC_dc  = sum_v_C / N;
    double vA_pp  = vA_max - vA_min;
    double vB_pp  = vB_max - vB_min;
    double vC_pp  = vC_max - vC_min;

    fprintf(out, "\n --- :) Power Quality Analysis :) ---\n");

    fprintf(out, "Phase A RMS: %.1f V", vA_rms);
    if (vA_rms >= 207.0 && vA_rms <= 253.0)
        fprintf(out, " (within 207-253 V tolerance band - COMPLIANT)\n");
    else
        fprintf(out, " (NON-COMPLIANT)\n");
    fprintf(out, "Phase A peak-to-peak: %.1f V\n", vA_pp);
    fprintf(out, "Phase A DC offset: %.2f V\n", vA_dc);

    fprintf(out, "Phase B RMS: %.1f V", vB_rms);
    if (vB_rms >= 207.0 && vB_rms <= 253.0)
        fprintf(out, " (COMPLIANT)\n");
    else
        fprintf(out, " (NON-COMPLIANT)\n");
    fprintf(out, "Phase B peak-to-peak: %.1f V\n", vB_pp);
    fprintf(out, "Phase B DC offset: %.2f V\n", vB_dc);

    fprintf(out, "Phase C RMS: %.1f V", vC_rms);
    if (vC_rms >= 207.0 && vC_rms <= 253.0)
        fprintf(out, " (COMPLIANT)\n");
    else
        fprintf(out, " (NON-COMPLIANT)\n");
    fprintf(out, "Phase C peak-to-peak: %.1f V\n", vC_pp);
    fprintf(out, "Phase C DC offset: %.2f V\n", vC_dc);

    fprintf(out, "Clipped samples (|V| >= 324.9 V, any phase): %d samples total\n", clipped);
    fprintf(out, "Frequency range: %.3f Hz to %.3f Hz\n", freq_min, freq_max);
    fprintf(out, "Power factor range: %.3f to %.3f\n", pf_min, pf_max);
    fprintf(out, "THD range: %.2f%% to %.2f%%\n", thd_min, thd_max);

    return 0;
}