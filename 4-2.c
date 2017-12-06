#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/shm.h>
#define KEY 75

struct message {
    long mtype;
    char data[1024];
};

int main() {
    if (fork()) {
        // server, receive message
        struct message* rcvMsg;
        //*rcvMsg = 0;
        int handler = shmget(KEY, sizeof(struct message), IPC_CREAT|0777);
        rcvMsg = (struct message*)shmat(handler, NULL, 0);
        printf("[SInfo]\tReceive share memory %d created.\n", handler);
        //wait for first message
        while(1){
            if (rcvMsg->mtype == 10) break;
            sleep(1);
        }

        while(1){
            if (rcvMsg->mtype != -1){
                printf("[S-rcv]\tMessage received %ld: %s\n", rcvMsg->mtype, rcvMsg->data);
                if (rcvMsg->mtype <= 1) break;
                rcvMsg->mtype = -1;
            }
            sleep(1);
        }
        rcvMsg->mtype = -1; // tell client to exit
        shmdt(rcvMsg);
        printf("[SInfo]\tAll received, server exit.\n");
        

        // wait for sub process
        wait(0);
        shmctl(handler, IPC_RMID, NULL); // release memory
        printf("[Parent]\tAll end.\n");
    }
    else {

        // client, send message
        struct message* sndMsg;
        int handler = shmget(KEY, sizeof(struct message), IPC_CREAT|0777);
        sndMsg = (struct message*)shmat(handler, NULL, 0);
        printf("[CInfo]\tSend share memory %d created.\n", handler);
        for (int i=10;i>=1;i--) {
            sndMsg->mtype = i;
            sprintf(sndMsg->data, "this is message %ld", sndMsg->mtype);
            printf("[C-snd]\tMessage sent %ld: %s\n", sndMsg->mtype, sndMsg->data);
            // wait server to receive
            while(1) {
                if(sndMsg->mtype == -1) break;
                sleep(1);
            }
        }
        shmdt(sndMsg);
        printf("[CInfo]\tAll sent, client exit.\n");
    }
}