#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <random>
#include <unistd.h>

#ifndef MIZZO_H
#define MIZZO_H

#define BUFFER_SIZE 10

void *fish_produce(void *arg);
void *escargot_produce(void *arg);
void *ethel_consume(void *arg);
void *lucy_consume(void *arg);

typedef struct CTX {
    int buffer[BUFFER_SIZE] = {0};
    int index = 0;
    sem_t full, empty;
    pthread_mutex_t mutex;
} CTX;

#endif // MIZZO_H