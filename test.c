#include <stdio.h>
#include <stdlib.h>


void averageVector(int* a, int n)
{
    for (int i = 0; i < n; i++)
        a[i] = (a[i] + a[i+1]) / 2;
}

int main(int argc, char* argv[])
{
    int a[20];
    int* b = malloc(10 * sizeof(int));
    for (int i = 0; i < 20; i++)
        a[i] = i*i;
    for (int i = 0; i < 10; i++)
        b[i] = a[i];
    averageVector(b, 10);
    free(b);
}