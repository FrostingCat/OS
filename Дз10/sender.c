#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <wait.h>
#include <errno.h>

int i = 128, res, number;
pid_t ppid;
void (*p)(int);

void my_handler(int signum) {
    res = i & number;
    i /= 2;
    if (res == 1) {
        kill(ppid, SIGUSR1);
    } else {
        kill(ppid, SIGUSR2);
    }
    if (i == 0) {
        (void)signal(SIGINT, p);
    }
}

int main(void) {
    pid_t pid = getpid();
    printf("my pid - %d\n", pid);

    scanf("%d%d", &number, &ppid);

    p = signal(SIGINT, my_handler);

    return 0;
}