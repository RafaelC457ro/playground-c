#include <stdio.h>
#include <stdlib.h>

int *random_int()
{
    int *r = malloc(sizeof(int));
    // // seed
    // srand(time(NULL));
    // *r = rand();
    return r;
}

int main()
{
    int *r = random_int();

    free(r);
}
