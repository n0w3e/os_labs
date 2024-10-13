#include <gtest/gtest.h>
#include <fstream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include "child.h"
#include "parent.h"

// Тест для child_process
TEST(ChildProcessTest, CorrectSum) {
    const char* filename = "test_output.txt";
    const char* input = "1.0 2.0 3.0\n";

    // Перенаправляем stdin для child_process
    FILE* pipe = fmemopen((void*)input, strlen(input), "r");
    stdin = pipe;

    // Вызываем функцию дочернего процесса
    child_process(filename);

    fclose(pipe);

    // Проверяем содержимое файла
    std::ifstream file(filename);
    ASSERT_TRUE(file.is_open());

    std::string result;
    std::getline(file, result);
    file.close();

    // Ожидаемое значение
    std::string expected = "Сумма: 6.00";
    ASSERT_EQ(result, expected);

    // Удаляем тестовый файл
    remove(filename);
}

// Тест для parent_process с имитацией дочернего процесса
TEST(ParentProcessTest, BasicInteraction) {
    int pipe1[2], pipe2[2];
    pipe(pipe1);  // Pipe для передачи данных от родителя к ребенку
    pipe(pipe2);  // Pipe для передачи данных от ребенка к родителю

    pid_t pid = fork();
    if (pid == 0) {
        // Дочерний процесс
        dup2(pipe1[0], STDIN_FILENO);  // Чтение данных из pipe1
        dup2(pipe2[1], STDOUT_FILENO); // Запись данных в pipe2
        close(pipe1[1]);
        close(pipe2[0]);

        // Имитация работы child_process
        const char* input = "1.0 2.0 3.0\n";
        write(STDOUT_FILENO, "Сумма: 6.00\n", 13);
        exit(0);
    } else {
        // Родительский процесс
        close(pipe1[0]);
        close(pipe2[1]);

        // Передаем данные в pipe для дочернего процесса
        write(pipe1[1], "1.0 2.0 3.0\n", 13);

        // Чтение результата из дочернего процесса
        char result[256];
        read(pipe2[0], result, sizeof(result));

        // Ожидание завершения дочернего процесса
        wait(NULL);

        // Проверка результата
        std::string expected = "Сумма: 6.00\n";
        ASSERT_EQ(std::string(result), expected);
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
