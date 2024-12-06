#include <gtest/gtest.h>
#include <fstream>
#include <string>
#include <cstdlib>
#include <sys/wait.h>
#include <unistd.h>

void writeInputToFile(const std::string& input, const std::string& filename) {
    std::ofstream file(filename);
    ASSERT_TRUE(file.is_open());
    file << input;
    file.close();
}

void executeParentAndChild(const std::string& input, const std::string& sharedFile, const std::string& resultFile) {
    pid_t pid = fork();
    ASSERT_NE(pid, -1) << "Ошибка fork()";

    if (pid == 0) {
        execl("./lab3", "lab3", sharedFile.c_str(), nullptr);
        perror("Ошибка вызова execl");
        exit(EXIT_FAILURE);
    } else {
        int status;
        waitpid(pid, &status, 0);
        ASSERT_TRUE(WIFEXITED(status)) << "Ошибка выполнения дочернего процесса";
        ASSERT_EQ(WEXITSTATUS(status), 0) << "Ошибка завершения дочернего процесса";
    }
}

TEST(ParentChildIntegrationTest, SumWithNegativeNumbers) {
    const std::string input = "-3 -4 5";
    const std::string expected = "Сумма чисел: -2";
    const std::string sharedFile = "input.txt";
    const std::string resultFile = "result.txt";

    writeInputToFile(input, sharedFile);

    executeParentAndChild(input, sharedFile, resultFile);

    std::ifstream result(resultFile);
    ASSERT_TRUE(result.is_open());

    std::string resultContent;
    std::getline(result, resultContent);
    result.close();

    ASSERT_EQ(resultContent, expected);

    remove(sharedFile.c_str());
    remove(resultFile.c_str());
}

TEST(ParentChildIntegrationTest, SumWithZero) {
    const std::string input = "0 0 0";
    const std::string expected = "Сумма чисел: 0";
    const std::string sharedFile = "input.txt";
    const std::string resultFile = "result.txt";

    writeInputToFile(input, sharedFile);
    executeParentAndChild(input, sharedFile, resultFile);

    std::ifstream result(resultFile);
    ASSERT_TRUE(result.is_open());

    std::string resultContent;
    std::getline(result, resultContent);
    result.close();

    ASSERT_EQ(resultContent, expected);

    remove(sharedFile.c_str());
    remove(resultFile.c_str());
}

TEST(ParentChildIntegrationTest, SumWithLargeNumbers) {
    const std::string input = "100000 200000 300000";
    const std::string expected = "Сумма чисел: 600000";
    const std::string sharedFile = "input.txt";
    const std::string resultFile = "result.txt";

    writeInputToFile(input, sharedFile);
    executeParentAndChild(input, sharedFile, resultFile);

    std::ifstream result(resultFile);
    ASSERT_TRUE(result.is_open());

    std::string resultContent;
    std::getline(result, resultContent);
    result.close();

    ASSERT_EQ(resultContent, expected);

    remove(sharedFile.c_str());
    remove(resultFile.c_str());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
