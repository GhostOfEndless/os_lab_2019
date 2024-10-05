#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <seed> <array_size>\n", argv[0]);
        exit(1);
    }

    pid_t pid = fork();

    if (pid < 0) {
        fprintf(stderr, "Fork failed\n");
        exit(1);
    } else if (pid == 0) {
        // Child process
        char *args[] = {"./sequential_min_max", argv[1], argv[2], NULL};
        execv("./sequential_min_max", args);


        fprintf(stderr, "execv failed\n");
        exit(1);
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            printf("Sequential Min Max completed with status %d\n", WEXITSTATUS(status));
        } else {
            printf("Sequential Min Max terminated abnormally\n");
        }
    }

    return 0;
}
