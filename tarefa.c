#pragma once
#define _CRT_SECURE_NO_WARNINGS 1
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
//define quantidade de threads
#define NUM_THREADS 4

void* printID(void* threadID)
{
	int id = (int)threadID;
	//print de seu ID
	printf("THREAD #%d \n",id);
	pthread_exit((void*)NULL); //saída da thread
}

int main() {
	pthread_t tid[NUM_THREADS]; //Estrutura que define thread //

	int i,rc;
	
	for (i = 0; i <= NUM_THREADS; i++) {

		rc=pthread_create(&tid[i], NULL, printID, (void*)i);
		if (rc) {
			printf("Erro %d \n", rc);
			exit(-1);
		}
	}

	for (i = 0; i <= NUM_THREADS; i++) {
		pthread_join(tid[i], NULL);
	}
	
	system("pause");

	pthread_exit((void*)NULL);
}