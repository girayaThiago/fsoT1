#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>


//const int MSGSIZE = 10; // error: redefining array size during execution <<
#define MSGSIZE 10

struct msgbuf {
    long mtype;       /* message type, must be > 0 */
    char mtext[MSGSIZE];    /* message data */
};

int main() {
    // colinha do chmod
    // permissions
    // own grp all
    // rwx rwx rwx
    // 421 421 421
    // rw- --- ---
    // 6   0   0
    // rwx r-- r--
    // 7   4   4
    int msgid = msgget(1234, IPC_CREAT | 0600);
    if (msgid == -1) {
        perror("msgget failed");
        return 0;
    }
    pid_t pids[10];
    pid_t pid;
    int i;
    int status;
    for (int i = 0; i < 10; i++){
        pid = fork();
        if (pid == 0){
            break;
        } 
        pids[i] = pid;
    }

    if (pid == 0) {
        // child
        sleep(60);
        printf("Sou processo filho pid = <%d>\n", getpid());
        struct msgbuf msgp;
        msgp.mtype = 1;
        const char * text = "imprimi!";
        strcpy(msgp.mtext, text);
        status = msgsnd(msgid, &msgp, MSGSIZE, 0);
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
        sleep(22);
        struct msgbuf msgp;
        // int status = msgrcv(msgid, &msgp, MSGSIZE, 1, IPC_NOWAIT);
        for (i = 0; i < 10; i++){
            status = msgrcv(msgid, &msgp, MSGSIZE, 1, 0); // isso aqui é basicamente um lock, execução trava até receber uma mensagem do tipo 1;
            if (status == -1) {
                perror("parent");
            } else {
                printf("%s\n", msgp.mtext);struct msgbuf msgp;
                msgp.mtype = 2;
                const char * text = "ok filho";
                strcpy(msgp.mtext, text);
                int status = msgsnd(msgid, &msgp, MSGSIZE, 0);
                msgsnd(msgid, &msgp, MSGSIZE, 0);
                if (status == -1) { perror("parend"); }
            }
        }
        for (i = 0; i < 10; i++){
            waitpid(pids[i], &status, 0); // se não dermos wait aqui vamos limpar a msg queue e os filhos vão ficar presos aguardando mensagens.

            if (WIFSIGNALED(status)){
                printf("Error\n");
            }
            else if (WEXITSTATUS(status)){
                printf("Exited Normally\n");
            }
        }

        //free msg queue
        status = msgctl(msgid, IPC_RMID, NULL);
        if (status == -1) {
            perror("parent");
        }
    }
}
