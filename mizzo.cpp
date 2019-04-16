#include "mizzo.h"

void debug(CTX *ctx)
{

    printf("DEBUG -> [ ");
    for(int i = 0; i < BUFFER_SIZE; i++){
        int x = ctx->buffer[i];
        if(x == 2) std::cout << "escargot, ";
        if(x == 1) std::cout << "frog, ";
        else std::cout << "none, ";
    }

    printf("] \n");

}

void *frog_produce(void *ctx)
{
    CTX* context = (CTX*)ctx;
    while (1)
    {
        sem_wait(&context->empty);
        pthread_mutex_lock(&context->mutex);
        printf("starting frog produce...\n");
        printf("before: ");
        debug(context);
        int index_to_add = context->index;
        context->buffer[index_to_add] = 1;
        context->index = index_to_add + 1;
        printf("frog added at index %i\nafter:", index_to_add);
        debug(context);
        printf("\n");
        pthread_mutex_unlock(&context->mutex);
        sem_post(&context->full);
        sleep(FROG_PRODUCE_WAIT);
    }
}

void *escargot_produce(void *ctx)
{
    CTX* context = (CTX*)ctx;
    while (1)
    {
        sem_wait(&context->empty);
        pthread_mutex_lock(&context->mutex);
        printf("starting escargot produce...\n");
        printf("before: ");
        debug(context);
        int index_to_add = context->index;
        context->buffer[index_to_add] = 2;
        context->index = index_to_add + 1;
        printf("escargot added at index %i: ", index_to_add);
        debug(context);
        printf("\n");
        pthread_mutex_unlock(&context->mutex);
        sem_post(&context->full);
        sleep(ESCARGOT_PRODUCE_WAIT);
    }
}

void *ethel_consume(void *ctx)
{
    CTX* context = (CTX*)ctx;
    while (1)
    {
        sem_wait(&context->full);
        pthread_mutex_lock(&context->mutex);
        
        printf("starting ethel consume...\n");
        printf("before: ");
        debug(context);
        context->index = context->index - 1;
        int item = context->buffer[context->index];
        context->buffer[context->index] = 0;
        printf("ethel consumed at index %i: ", context->index);
        debug(context);
        printf("\n");
        pthread_mutex_unlock(&context->mutex);
        sem_post(&context->empty);
        sleep(1);
    }
}

void *lucy_consume(void *ctx)
{
    CTX* context = (CTX*)ctx;
    while (1)
    {
        sem_wait(&context->full);
        pthread_mutex_lock(&context->mutex);
        printf("starting lucy consume...\n");
        printf("before: ");
        debug(context);
        context->index = context->index - 1;
        int item = context->buffer[context->index];
        printf("lucy consumed at index %i: ", context->index);
        debug(context);
        printf("\n");
        pthread_mutex_unlock(&context->mutex);
        sem_post(&context->empty);
        sleep(1);
    }
}

int mizzo() 
{
    // init thread ids and shared data
    pthread_t frog_producer, escargot_producer, ethel_consumer, lucy_consumer;
    CTX context;
    sem_init(&context.empty, 0, BUFFER_SIZE);
    sem_init(&context.full, 0, 0);
    pthread_mutex_init(&context.mutex, NULL);

    // create the producer threads
    pthread_create(&frog_producer, NULL, frog_produce, &context);
    pthread_create(&escargot_producer, NULL, escargot_produce, &context);

    // create the consumer threads
    pthread_create(&ethel_consumer, NULL, ethel_consume, &context);
    pthread_create(&lucy_consumer, NULL, lucy_consume, &context);

    // kill all the threads
    pthread_exit(NULL);
    return 1;
}
