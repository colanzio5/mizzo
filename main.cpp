/**
 * COLIN CASAZZA
 * CS570 - OPERATING SYSTEMS
 * CSSC1070
 * ASSIGNMENT FOUR
 * 4/10/19
 */
#include "mizzo.h"


void *fish_produce(void *ctx)
{
    CTX* context = (CTX*)ctx;
    while (1)
    {
        sleep(1);
        sem_wait(&context->empty);
        pthread_mutex_lock(&context->mutex);
        int item = 1;
        context->index = context->index +1;
        context->buffer[context->index] = 1;
        std::cout << "Produced Fish" << std::endl;
        pthread_mutex_unlock(&context->mutex);
        sem_post(&context->full);
    }
}

void *escargot_produce(void *ctx)
{
    CTX* context = (CTX*)ctx;
    while (1)
    {
        sleep(1);
        sem_wait(&context->empty);
        pthread_mutex_lock(&context->mutex);
        context->index = context->index +1;
        context->buffer[context->index] = 2;
        std::cout << "Produced Escargot" << std::endl;
        pthread_mutex_unlock(&context->mutex);
        sem_post(&context->full);
    }
}

void *ethel_consume(void *ctx)
{
    CTX* context = (CTX*)ctx;
    while (1)
    {
        sleep(1);
        sem_wait(&context->full);
        pthread_mutex_lock(&context->mutex);
        context->index = context->index -1;
        int item = context->buffer[context->index];
        std::cout << "Ethel Consumed " << item << std::endl;
        pthread_mutex_unlock(&context->mutex);
        sem_post(&context->empty);
    }
}

void *lucy_consume(void *ctx)
{
    CTX* context = (CTX*)ctx;
    while (1)
    {
        sleep(1);
        sem_wait(&context->full);
        pthread_mutex_lock(&context->mutex);
        context->index = context->index -1;
        int item = context->buffer[context->index];
        std::cout << "Lucy Consumed " << item << std::endl;
        pthread_mutex_unlock(&context->mutex);
        sem_post(&context->empty);
    }
}

int main()
{
    
    // init thread ids and shared data
    pthread_t fish_producer, escargot_producer, ethel_consumer, lucy_consumer;
    CTX context;
    sem_init(&context.empty, 0, BUFFER_SIZE);
    sem_init(&context.full, 0, 0);
    pthread_mutex_init(&context.mutex, NULL);

    // create the producer threads
    pthread_create(&fish_producer, NULL, fish_produce, &context);
    pthread_create(&escargot_producer, NULL, escargot_produce, &context);

    // create the consumer threads
    pthread_create(&ethel_consumer, NULL, ethel_consume, &context);
    pthread_create(&lucy_consumer, NULL, lucy_consume, &context);

    // kill all the threads
    pthread_exit(NULL);
}