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

    printf("Введите имя файла: ");
    scanf("%s", filename);

    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Дочерний процесс
        dup2(pipe1[0], STDIN_FILENO);
        dup2(pipe2[1], STDOUT_FILENO);

        close(pipe1[1]);
        close(pipe2[0]);

        execl("./child", "./child", filename, NULL);
        perror("execl");
        exit(EXIT_FAILURE);
    } else {
        // Родительский процесс
        close(pipe1[0]);
        close(pipe2[1]);

        char input[256];
        while (1) {
            printf("Введите числа (или 'exit' для завершения): ");
            fgets(input, sizeof(input), stdin);

            if (strcmp(input, "exit\n") == 0) {
                break;
            }

            write(pipe1[1], input, strlen(input) + 1);

            char result[256];
            read(pipe2[0], result, sizeof(result));

            printf("Результат: %s\n", result);
        }

        wait(NULL);

        close(pipe1[1]);
        close(pipe2[0]);
    }
}
