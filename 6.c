#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void thread1(char* arg) {
	printf("This is thread 1\n%s\n", arg);
	return;
}

void thread2(int* arg) {
	printf("This is thread 2\n%d\n", *arg);
	return;
}

int main (){
	pthread_t tid1, tid2;
	char str[100] = "This is string argument\n";
	int arg = 15071025;
	if (pthread_create(&tid1, NULL, (void *)thread1, str)) {
		printf("Thread 1 create failed!");
	}
	if (pthread_create(&tid2, NULL, (void *)thread2, &arg)) {
		printf("Thread 2 create failed!");
	}
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	return 0;
}