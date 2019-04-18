#include "mizzo.h"

void debug(CTX *ctx)
{
    std::queue<int> q = ctx->buffer;
    printf("[");
    while (!q.empty() && q.size() > 0)
    {
        int x = q.front();
        std::cout << x << ", ";
        q.pop();
    }
    printf("] \n");
}

int total_produced(CTX *ctx)
{
    return ctx->frogs + ctx->escargot;
}

int total_consumed(CTX *ctx)
{
    return ctx->ethelEscargot + ctx->ethelFrog + ctx->lucyEscargot + ctx->lucyFrog;
}

int current_frogs(CTX *ctx)
{
    int count = 0;
    std::queue<int> q = ctx->buffer;
    while (!q.empty())
    {
        if (q.front() == 1)
            count++;
        q.pop();
    }
    return count;
}

void *frog_produce(void *ctx)
{
    CTX *context = (CTX *)ctx;
    while (total_produced(context) < MAX_ITEMS)
    {
        sleep(context->frog_produce_wait);
        sem_wait(&context->empty);
        pthread_mutex_lock(&context->mutex);
        // start critical section
        if (total_produced(context) < MAX_ITEMS &&
            current_frogs(context) < MAX_FROGS)
        {
            printf("producing frog...\n");
            printf("before: ");
            debug(context);
            context->buffer.push(1);
            context->frogs++;
            printf("after: ");
            debug(context);
            printf("\n");
        }
        // end critical section
        pthread_mutex_unlock(&context->mutex);
        sem_post(&context->full);
    }
}

void *escargot_produce(void *ctx)
{
    CTX *context = (CTX *)ctx;
    while (total_produced(context) < MAX_ITEMS)
    {
        sleep(context->escargor_produce_wait);
        sem_wait(&context->empty);
        pthread_mutex_lock(&context->mutex);
        // start critical section
        if (total_produced(context) < MAX_ITEMS)
        {
            printf("produdcing escargot...\n");
            printf("before: ");
            debug(context);
            context->buffer.push(2);
            context->escargot++;
            printf("after: ");
            debug(context);
            printf("\n");
        }
        // end critical section
        pthread_mutex_unlock(&context->mutex);
        sem_post(&context->full);
    }
}

void *ethel_consume(void *ctx)
{
    CTX *context = (CTX *)ctx;
    while (total_consumed(context) <= MAX_ITEMS - 5)
    {
        sem_wait(&context->full);
        pthread_mutex_lock(&context->mutex);
        // start critical section
        if (context->buffer.size() > 0)
        {
            printf("ethel is consuming...\n");
            printf("before: ");
            debug(context);
            int item = context->buffer.front();
            context->buffer.pop();
            if (item == 1)
                context->ethelFrog++;
            if (item == 2)
                context->ethelEscargot++;

            printf("ethel consumed item %i\nafter: ", item);
            debug(context);
            printf("\n");
            // end critical section
            pthread_mutex_unlock(&context->mutex);
            sem_post(&context->empty);
            sleep(context->ethel_consume_wait);
        }
        else
        {
            pthread_mutex_unlock(&context->mutex);
            sem_post(&context->empty);
        }
    }
}

void *lucy_consume(void *ctx)
{
    CTX *context = (CTX *)ctx;
    while (total_consumed(context) < MAX_ITEMS)
    {
        sem_wait(&context->full);
        pthread_mutex_lock(&context->mutex);
        // start critical section
        if (context->buffer.size() > 0)
        {
            printf("lucy is consuming...\n");
            printf("before: ");
            debug(context);
            int item = context->buffer.front();
            context->buffer.pop();
            if (item == 1)
                context->lucyFrog++;
            if (item == 2)
                context->lucyEscargot++;

            printf("lucy consumed item %i\nafter: ", item);
            debug(context);
            printf("\n");
            // end critical section
            pthread_mutex_unlock(&context->mutex);
            sem_post(&context->empty);
            sleep(context->lucy_consume_wait);
        }
        else
        {
            pthread_mutex_unlock(&context->mutex);
            sem_post(&context->empty);
        }
    }
}

CTX init_args(int argc, char **argv, CTX ctx)
{
    int opt = -1;
    while ((opt = getopt(argc, argv, "E:L:f:e:")) != -1)
    {
        switch (opt)
        {
        case 'E':
            ctx.ethel_consume_wait = atoi(optarg) / 1000;
            break;
        case 'L':
            ctx.lucy_consume_wait = atoi(optarg) / 1000;
            break;
        case 'f':
            ctx.frog_produce_wait = atoi(optarg) / 1000;
            break;
        case 'e':
            ctx.escargor_produce_wait = atoi(optarg) / 1000;
            break;
        default:
            std::cout << "default case\n";
        }
    }
    return ctx;
}

int mizzo(int argc, char *argv[])
{
    CTX context;

    // load in args from argc, argv
    context = init_args(argc, argv, context);

    // init thread ids and shared data
    pthread_t frog_producer, escargot_producer, ethel_consumer, lucy_consumer;
    sem_init(&context.empty, 0, BUFFER_SIZE);
    sem_init(&context.full, 0, 0);
    pthread_mutex_init(&context.mutex, NULL);

    // create the producer threads
    if (pthread_create(&frog_producer, NULL, frog_produce, &context))
    {
        printf("\nerror creating frog producer thread\n");
        exit(1);
    }
    if (pthread_create(&escargot_producer, NULL, escargot_produce, &context))
    {
        printf("\nerror creating escargot producer thread\n");
        exit(1);
    }
    // create the consumer threads
    if (pthread_create(&ethel_consumer, NULL, ethel_consume, &context))
    {
        printf("\nerror creating ethel consumer thread\n");
        exit(1);
    }
    if (pthread_create(&lucy_consumer, NULL, lucy_consume, &context))
    {
        printf("\nerror creating lucy consumer thread\n");
        exit(1);
    }

    // join all the threads to stop
    if (pthread_join(frog_producer, NULL))
    {
        printf("\n error closing the frog producer thread\n");
        exit(1);
    }
    if (pthread_join(escargot_producer, NULL))
    {
        printf("\n error closing the escargot producer thread\n");
        exit(1);
    }
    if (pthread_join(ethel_consumer, NULL))
    {
        printf("\n error closing the ethel consumer thread\n");
        exit(1);
    }
    if (pthread_join(lucy_consumer, NULL))
    {
        printf("\n error closing the lucy consumer thread\n");
        exit(1);
    }

    // print out reports
    printf("production -> frogs: %i, escargot: %i\n",
           context.frogs, context.escargot);
    printf("ethel consumption -> frogs: %i, escargot: %i, total: %i\n",
           context.ethelFrog, context.ethelEscargot,
           context.ethelFrog + context.ethelEscargot);
    printf("lucy consumption -> frogs: %i, escargot: %i, total: %i\n",
           context.lucyFrog, context.lucyEscargot,
           context.lucyFrog + context.lucyEscargot);
    printf("total production:  %i\n", total_produced(&context));
    printf("total consumption: %i\n", total_consumed(&context));

    //clean everything up
    pthread_exit(NULL);
    sem_destroy(&context.full);
    sem_destroy(&context.empty);
    return 1;
}
