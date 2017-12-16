#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main() {
    char buf[1000]; // reading buffer
    char* cmd[100]; // command to exec via 'execvp'
    int count = 0;  // command spliting counter
    const char d = ' '; // command splitter
    char *p;    // splitted string pointer
    // read command
    fgets(buf ,1000, stdin);
    
    // split command and arguments
    p = strtok(buf, &d);
    while(p) {
        cmd[count] = p;
        count++;
        p = strtok(NULL, &d);
    }
    // add NULL at the end of command
    if (cmd[count - 1] == "\n")
        cmd[count - 1] = NULL;
    else
        cmd[count] = NULL;

    if(fork()==0){
        // use sub process to execute
        execvp(cmd[0], cmd);
    }
    else {
        // display return code
        int ret;
        wait(&ret);
        printf("returned %d\n", ret);
    }
    return 0;
}