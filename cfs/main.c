#include "file_system.h"

#include <stdio.h>
#include <string.h>

static void print_usage(const char *program) {
    printf("Usage:\n");
    printf("  %s create <filename>\n", program);
    printf("  %s open <filename>\n", program);
    printf("  %s close <filename>\n", program);
    printf("  %s search <filename>\n", program);
}

int main(int argc, char *argv[]) {
    int result;

    fs_init();

    if (argc != 3) {
        print_usage(argv[0]);
        fs_cleanup();
        return 1;
    }

    if (strcmp(argv[1], "create") == 0) {
        result = fs_create_file(argv[2]);
        printf(result == 0 ? "File created successfully.\n" : "Failed to create file.\n");
    } else if (strcmp(argv[1], "open") == 0) {
        result = fs_open_file(argv[2]);
        if (result == 0) {
            printf("File opened successfully.\n");
        } else if (result == -2) {
            printf("File is already open.\n");
        } else if (result == -3) {
            printf("Open file table is full.\n");
        } else {
            printf("Failed to open file.\n");
        }
    } else if (strcmp(argv[1], "close") == 0) {
        result = fs_close_file(argv[2]);
        printf(result == 0 ? "File closed successfully.\n" : "File is not open.\n");
    } else if (strcmp(argv[1], "search") == 0) {
        result = fs_search_file(argv[2]);
        printf(result == 1 ? "File found.\n" : "File not found.\n");
    } else {
        print_usage(argv[0]);
        fs_cleanup();
        return 1;
    }

    fs_cleanup();
    return 0;
}
