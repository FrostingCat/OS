#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <error.h>

int main() {
    int shm_id, shm_id_finish;
    int *share, *share_i;
    
    shm_id = shmget (0x2FF, getpagesize(), 0666 | IPC_CREAT);
    shm_id_finish = shmget (0x2FA, getpagesize(), 0666 | IPC_CREAT);
    if (shm_id == -1 || shm_id_finish == -1) {
        perror("shmget()");
        exit(1);
    }
    
    share = (int *)shmat(shm_id, 0, 0);
    share_i = (int *)shmat(shm_id_finish, 0, 0);
    if (share == NULL || share_i == NULL) {
        perror("shmat()");
        exit(2);
    }

    int kol = *share_i;
    
    for (int i = 0; i < kol - 1; ++i) {
        sleep(1);
        printf("%d\n", *share);
    }
    
    return 0;
}
