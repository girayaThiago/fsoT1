# fsoT1
trabalho 1 de Fso grupo 9

Implementação de sistema de mensageria entre processos.

O objetivo deste trabalho era fazer uso da biblioteca de mensageria das bibliotecas <sys/ipc.h> e <sys/msg.h>.

Tinhamos como objetivo evidenciar a criação do canal de comunicação e a troca de mensagens, isso pode ser feito através do uso dos comandos ipcs e ps como mostrado abaixo:

```
➜  fsoT1 git:(main) ✗ ipcs
------ Message Queues --------
key        msqid      owner      perms      used-bytes   messages    
0x000004d2 12         owner      600        0            0       

➜  fsoT1 git:(main) ✗ ipcs
------ Message Queues --------
key        msqid      owner      perms      used-bytes   messages    
0x000004d2 id         owner      600        200          20       

```
e
```
➜  fsoT1 git:(main) ✗ ps
  PID TTY          TIME CMD
11958 pts/4    00:00:00 zsh
12771 pts/4    00:00:00 a.out
12774 pts/4    00:00:00 a.out
12775 pts/4    00:00:00 a.out
12776 pts/4    00:00:00 a.out
12777 pts/4    00:00:00 a.out
12778 pts/4    00:00:00 a.out
12779 pts/4    00:00:00 a.out
12780 pts/4    00:00:00 a.out
12781 pts/4    00:00:00 a.out
12782 pts/4    00:00:00 a.out
12783 pts/4    00:00:00 a.out
12840 pts/4    00:00:00 ps

[1]  + 12771 done       ./a.out
➜  fsoT1 git:(main) ✗ ps
  PID TTY          TIME CMD
11958 pts/4    00:00:00 zsh
12984 pts/4    00:00:00 ps
```