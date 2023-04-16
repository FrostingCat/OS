#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <wait.h>
#include <errno.h>

int i = 0;
pid_t ppid;

void (*p)(int);
void (*m)(int);

void sig1(int signum){
    printf("1");
    kill(ppid, SIGINT);
    i++;
    if (i == 7) {
        (void)signal(SIGINT, p);
    }
}

void sig2(int signum){
    printf("0");
    kill(ppid, SIGINT);
    i++;
    if (i == 7) {
        (void)signal(SIGINT, p);
    }
}

int main(void){
    pid_t pid = getpid();
    printf("my pid - %d\n", pid);

    scanf("%d", &ppid);
    kill(ppid, SIGINT);

    p = signal(SIGUSR1, sig1);
    m = signal(SIGUSR2, sig2);

    return 0;
}