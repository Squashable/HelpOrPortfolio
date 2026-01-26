#include <stdio.h>

int main(void) {
    float ms;
    float mph;

    printf("Insert speed (m/s): ");
    scanf("%f", &ms);

    mph = ms * 2.23694;

    printf("%f m/s = %.2f mph\n", ms, mph);

    return 0;
}
