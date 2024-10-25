#include <gtest/gtest.h>
#include <fstream>
#include <string>
#include <locale>
#include <codecvt>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include "../include/child.h"
#include "../include/parent.h"

TEST(ParentProcessTest, SumWithNegativeNumbers) {
    const char* input = "-3 -4 5";
    const char* expected = "Sum: -2.00";
    const char* filename = "result_neg.txt";

    int pipe1[2], pipe2[2];
    ASSERT_EQ(pipe(pipe1), 0);
    ASSERT_EQ(pipe(pipe2), 0);

    pid_t pid = fork();
    ASSERT_GE(pid, 0);

    if (pid == 0) {
        dup2(pipe1[0], STDIN_FILENO);
        dup2(pipe2[1], STDOUT_FILENO);

        close(pipe1[1]);
        close(pipe2[0]);

        execl("../", "lab1", filename, NULL); // путь до исполняемого файла
        perror("execl");
        exit(EXIT_FAILURE);
    } else {
        close(pipe1[0]);
        close(pipe2[1]);

        write(pipe1[1], input, strlen(input));
        close(pipe1[1]);

        char result[256];
        int bytesRead = read(pipe2[0], result, sizeof(result) - 1);
        result[bytesRead] = '\0';

        result[strcspn(result, "\n")] = '\0';

        ASSERT_EQ(std::string(result), expected);

        std::ifstream file(filename);
        ASSERT_TRUE(file.is_open());

        std::string file_content;
        std::getline(file, file_content);
        file.close();

        ASSERT_EQ(file_content, expected);

        remove(filename);

        wait(NULL);
    }
}

TEST(ParentProcessTest, SumWithZero) {
    const char* input = "0 0 0";
    const char* expected = "Sum: 0.00";
    const char* filename = "result_zero.txt";

    int pipe1[2], pipe2[2];
    ASSERT_EQ(pipe(pipe1), 0);
    ASSERT_EQ(pipe(pipe2), 0);

    pid_t pid = fork();
    ASSERT_GE(pid, 0);

    if (pid == 0) {
        dup2(pipe1[0], STDIN_FILENO);
        dup2(pipe2[1], STDOUT_FILENO);

        close(pipe1[1]);
        close(pipe2[0]);

        execl("../", "lab1", filename, NULL); // путь до исполняемого файла
        perror("execl");
        exit(EXIT_FAILURE);
    } else {
        close(pipe1[0]);
        close(pipe2[1]);

        write(pipe1[1], input, strlen(input));
        close(pipe1[1]);

        char result[256];
        int bytesRead = read(pipe2[0], result, sizeof(result) - 1);
        result[bytesRead] = '\0';

        result[strcspn(result, "\n")] = '\0';

        ASSERT_EQ(std::string(result), expected);


        std::ifstream file(filename);
        ASSERT_TRUE(file.is_open());

        std::string file_content;
        std::getline(file, file_content);
        file.close();

        ASSERT_EQ(file_content, expected);

        remove(filename);

        wait(NULL);
    }
}

TEST(ParentProcessTest, SumWithLargeNumbers) {
    const char* input = "100000 200000 300000";
    const char* expected = "Sum: 600000.00";
    const char* filename = "result_large.txt";

    int pipe1[2], pipe2[2];
    ASSERT_EQ(pipe(pipe1), 0);
    ASSERT_EQ(pipe(pipe2), 0);

    pid_t pid = fork();
    ASSERT_GE(pid, 0);

    if (pid == 0) {
        dup2(pipe1[0], STDIN_FILENO);
        dup2(pipe2[1], STDOUT_FILENO);

        close(pipe1[1]);
        close(pipe2[0]);

        execl("../", "lab1", filename, NULL); // путь до исполняемого файла
        perror("execl");
        exit(EXIT_FAILURE);
    } else {
        close(pipe1[0]);
        close(pipe2[1]);

        write(pipe1[1], input, strlen(input));
        close(pipe1[1]);

        char result[256];
        read(pipe2[0], result, sizeof(result) - 1);
        result[sizeof(result) - 1] = '\0';
        close(pipe2[0]);

        std::string result_str(result);
        result_str.erase(result_str.find_last_not_of(" \n\r\t") + 1);

        ASSERT_EQ(result_str, expected);


        std::ifstream file(filename);
        ASSERT_TRUE(file.is_open());

        std::string file_content;
        std::getline(file, file_content);
        file.close();

        ASSERT_EQ(file_content, expected);

        remove(filename);

        wait(NULL);
    }
}


int main(int argc, char **argv) {
    std::locale::global(std::locale("en_US.UTF-8"));
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
