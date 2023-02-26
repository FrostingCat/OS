#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <time.h>
#include <error.h>

void sys_err (char *msg) {
  puts (msg);
  exit (1);
}

int main(){
    int shm_id, shm_id_finish;
    int *share, *share_i;
    int num;   
    int kol;

    printf("Введите кол-во генерируемых чисел\n");
    scanf("%d", &kol);
    
    srand(time(NULL));
    shm_id = shmget (0x2FF, getpagesize(), 0666 | IPC_CREAT);
    shm_id_finish = shmget (0x2FA, getpagesize(), 0666 | IPC_CREAT);
    if (shm_id < 0 || shm_id_finish < 0) {
        perror("shmget()");
        exit(1);
    }
    
    share = (int *)shmat(shm_id, 0, 0);
    share_i = (int *)shmat(shm_id_finish, 0, 0);
    
    if (share == NULL || share_i == NULL) {
        perror("shmat()");
        exit(2);
    }
    
    for (int i = 0; i < kol; ++i) {
        num = random() % 1000;
        *share = num;
        *share_i = kol - i;
        printf("write a random number %d\n", num);
        sleep(1);
    }

    shmdt (share);
    shmdt (share_i);
    if (shmctl (shm_id, IPC_RMID, (struct shmid_ds *) 0) < 0) {
        sys_err ("eraser: shared memory remove error");
    } else if (shmctl (shm_id_finish, IPC_RMID, (struct shmid_ds *) 0) < 0) {
        sys_err ("eraser: shared memory remove error");
    }

    printf("eraser: shared memory using key = %x deleted\n", 0x2FF);
    printf("eraser: shared memory using key = %x deleted\n", 0x2FA);
    return 0;
}
