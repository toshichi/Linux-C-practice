#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main() {
	char buf[1000];
	char* cmd[100];
    // const char *d = " ";
    fgets(buf ,1000, stdin) != NULL;
    int count = 0;
    const char d = ' ';
    char *p;
    p = strtok(buf, &d);
    while(p) {
    	cmd[count] = p;
    	count++;
        p = strtok(NULL, &d);
    }
    if (cmd[count - 1] == "\n")
    	cmd[count - 1] = NULL;
    else
    	cmd[count] = NULL;
    // cmd[count] = (cmd[count] == "\n"NULL;

    for (int i=0;i<count ;i++){
    	printf ("%s\n", cmd[i]);
    }

    if(fork()==0){
    	execvp(cmd[0], cmd);
    	// execle
    }
    else {
    	int ret;
    	wait(&ret);
    	printf("returned %d\n", ret);
    }
    return 0;
}