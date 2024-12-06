#include "parent.h"
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

void runParentProcess(const std::string& filename) {
    std::cout << "Введите числа через пробел:\n";
    std::string input;
    std::getline(std::cin, input);

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
        execl("/home/n0wee/Coding/os_labs/build/lab3/lab3", "lab3", filename.c_str(), nullptr);
        perror("Ошибка вызова execl");
        exit(EXIT_FAILURE);
    } else {
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            std::cout << "Дочерний процесс завершился с кодом: " << WEXITSTATUS(status) << "\n";
        } else {
            std::cerr << "Ошибка: дочерний процесс завершился некорректно\n";
        }
    }
}
