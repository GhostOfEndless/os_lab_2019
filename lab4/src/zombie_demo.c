#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void create_zombie() {
    pid_t pid = fork();

    if (pid < 0) {
        // Ошибка при создании процесса
        perror("fork failed");
        exit(1);
    }

    if (pid == 0) {
        // Дочерний процесс
        printf("Child process (PID: %d) is starting...\n", getpid());
        printf("Child process (PID: %d) is exiting...\n", getpid());
        exit(0);
    } else {
        // Родительский процесс
        printf("Parent process (PID: %d) created child (PID: %d)\n", getpid(), pid);
        printf("Parent is sleeping for 30 seconds...\n");
        printf("You can check for zombie process using:\n");
        printf("ps aux | grep defunct\n");
        printf("or\n");
        printf("ps -l | grep Z\n");
        sleep(30);

        // Теперь считываем статус дочернего процесса
        int status;
        pid_t child_pid = wait(&status);

        if (child_pid > 0) {
            printf("Child process (PID: %d) has been reaped\n", child_pid);
        }
    }
}

int main() {
    printf("Zombie process demonstration starting...\n");
    create_zombie();
    printf("Demonstration finished.\n");
    return 0;
}
