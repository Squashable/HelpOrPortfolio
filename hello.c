#include <stdio.h>
int main() {
    char name[20];
    char studentID[20];
    printf("Student's name: ");
    scanf("%s", name);

    printf("Student's ID: ");
    scanf("%s", studentID);

    printf("hello there, my name is %s and my ID is %s.\n", name, studentID );
}