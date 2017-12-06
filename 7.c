#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

int cache1[100];
int cache2[100];
sem_t sem1;
sem_t sem2;

void read1() {
	FILE *fp = fopen("./7-1.txt", "r");
	int i = 0;
	while(!feof(fp)) {
		fscanf(fp, "%d", &cache1[i]);
		printf("[1]wrote, %d\n", cache1[i]);
		sem_post(&sem1);
		i++;
	}

	cache1[i] = -1;//end mark
	printf("[1]write finished\n");
	sem_post(&sem1);
	return;
}

void read2() {
	FILE *fp = fopen("./7-2.txt", "r");
	int i = 0;
	while(!feof(fp)) {
		fscanf(fp, "%d", &cache2[i]);
		printf("[2]wrote, %d\n", cache2[i]);
		sem_post(&sem2);
		i++;
	}
	cache2[i] = -1;//end mark
	printf("[2]write finished\n");
	sem_post(&sem2);
	return;
}


void calc1() {
	int i = 0;
	int sum = 0;
	while (1) {
		sem_wait(&sem1);
		if (cache1[i] == -1) break;//end mark
		sum+=cache1[i];
		printf("[1]readed: %d\n", cache1[i]);
		i++;
	}
	printf("[1] sum is %d\n", sum);
	return;
}

void calc2() {
	int i = 0;
	int quad = 1;
	while (1) {
		sem_wait(&sem2);
		if (cache2[i] == -1) break;//end mark
		quad*=cache2[i];
		printf("[2]readed: %d\n", cache2[i]);
		i++;
	}
	printf("[2] quad is %d\n", quad);
	return;
}


int main() {
	pthread_t tid1, tid2, tid3, tid4;
	pthread_create(&tid1, NULL, (void *)read1, NULL);
	pthread_create(&tid2, NULL, (void *)read2, NULL);
	pthread_create(&tid3, NULL, (void *)calc1, NULL);
	pthread_create(&tid4, NULL, (void *)calc2, NULL);
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	pthread_join(tid3, NULL);
	pthread_join(tid4, NULL);
	printf("All thread quited.\n");
    return 0;
}