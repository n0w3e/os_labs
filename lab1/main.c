#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/parent.h"
#include "../include/child.h"

int main(int argc, char* argv[]) {
    if (argc == 1) {
        // Если программа запущена без аргументов, запускаем родительский процесс
        printf("Запуск родительского процесса\n");
        parent_process();
    } else if (argc == 2) {
        // Если передан аргумент (например, имя файла), запускаем дочерний процесс
        printf("Запуск дочернего процесса, файл: %s\n", argv[1]);
        child_process(argv[1]);
    } else {
        fprintf(stderr, "Неверное количество аргументов\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
