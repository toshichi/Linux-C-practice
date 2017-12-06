#include <stdio.h>
#include <signal.h>

typedef void (*sighandler_t) (int);

sighandler_t sb;
sighandler_t sig(int signum){
	printf("PID:%d, %d received", getpid(), signum);
}

int main (){
	fork();
	signal(SIGINT, sig);
	return 0;
}