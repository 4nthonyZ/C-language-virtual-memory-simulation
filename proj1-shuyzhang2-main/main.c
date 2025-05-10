#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "task1.h"
#include "task2.h"
#include "task3.h"
#include "task4.h"

void print_usage_and_exit() {
    fprintf(stderr, "Usage: translate -f <filename> -t <task>\n");
    exit(1);
}

int main(int argc, char *argv[]) {
    char *filename = NULL;
    char *task = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) {
            filename = argv[++i];
        } else if (strcmp(argv[i], "-t") == 0 && i + 1 < argc) {
            task = argv[++i];
        } else {
            print_usage_and_exit();
        }
    }

    if (!filename || !task) {
        print_usage_and_exit();
    }

    if (strcmp(task, "task1") == 0) {
        run_task1(filename);
    } else if (strcmp(task, "task2") == 0) {
        run_task2(filename);
    } else if (strcmp(task, "task3") == 0) {
        run_task3(filename);
    }else if (strcmp(task, "task4") == 0) {
        run_task4(filename);
    }else {
        fprintf(stderr, "Unsupported task: %s\n", task);
        exit(1);
    }

    return 0;
}
