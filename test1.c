#include <stdio.h>
#include <string.h> 
#include <stdlib.h>

int OpenCSV(void){
    FILE *fp = fopen("ReadFrom.csv", "r");
    if(fp == NULL) {
        printf("Error opening file\n");
        return 1;
    }
}


int main(){
    OpenCSV();
}