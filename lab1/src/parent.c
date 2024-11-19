#include "parent.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>

void parent_process() {
    int pipe1[2], pipe2[2];
    pid_t pid;
    char filename[100];

    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    printf("Enter name file: ");
    scanf("%s", filename);

    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        if (dup2(pipe1[0], STDIN_FILENO) == -1 || dup2(pipe2[1], STDOUT_FILENO) == -1) {
            perror("dup2");
            exit(EXIT_FAILURE);
        }

        close(pipe1[1]);
        close(pipe2[0]);

        execl("../", "lab1", filename, NULL); // Путь до исполняемого файла
        perror("execl");
        exit(EXIT_FAILURE);
    } else {
        close(pipe1[0]);
        close(pipe2[1]);

        char input[256];
        while (1) {
            printf("Enter numbers (or 'exit'): ");
            fgets(input, sizeof(input), stdin);

            if (strcmp(input, "exit\n") == 0) {
                break;
            }

            write(pipe1[1], input, strlen(input));
            fsync(pipe1[1]);

            char result[256];
            int bytesRead = read(pipe2[0], result, sizeof(result) - 1);
            result[bytesRead] = '\0';

            printf("Result: %s\n", result);
        }

        close(pipe1[1]);
        wait(NULL);

        close(pipe2[0]);
    }
}