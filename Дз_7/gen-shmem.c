#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/mman.h>
#include <fcntl.h>

int main(){
    int shm_id, shm_id_finish;
    char gen_object[] = "gen-memory", gen_i[] = "elem numbers";
    int num, kol;

    printf("Введите кол-во генерируемых чисел\n");
    scanf("%d", &kol);

    if ((shm_id = shm_open(gen_object, O_CREAT|O_RDWR, 0666)) == -1 || 
         (shm_id_finish = shm_open(gen_i, O_CREAT|O_RDWR, 0666)) == -1) {
        perror("shm_open");
        return 1;
    }

    if (ftruncate(shm_id, sizeof (num)) == -1 || 
        ftruncate(shm_id_finish, sizeof (num)) == -1) {
        perror("ftruncate");
        return 1;
    }
    
    srand(time(NULL));

    int *addr = mmap(0, sizeof(num), PROT_WRITE|PROT_READ, MAP_SHARED, shm_id, 0);
    int *addr_i = mmap(0, sizeof(num), PROT_WRITE|PROT_READ, MAP_SHARED, shm_id_finish, 0);
    if (addr == (int*)-1 || addr_i == (int*)-1) {
        printf("Error getting pointer to shared memory\n");
        return 1;
    }

    *addr_i = kol;
    
    for (int i = 0; i < kol; ++i) {
        num = random() % 1000;
        addr[i] = num;
        printf("write a random number %d\n", num);
        sleep(1);
    }

    return 0;
}
