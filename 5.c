#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

void sig(int signum){
	printf("\nPID:%d, %d received\n", getpid(), signum);
}

int main (){
	if (fork()){
		signal(SIGINT, sig);
		wait(0);
	}
	else {
		while(1);
	}
	return 0;
}