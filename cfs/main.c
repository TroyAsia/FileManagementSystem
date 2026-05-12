#include "file_system.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void print_usage(const char *program) {
    printf("Usage:\n");
    printf("  %s mkdir <dir/path>          create subdirectory (parents created as needed)\n", program);
    printf("  %s create <file/path>        create empty file (fails if exists)\n", program);
    printf("  %s open <file/path>          open file (tracks in open table)\n", program);
    printf("  %s close <file/path>         close file\n", program);
    printf("  %s read <file/path>          read file to stdout (must be open)\n", program);
    printf("  %s write <file/path> [text]  overwrite file (must be open); text may be omitted\n", program);
    printf("  %s search <file/path>        report if regular file exists\n", program);
    printf("  %s delete <file/path>        delete file (removes from open table if open)\n", program);
    printf("\nPaths are relative to virtual_disk; use subdirs like docs/notes.txt (no .. or leading /).\n");
}

static char *join_argv(int argc, char **argv, int start) {
    size_t total = 0;

    for (int i = start; i < argc; i++) {
        total += strlen(argv[i]) + 1;
    }

    char *buf = malloc(total + 1);
    if (buf == NULL) {
        return NULL;
    }

    size_t pos = 0;
    for (int i = start; i < argc; i++) {
        if (i > start) {
            buf[pos++] = ' ';
        }
        size_t len = strlen(argv[i]);
        memcpy(buf + pos, argv[i], len);
        pos += len;
    }
    buf[pos] = '\0';
    return buf;
}

int main(int argc, char *argv[]) {
    int result;

    fs_init();

    if (argc < 3) {
        print_usage(argv[0]);
        fs_cleanup();
        return 1;
    }

    if (strcmp(argv[1], "write") == 0) {
        char *payload = join_argv(argc, argv, 3);
        if (payload == NULL) {
            fprintf(stderr, "Out of memory.\n");
            fs_cleanup();
            return 1;
        }
        result = fs_write_file(argv[2], payload, strlen(payload));
        free(payload);

        if (result == 0) {
            printf("Write OK.\n");
        } else if (result == -4) {
            printf("File is not open (open it before write).\n");
        } else {
            printf("Write failed.\n");
        }
        fs_cleanup();
        return result == 0 ? 0 : 1;
    }

    if (argc != 3) {
        print_usage(argv[0]);
        fs_cleanup();
        return 1;
    }

    if (strcmp(argv[1], "mkdir") == 0) {
        result = fs_mkdir(argv[2]);
        if (result == 0) {
            printf("Directory created.\n");
        } else if (result == -2) {
            printf("Directory already exists.\n");
        } else {
            printf("Failed to create directory.\n");
        }
    } else if (strcmp(argv[1], "create") == 0) {
        result = fs_create_file(argv[2]);
        if (result == 0) {
            printf("File created successfully.\n");
        } else if (result == -2) {
            printf("File already exists.\n");
        } else {
            printf("Failed to create file.\n");
        }
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
    } else if (strcmp(argv[1], "read") == 0) {
        result = fs_read_file(argv[2]);
        if (result == 0) {
            if (isatty(STDOUT_FILENO)) {
                printf("\n");
            }
        } else if (result == -4) {
            printf("File is not open (open it before read).\n");
        } else {
            printf("Read failed.\n");
        }
    } else if (strcmp(argv[1], "search") == 0) {
        result = fs_search_file(argv[2]);
        printf(result == 1 ? "File found.\n" : "File not found.\n");
    } else if (strcmp(argv[1], "delete") == 0) {
        result = fs_delete_file(argv[2]);
        printf(result == 0 ? "File deleted.\n" : "Delete failed.\n");
    } else {
        print_usage(argv[0]);
        fs_cleanup();
        return 1;
    }

    fs_cleanup();
    return 0;
}
