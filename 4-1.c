#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <sys/wait.h>
#define KEY 75

struct message {
    long mtype;
    char data[1024];
};

int main() {
    int flag;
    msgctl(KEY, IPC_RMID, NULL);
    if (fork()) {
        // server, receive message
        struct message rcvMsg;
        rcvMsg.mtype = 0;
        // create IPC
        int handler = msgget(KEY, IPC_CREAT|0777);
        printf("[SInfo]\tReceive IPC %d created.\n", handler);
        // ready to receive message
        while(1){
            // read message
            flag = msgrcv(handler, (void *)&rcvMsg, sizeof(struct message) - sizeof(long), 0, 0);
            printf("[S%d]\tMessage received %ld: %s\n", flag, rcvMsg.mtype, rcvMsg.data);
            // if last message, break
            if (rcvMsg.mtype <= 1) break;
        }
        printf("[SInfo]\tAll received, server exit.\n");
        // remove message path
        msgctl(KEY, IPC_RMID, NULL);

        // wait for sub process
        wait(0);
        printf("[Parent]\tAll end.\n");
    }
    else {
        // client, send message
        struct message sndMsg;
        // create IPC
        int handler = msgget(KEY, IPC_CREAT|0777);
        printf("[CInfo]\tSend IPC %d created.\n", handler);
        // send 10 messaages
        for (int i=10;i>=1;i--) {
            sndMsg.mtype = i;
            sprintf(sndMsg.data, "this is message %ld", sndMsg.mtype);
            flag = msgsnd(handler, (void *)&sndMsg, sizeof(struct message) - sizeof(long), 0);
            printf("[C%d]\tMessage sent %ld: %s\n", flag, sndMsg.mtype, sndMsg.data);
        }
        printf("[CInfo]\tAll sent, client exit.\n");
        // remove IPC
        msgctl(KEY, IPC_RMID, NULL);
    }
}