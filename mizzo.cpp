/**
 * COLIN CASAZZA
 * CS570 - OPERATING SYSTEMS
 * CSSC1070
 * ASSIGNMENT FOUR
 * 4/10/19
 */
#include "mizzo.h"

int buffer[BUFFER_SIZE] = {0};
int index = 0;
sem_t full, empty;
pthread_mutex_t mutex;

void *fish_produce(void *arg)
{
    while (1)
    {
        sleep(1);
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        int item = 1;
        buffer[index++] = 1;
        std::cout << "Produced Fish" << std::endl;
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }
}

void *escargot_produce(void *arg)
{
    while (1)
    {
        sleep(1);
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        buffer[index++] = 2;
        std::cout << "Produced Escargot" << std::endl;
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }
}

void *ethel_consume(void *arg)
{
    while (1)
    {
        sleep(1);
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        int item = buffer[--index];
        std::cout << "Ethel Consumed " << item << std::endl;
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    }
}

void *lucy_consume(void *arg)
{
    while (1)
    {
        sleep(1);
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        int item = buffer[--index];
        std::cout << "Lucy Consumed " << item << std::endl;
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    }
}

int main()
{
    
    // init data
    pthread_t fish_producer, escargot_producer, ethel_consumer, lucy_consumer;
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    // create the producers
    pthread_create(&fish_producer, NULL, fish_produce, NULL);
    pthread_create(&escargot_producer, NULL, escargot_produce, NULL);

    // create the consumers
    pthread_create(&ethel_consumer, NULL, ethel_consume, NULL);
    pthread_create(&lucy_consumer, NULL, lucy_consume, NULL);

    pthread_exit(NULL);
}