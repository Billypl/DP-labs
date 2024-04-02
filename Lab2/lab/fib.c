#include "NWD.h"
#include <stdbool.h>
#include <stdio.h>

void print_fibonacci(int n) 
{
    int first = 0, second = 1, next;
    printf("%d, %d, ", first, second);

    next = first + second;
    while (next <= n) 
    {
        printf("%d, ", next);
        first = second;
        second = next;
        next = first + second;
    }
    printf("\b\b \n");
}
