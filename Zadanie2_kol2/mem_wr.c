#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_INT 2

int main() {
    key_t key = ftok("file", 65);

    int shmid = shmget(key, MAX_INT * sizeof(int), 0777 | IPC_CREAT);
    if (shmid == -1) {
        perror("failed to create shared memory segment\n");
        _exit(1);
    }

    int *numbers = (int*) shmat(shmid, NULL, 0);
    if (numbers == NULL) {
        perror("failed to attach shared memory segment\n");
        _exit(1);
    }

    for (int i = 0; i < 1000000; ++i) {
        if (i % 2 == 0) {
            numbers[i % MAX_INT] = 0;
        } else {
            numbers[i % MAX_INT] = 1;
        }
    }

    shmdt(numbers);
    
    return 0;
}