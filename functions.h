#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void printRandoms(int lower, int upper,
                  int count)
{
    int i;
    for (i = 0; i < count; i++)
    {
        int num = (rand() %
                   (upper - lower + 1)) +
                  lower;
        printf("%d ", num);
    }
}

int generateRandom(int lower, int upper)
{
    int num = (rand() %
               (upper - lower + 1)) +
              lower;
    return num;
}