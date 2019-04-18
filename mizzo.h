
#ifndef MIZZO_H
#define MIZZO_H

// librarys used
#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <queue>
#include <unistd.h>

// constants
#define BUFFER_SIZE 10
#define MAX_ITEMS 100
#define MAX_FROGS 3
#define ESCARGOT_PRODUCE_WAIT 1
#define FROG_PRODUCE_WAIT 1
#define ETHEL_WAIT 1
#define LUCY_WAIT 1

// shared data between consumers and producers
typedef struct CTX
{
    // variables to ensure safe threading
    sem_t full, empty;
    pthread_mutex_t mutex;

    // config variables
    float frog_produce_wait = FROG_PRODUCE_WAIT;
    float escargor_produce_wait = ESCARGOT_PRODUCE_WAIT;
    float ethel_consume_wait = ETHEL_WAIT;
    float lucy_consume_wait = LUCY_WAIT;

    // shared data variables
    std::queue<int> buffer;
    int index = 0;

    // counts for producers and consumers
    int frogs = 0;
    int escargot = 0;
    int ethelFrog = 0;
    int ethelEscargot = 0;
    int lucyFrog = 0;
    int lucyEscargot = 0;

} CTX;

// util functions
void debug(CTX *ctx);
int total_produced(CTX *ctx);
int total_consumed(CTX *ctx);
int current_frogs(CTX *ctx);

// main application functiin
int mizzo(int argc, char* argv[]);

// helper function for parsing args into context
void *init_args(int argc, char **argv, CTX *ctx);

// producer functins
void *frog_produce(void *arg);
void *escargot_produce(void *arg);

// consumer functions
void *ethel_consume(void *arg);
void *lucy_consume(void *arg);

#endif // MIZZO_H