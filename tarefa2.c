#pragma once
#define _CRT_SECURE_NO_WARNINGS 1
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>


#define TAM_VETOR 100000
#define NUM_THREADS 10


void* slave(void* arg) {
    typedef struct
    {
        int a[TAM_VETOR];
        int global_index;
        int sum;
        int thread_sum[NUM_THREADS];
        int id;

        //declarando mutex
        pthread_mutex_t mutex1;
    }variables;

    variables* var2 = (variables*)arg;

    int id_thread = var2->id;
    pthread_mutex_unlock(&var2->mutex1);

    int local_index, partial_sum = 0;

    var2->thread_sum[id_thread] = 0;

    do {
        pthread_mutex_lock(&var2->mutex1);
        local_index = var2->global_index;
        var2->global_index++;
        var2->thread_sum[id_thread]++;
        pthread_mutex_unlock(&var2->mutex1);




        if (local_index < TAM_VETOR)
        {
            partial_sum += var2->a[local_index];
        }
    } while (local_index < TAM_VETOR);

    pthread_mutex_lock(&var2->mutex1);
    var2->sum += partial_sum;
    pthread_mutex_unlock(&var2->mutex1);

    return 0;
}

int main() {
    int i;
    typedef struct
    {
        int a[TAM_VETOR];
        int global_index;
        int sum;
        int thread_sum[NUM_THREADS];
        int id;

        //declarando mutex
        pthread_mutex_t mutex1;
    }variables;
    variables var1;

    var1.sum = 0;
    var1.global_index = 0;


    pthread_t thread[NUM_THREADS];
    int id_threads[NUM_THREADS];
    double percent;
    pthread_mutex_init(&var1.mutex1, NULL);

    //populando o vetor
    for (i = 0; i < TAM_VETOR; i++) {
        var1.a[i] = i + 1;
    }

    for (i = 0; i < NUM_THREADS; i++) {

        pthread_mutex_lock(&var1.mutex1);
        var1.id = i;
        if (pthread_create(&thread[i], NULL, slave, &var1) != 0) {
            perror("Pthread_create falhou");
            exit(-1);
        }

    }

    for (i = 0; i < NUM_THREADS; i++) {

        if (pthread_join(thread[i], NULL) != 0) {
            perror("Pthread_join falhou");
            exit(-2);
        }
    }

    for (i = 0; i < NUM_THREADS; i++) {
        percent = (var1.thread_sum[i] * 100.0) / TAM_VETOR;
        printf("o trabalho de thread #%d foi de %d somas [%.2f%%]\n", i, var1.thread_sum[i], percent);
    }
    printf("A soma eh %d \n", var1.sum);
    system("pause");
}
