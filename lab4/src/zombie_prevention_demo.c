#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

// Обработчик сигнала SIGCHLD
void handle_child_exit(int sig) {
    pid_t pid;
    int status;

    // Используем waitpid с WNOHANG, чтобы обработать все завершившиеся процессы
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        printf("Child process (PID: %d) has been reaped by signal handler\n", pid);
    }
}

void create_processes() {
    // Устанавливаем обработчик сигнала SIGCHLD
    struct sigaction sa;
    sa.sa_handler = &handle_child_exit;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;

    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction failed");
        exit(1);
    }
    // Создаем несколько дочерних процессов
    for (int i = 0; i < 3; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            perror("fork failed");
            exit(1);
        }

        if (pid == 0) {
            // Дочерний процесс
            printf("Child process %d (PID: %d) is starting...\n", i + 1, getpid());
            sleep(4); // Имитация работы
            printf("Child process %d (PID: %d) is exiting...\n", i + 1, getpid());
            exit(0);
        } else {
            // Родительский процесс
            printf("Parent created child %d (PID: %d)\n", i + 1, pid);
        }
    }

    // Родительский процесс продолжает работу
    printf("Parent is working for 20 seconds...\n");
    printf("You can check that there are no zombie processes using:\n");
    printf("ps aux | grep defunct\n");
    sleep(20);
}

int main() {
    printf("Zombie prevention demonstration starting...\n");
    create_processes();
    printf("Demonstration finished.\n");
    return 0;
}
