#include "parent.h"
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

void runParentProcess(const std::string& filename) {
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cerr << "Ошибка: входной файл не найден: " << filename << std::endl;
        return;
    }

    std::string input((std::istreambuf_iterator<char>(inputFile)),
                      std::istreambuf_iterator<char>());
    inputFile.close();

    int fd = open(filename.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0666);
    if (fd == -1) {
        perror("Ошибка открытия файла");
        return;
    }

    if (write(fd, input.c_str(), input.size()) == -1) {
        perror("Ошибка записи в файл");
        close(fd);
        return;
    }
    close(fd);

    pid_t pid = fork();

    if (pid == -1) {
        perror("Ошибка создания дочернего процесса");
        return;
    }

    if (pid == 0) {
        execl("./lab3", "./lab3", filename.c_str(), nullptr);
        perror("Ошибка вызова execl");
        exit(EXIT_FAILURE);
    } else {
        int status;
        waitpid(pid, &status, 0);
    }
}
