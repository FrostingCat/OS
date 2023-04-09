#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

const int size = 4;

int main(int argc, char *argv[]) {
    int mem[2];
    pid_t readf;

    pthread_mutex_t mutex;

    pthread_mutex_init(&mutex, 0);

    pipe(mem);
    readf = fork();
    if (readf == 0) {
        for (int i = 0; i < 10; ++i) {
            pthread_mutex_lock(&mutex);
            if (i > 0) {
                int got_n;
                read(mem[0], &got_n, size);
                printf("1 : got number %d. ", got_n);

                if (got_n % 2 == 0) {
                    printf("it is even.\n");
                } else {
                    printf("it is not even.\n");
                }
            }

            srand(time(NULL));
            int number = rand() % 3;
            write(mem[1], &number, size);
            printf("1 : send number %d.\n", number);

            pthread_mutex_unlock(&mutex);
            sleep(1);
        }
        exit(0);
    } else if (readf > 0) {
        for (int i = 0; i < 10; ++i) {
            if (i == 0) {
                sleep(1);
            }
            pthread_mutex_lock(&mutex);

            int got_n;
            read(mem[0], &got_n, size);
            printf("2 : got number %d. ", got_n);
    
            if (got_n % 2 == 0) {
                printf("it is even.\n");
            } else {
                printf("it is not even.\n");
            }
    
            int number = rand() % 3;
            write(mem[1], &number, size);
            printf("2 : send number %d.\n", number);
    
            pthread_mutex_unlock(&mutex);
            sleep(1);
        }

        exit(0);
    } else {
        printf("Can't create a child\n");
        exit(-1);
    }

    pthread_mutex_destroy(&mutex);
    close(mem[0]);
    close(mem[1]);

    return 0;
}