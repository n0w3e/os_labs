#include "parent.h"
#include "child.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc == 1) {
        runParentProcess("data.txt");
    } else if (argc == 2) {
        runChildProcess(argv[1]);
    } else {
        std::cerr << "Некорректные аргументы\n";
        return 1;
    }
    return 0;
}
