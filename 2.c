#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main (){
	int pP[2];
	pipe(pP);
	if (fork() == 0) {
		close(pP[0]);
		char str[100] = "Child 1 is sending a message.\n";
		write(pP[1], str, strlen(str)+1);
		exit(0);
	}
	else if (fork() == 0){
		close(pP[0]);
		char str[100] = "Child 2 is sending a message.\n";
		write(pP[1], str, strlen(str)+1);
		exit(0);
	}
	else{
		char buf[100];
		close(pP[1]);
		read (pP[0], buf, 100);
		printf("%s", buf);
		read (pP[0], buf, 100);
		printf("%s", buf);
	}
}