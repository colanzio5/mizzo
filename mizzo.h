
#ifndef MIZZO_H
#define MIZZO_H

// librarys used
#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <queue>
#include <unistd.h>

// constants
#define BUFFER_SIZE 5
#define MAX_ITEMS 10
#define MAX_FROGS 3
#define ESCARGOT_PRODUCE_WAIT 0.5
#define FROG_PRODUCE_WAIT 0.5
#define ETHEL_WAIT 0.9
#define LUCY_WAIT 0.3

// shared data between consumers and producers
typedef struct CTX {
    // frog == 1, escargot == 2
    std::queue<int> buffer;
    int index = 0;
    sem_t full, empty;
    pthread_mutex_t mutex;

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
int mizzo();

// producer functins
void *frog_produce(void *arg);
void *escargot_produce(void *arg);

// consumer functions
void *ethel_consume(void *arg);
void *lucy_consume(void *arg);

#endif // MIZZO_H