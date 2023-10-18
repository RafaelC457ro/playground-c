#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *myTurn(void *arg)
{
    int *iptr = (int *)malloc(sizeof(int));

    for (int i = 0; i < 10; i++)
    {
        printf("your turn %d: %d\n", i, *iptr);
        sleep(1);
        (*iptr)++;
    }
    return iptr;
}

void yourTurn()
{
    for (int i = 0; i < 3; i++)
    {
        printf("my turn %d\n", i);
        sleep(1);
    }
}

int main(int argc, char **argv)
{
    pthread_t newthead;
    int *ret;

    if (pthread_create(&newthead, NULL, myTurn, NULL))
    {
        perror("pthread_create() error");
        exit(1);
    }

    yourTurn();
    if (pthread_join(newthead, (void *)&ret) != 0)
    {
        perror("pthread_create() error");
        exit(3);
    }

    printf("Value: %d\n", *ret);
    free(ret);
    exit(0);
}