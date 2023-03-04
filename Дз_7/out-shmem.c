#include<stdio.h>
#include<unistd.h>
#include<sys/shm.h>
#include<stdlib.h>
#include<error.h>
#include <sys/mman.h>
#include <fcntl.h>

int main() {
  int shm_id, shm_id_finish;
  char gen_object[] = "gen-memory", gen_i[] = "elem numbers";

  if ((shm_id = shm_open(gen_object, O_RDWR, 0666)) == -1 || 
       (shm_id_finish = shm_open(gen_i, O_CREAT|O_RDWR, 0666)) == -1) {
    printf("Opening error\n");
    perror("shm_open");
    return 1;
  }

    int *addr = mmap(0, sizeof(int), PROT_WRITE|PROT_READ, MAP_SHARED, shm_id, 0);
    int *addr_i = mmap(0, sizeof(int), PROT_WRITE|PROT_READ, MAP_SHARED, shm_id_finish, 0);
    if (addr == (int*)-1 || addr_i == (int*)-1) {
        printf("Error getting pointer to shared memory\n");
        return 1;
    }

    int kol = *addr_i;
    
    for (int i = 0; i < kol; ++i) {
        sleep(1);
        printf("%d\n", addr[i]);
    }

    if (shm_unlink(gen_object) == -1 || shm_unlink(gen_i) == -1) {
        printf("Shared memory is absent\n");
        perror("shm_unlink");
        return 1;
    }
    
    return 0;
}
