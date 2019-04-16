
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

#define ESCARGOT_PRODUCE_WAIT 1
#define FROG_PRODUCE_WAIT 1

#define ETHEL_WAIT 1
#define LUCY_WAIT 1

// shared data between consumers and producers
typedef struct CTX {
    int buffer[BUFFER_SIZE] = {0};
    int index = 0;
    sem_t full, empty;
    pthread_mutex_t mutex;
} CTX;

// util functions
void debug(CTX *ctx);

// main application functiin
int mizzo();

// producer functins
void *frog_produce(void *arg);
void *escargot_produce(void *arg);

// consumer functions
void *ethel_consume(void *arg);
void *lucy_consume(void *arg);

#endif // MIZZO_H