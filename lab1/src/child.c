#include "child.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void child_process(const char* filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    char input[256];
    while (fgets(input, sizeof(input), stdin)) {
        float sum = 0;
        float number;
        char *token = strtok(input, " ");
        while (token != NULL) {
            if (sscanf(token, "%f", &number) == 1) {
                sum += number;
            }
            token = strtok(NULL, " ");
        }

        fprintf(file, "Sum: %.2f\n", sum);
        fflush(file);

        printf("Sum: %.2f\n", sum);
    }

    fclose(file);
}
