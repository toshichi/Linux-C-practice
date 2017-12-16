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
        // create shared memory
        int handler = shmget(KEY, sizeof(struct message), IPC_CREAT|0777);
        rcvMsg = (struct message*)shmat(handler, NULL, 0);
        printf("[SInfo]\tReceive share memory %d created.\n", handler);
        //wait for first message
        while(1){
            if (rcvMsg->mtype == 10) break;
            sleep(1);
        }
        // receive messages untill received type 1 message
        while(1){
            if (rcvMsg->mtype != -1){   // if the client has sent something
                printf("[S-rcv]\tMessage received %ld: %s\n", rcvMsg->mtype, rcvMsg->data);
                if (rcvMsg->mtype <= 1) break;  // breaks when receive type 1
                rcvMsg->mtype = -1; // set msg type to -1 to tell client msg has been read
            }
            sleep(1);   // sleep incase of stuck
        }
        rcvMsg->mtype = -1; // tell client to exit
        shmdt(rcvMsg);  // remove shared memory pointer
        printf("[SInfo]\tAll received, server exit.\n");
        

        // wait for sub process
        wait(0);
        shmctl(handler, IPC_RMID, NULL); // release memory
        printf("[Parent]\tAll end.\n");
    }
    else {

        // client, send message
        struct message* sndMsg;
        // create shared memory
        int handler = shmget(KEY, sizeof(struct message), IPC_CREAT|0777);
        sndMsg = (struct message*)shmat(handler, NULL, 0);
        printf("[CInfo]\tSend share memory %d created.\n", handler);
        // send 10 messages
        for (int i=10;i>=1;i--) {
            sndMsg->mtype = i;
            sprintf(sndMsg->data, "this is message %ld", sndMsg->mtype);
            printf("[C-snd]\tMessage sent %ld: %s\n", sndMsg->mtype, sndMsg->data);
            // wait server to receive
            while(1) {
                // msg type set to -1 means server finished reading
                if(sndMsg->mtype == -1) break;
                sleep(1);
            }
        }
        // remove shared memory pointer
        shmdt(sndMsg);
        printf("[CInfo]\tAll sent, client exit.\n");
    }
}