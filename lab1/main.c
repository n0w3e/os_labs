#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/parent.h"
#include "../include/child.h"

int main(int argc, char* argv[]) {
    if (argc == 1) {
        parent_process();
    } else if (argc == 2) {
        child_process(argv[1]);
    } else {
        fprintf(stderr, "Error\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
