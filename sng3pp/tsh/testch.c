#include <stdio.h>

int main(void)
{
    char name[100];

    printf("Enter your name: ");
    // pay attention to the space in front of the %
    //that do all the trick
    scanf(" %[^\n]s", name);
    printf("Your Name is: %s", name);

    return 0;
}
