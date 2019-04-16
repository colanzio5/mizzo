
#ifndef MIZZO_H
#define MIZZO_H

// librarys used
#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <random>
#include <unistd.h>

// constants
#define BUFFER_SIZE 10

// main application functiin
int mizzo();

// producer functins
void *fish_produce(void *arg);
void *escargot_produce(void *arg);

// consumer functions
void *ethel_consume(void *arg);
void *lucy_consume(void *arg);

// shared data between consumers and producers
typedef struct CTX {
    int buffer[BUFFER_SIZE] = {0};
    int index = 0;
    sem_t full, empty;
    pthread_mutex_t mutex;
} CTX;

#endif // MIZZO_H