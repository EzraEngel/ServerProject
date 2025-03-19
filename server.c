#include <stdio.h>
#include <pthread.h>
#include <unistd.h>


#define NUM_THREADS 8

typedef struct {
    int thread_num;
    pthread_mutex_t lock;
    pthread_cond_t cond;
} ThreadData;

void *runThread(void *arg) {
    //First we start the thread up.
    ThreadData *data = (ThreadData *) arg;
    printf("Hi, I'm thread number %i.\n", data -> thread_num);

    //Then We wait for the signal and exit.
    pthread_mutex_lock(&data->lock);
    pthread_cond_wait(&data->cond, &data->lock);
    pthread_mutex_unlock(&data->lock);
    printf("Shutting down...\n");
    return NULL;
}

int main() {
    // First, we'll initiate the data structures.
    pthread_t thread_list[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];

    // Now, we start our threads and initiate mutexes and conditions.
    for (int i=0; i<NUM_THREADS; i++) {
        thread_data[i].thread_num = i;
        pthread_mutex_init(&thread_data[i].lock, NULL);
        pthread_cond_init(&thread_data[i].cond, NULL);
        pthread_create(&thread_list[i], NULL, runThread, &thread_data[i]);
    }

    // Signal each thread to stop.
    for (int i=0; i<NUM_THREADS; i++) {
        sleep(1);
        printf("Shutting down thread %i.\n", i);
        pthread_mutex_lock(&thread_data[i].lock);
        pthread_cond_signal(&thread_data[i].cond);
        pthread_mutex_unlock(&thread_data[i].lock);
    }

    for (int i=0; i<NUM_THREADS; i++) {
        pthread_join(thread_list[i], NULL);
        pthread_mutex_destroy(&thread_data[i].lock);
        pthread_cond_destroy(&thread_data[i].cond);
    }
    return 0;
}