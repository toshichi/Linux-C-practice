#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
void ws(int num, int pP[]){
	close(pP[0]);	// disable read
	char str[100];
	sprintf(str, "Child %d is sending a message.\n", num);
	write(pP[1], str, strlen(str)+1);
}

void rs(int pP[]){
	char buf[100];
	close(pP[1]);	// disable write
	read (pP[0], buf, 100);
	printf("%s", buf);
}

int main (){
	int pP[2];
	pipe(pP);	// create pipe
	if (fork() == 0) {
		// child process
		ws(1, pP);
		exit(0);
	}
	else if (fork() == 0){
		// child process
		ws(2, pP);
		exit(0);
	}
	else{
		rs(pP);
		rs(pP);
		exit(0);
	}
}