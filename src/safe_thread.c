#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define BIG 1000000000UL

uint32_t counter = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *count_to_big(void *arg)
{
    for (uint32_t i = 0; i < BIG; i++)
    {
        pthread_mutex_lock(&lock);
        counter++;
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main(int argc, char **argv)
{
    pthread_t newthead;

    if (pthread_create(&newthead, NULL, count_to_big, NULL))
    {
        perror("pthread_create() error");
        exit(1);
    }

    count_to_big(NULL);
    if (pthread_join(newthead, NULL) != 0)
    {
        perror("pthread_join() error");
        exit(3);
    }

    printf("big number: %d\n", counter);
    pthread_mutex_destroy(&lock);
    exit(0);
}