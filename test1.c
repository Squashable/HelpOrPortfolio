#include <stdio.h>
#include <string.h> 
#include <stdlib.h>

int main(){
    FILE *fp = fopen("ReadFrom.csv", "r");
    if(fp == NULL) {
        printf("Error opening file\n");
        return 1;
    }
    char line[256];
    fgets(line, sizeof(line), fp);
    while (fgets(line, sizeof(line), fp) != NULL) {
        char *token;
        token = strtok(line, ",");
        float time = atof(token);

        token = strtok(NULL, ",");
        float temperature = atof(token);

        token = strtok(NULL, ",");
        float pressure = atof(token);

        printf("Time =%.1f, temp = %.1f, Pressure = %.1f\n",
            time, temperature, pressure);


    }
    fclose(fp);
    return(0);
}