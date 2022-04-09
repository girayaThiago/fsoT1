#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <string.h>


//linhas usadas para executar: gcc -Wall -Wextra -Pedantic 
// acho que tava usando -std=c10
// atualmente o código cria 1 processo filho e 1 pai e eles se comunicam via ipc->message send\ message receive, falta botar 10 e dar wait(pid) nos filhos;
//const int MSGSIZE = 10; // error: redefining array size during execution <<
#define MSGSIZE 10

struct msgbuf {
    long mtype;       /* message type, must be > 0 */
    char mtext[MSGSIZE];    /* message data */
};

int main() {
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

    pid_t pid = fork();

    if (pid == 0) {
        // child
        struct msgbuf msgp;
        msgp.mtype = 1;
        const char * text = "hi! 1";
        strcpy(msgp.mtext, text);
        int status = msgsnd(msgid, &msgp, MSGSIZE, 0);
        msgsnd(msgid, &msgp, MSGSIZE, 0);
        if (status == -1) { perror("child"); }
    } else {
        // parent
        struct msgbuf msgp;
        //int status = msgrcv(msgid, &msgp, MSGSIZE, 1, IPC_NOWAIT);
        int status = msgrcv(msgid, &msgp, MSGSIZE, 1, 0);
        if (status == -1) {
            perror("parent");
        } else {
            printf("%s\n", msgp.mtext);
        }

        //free msg queue
        status = msgctl(msgid, IPC_RMID, NULL);
        if (status == -1) {
            perror("parent");
        }
    }
}

// cmds
// ipcs -> listar os ipcs
// ipcrm -> remover um ipc
