#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *myTurn(void *arg)
{
    for (int i = 0; i < 10; i++)
    {
        printf("your turn %d\n", i);
        sleep(1);
    }
    return NULL;
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
    void *ret;

    if (pthread_create(&newthead, NULL, myTurn, "thread 1"))
    {
        perror("pthread_create() error");
        exit(1);
    }

    yourTurn();
    if (pthread_join(newthead, NULL) != 0)
    {
        perror("pthread_create() error");
        exit(3);
    }
    exit(0);
}