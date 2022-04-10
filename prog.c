#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <sys/msg.h>
#include <sys/types.h>


//const int MSGSIZE = 10; // error: redefining array size during execution <<
#define MSGSIZE 10

struct msgbuf {
    long mtype;       /* message type, must be > 0 */
    char mtext[MSGSIZE];    /* message data */
};

int main() {
    int msgid = msgget(1234, IPC_CREAT | 0600);
    if (msgid == -1) {
        perror("msgget failed");
        return 0;
    }
    // pid_t pids[10];
    pid_t pid;
    int i;
    for (int i = 0; i < 10; i++){
        pid = fork();
        if (pid == 0){
            break;
        } 
    }

    if (pid == 0) {
        // child
        sleep(60);
        struct msgbuf msgp;
        msgp.mtype = 1;
        const char * text = "hi pops!";
        strcpy(msgp.mtext, text);
        int status = msgsnd(msgid, &msgp, MSGSIZE, 0);
        msgsnd(msgid, &msgp, MSGSIZE, 0);
        if (status == -1) { perror("child"); }
        status = msgrcv(msgid, &msgp, MSGSIZE, 2, 0);
        if (status == -1) {
            perror("parent");
        } else {
            printf("%s\n", msgp.mtext);
        }
    } else {
        // parent
        struct msgbuf msgp;
        // int status = msgrcv(msgid, &msgp, MSGSIZE, 1, IPC_NOWAIT);
        int status = 0;
        for (i = 0; i < 10; i++){
            int status = msgrcv(msgid, &msgp, MSGSIZE, 1, 0); // isso aqui é basicamente um lock, execução trava até receber uma mensagem do tipo 1;
            if (status == -1) {
                perror("parent");
            } else {
                printf("%s\n", msgp.mtext);struct msgbuf msgp;
                msgp.mtype = 2;
                const char * text = "bye filho";
                strcpy(msgp.mtext, text);
                int status = msgsnd(msgid, &msgp, MSGSIZE, 0);
                msgsnd(msgid, &msgp, MSGSIZE, 0);
                if (status == -1) { perror("parend"); }
            }
        }

        //free msg queue
        status = msgctl(msgid, IPC_RMID, NULL);
        if (status == -1) {
            perror("parent");
        }
    }
}
