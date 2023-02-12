#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

const int size = 32; // Циклическое чтение с использованием системного вызова read
int main(int argc, char *argv[]) {
    int fd, fd1;
    char buffer[size + 1];
    ssize_t read_bytes;
    size_t size1;
    
    if ((fd = open(argv[1], O_RDONLY)) < 0) {
        printf("Can\'t open reading file\n");
        exit(-1);
    }
    if ((fd1 = open(argv[2], O_WRONLY | O_CREAT, 0666)) < 0) {
        printf("Can\'t open writing file\n");
        exit(-1);
    }
    
    do {
        read_bytes = read(fd, buffer, size);
        if (read_bytes == -1) {
            printf("Can\'t write this file\n");
            exit(-1);
        }
        buffer[read_bytes] = '\0';
        size1 = write(fd1, buffer, read_bytes);
    } while (read_bytes == size);
    
    if(close(fd) < 0) {
        printf("Can\'t close reading file\n");
    }
    if (close(fd1) < 0) {
        printf("Can\'t close writing file\n");
    }
    return 0;
}