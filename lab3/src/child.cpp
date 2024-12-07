#include "child.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

void runChildProcess(const std::string& filename) {
    int fd = open(filename.c_str(), O_RDONLY);
    if (fd == -1) {
        perror("Ошибка открытия файла");
        return;
    }

    struct stat sb;
    if (fstat(fd, &sb) == -1) {
        perror("Ошибка получения информации о файле");
        close(fd);
        return;
    }

    char* data = static_cast<char*>(mmap(nullptr, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0));
    if (data == MAP_FAILED) {
        perror("Ошибка отображения файла");
        close(fd);
        return;
    }

    std::istringstream iss(std::string(data, sb.st_size));
    float num, sum = 0;
    while (iss >> num) {
        sum += num;
    }

    std::ofstream outFile("result.txt");
    if (!outFile) {
        std::cerr << "Ошибка открытия файла для записи результата\n";
    } else {
        outFile << "Сумма чисел: " << sum << std::endl;
        outFile.close();
    }

    munmap(data, sb.st_size);
    close(fd);
}
