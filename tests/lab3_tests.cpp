#include <gtest/gtest.h>
#include <fstream>
#include <string>
#include "../include/parent.h"
#include "../include/child.h"

constexpr auto inputFile = "input.txt";
constexpr auto resultFile = "result.txt";

std::string readFile(const char* filename) {
    std::ifstream file(filename);
    std::ostringstream content;
    if (file.is_open()) {
        content << file.rdbuf();
        file.close();
    }
    return content.str();
}

TEST(ProcessTest, SumWithNegativeNumbers) {
    constexpr auto input = "-3 -4 5";
    constexpr auto expected = "Сумма чисел: -2\n";

    std::ofstream inputFileStream(inputFile);
    ASSERT_TRUE(inputFileStream.is_open());
    inputFileStream << input;
    inputFileStream.close();

    runParentProcess(inputFile);

    std::string result = readFile(resultFile);
    EXPECT_EQ(result, expected);

    remove(inputFile);
    remove(resultFile);
}

TEST(ProcessTest, SumWithZero) {
    constexpr auto input = "0 0 0";
    constexpr auto expected = "Сумма чисел: 0\n";

    std::ofstream inputFileStream(inputFile);
    ASSERT_TRUE(inputFileStream.is_open());
    inputFileStream << input;
    inputFileStream.close();

    runParentProcess(inputFile);

    std::string result = readFile(resultFile);
    EXPECT_EQ(result, expected);

    remove(inputFile);
    remove(resultFile);
}

TEST(ProcessTest, SumWithLargeNumbers) {
    constexpr auto input = "100000 200000 300000";
    constexpr auto expected = "Сумма чисел: 600000\n";

    std::ofstream inputFileStream(inputFile);
    ASSERT_TRUE(inputFileStream.is_open());
    inputFileStream << input;
    inputFileStream.close();

    runParentProcess(inputFile);

    std::string result = readFile(resultFile);
    EXPECT_EQ(result, expected);

    remove(inputFile);
    remove(resultFile);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
