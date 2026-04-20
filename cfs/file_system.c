#include "file_system.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

static OpenFileEntry open_table[MAX_OPEN_FILES];
static const char *OPEN_TABLE_FILE = "./virtual_disk/.open_table";

static void make_path(char *buffer, size_t size, const char *name) {
    snprintf(buffer, size, "%s/%s", ROOT_DIR, name);
}

static int find_open_index(const char *name) {
    for (int i = 0; i < MAX_OPEN_FILES; i++) {
        if (open_table[i].in_use && strcmp(open_table[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

static void load_open_table(void) {
    FILE *fp = fopen(OPEN_TABLE_FILE, "r");
    char name[MAX_NAME_LEN];
    int i = 0;

    if (fp == NULL) {
        return;
    }

    while (i < MAX_OPEN_FILES && fgets(name, sizeof(name), fp) != NULL) {
        size_t len = strlen(name);
        if (len > 0 && name[len - 1] == '\n') {
            name[len - 1] = '\0';
        }
        if (name[0] == '\0') {
            continue;
        }
        open_table[i].in_use = 1;
        strncpy(open_table[i].name, name, MAX_NAME_LEN - 1);
        open_table[i].name[MAX_NAME_LEN - 1] = '\0';
        i++;
    }
    fclose(fp);
}

static void save_open_table(void) {
    FILE *fp = fopen(OPEN_TABLE_FILE, "w");
    if (fp == NULL) {
        return;
    }

    for (int i = 0; i < MAX_OPEN_FILES; i++) {
        if (open_table[i].in_use) {
            fprintf(fp, "%s\n", open_table[i].name);
        }
    }
    fclose(fp);
}

void fs_init(void) {
    memset(open_table, 0, sizeof(open_table));
    mkdir(ROOT_DIR, 0755);
    load_open_table();
}

int fs_create_file(const char *name) {
    char path[512];
    FILE *fp;

    make_path(path, sizeof(path), name);
    fp = fopen(path, "w");
    if (fp == NULL) {
        return -1;
    }
    fclose(fp);
    return 0;
}

int fs_open_file(const char *name) {
    char path[512];
    FILE *fp;
    int free_index = -1;

    if (find_open_index(name) >= 0) {
        return -2;
    }

    for (int i = 0; i < MAX_OPEN_FILES; i++) {
        if (!open_table[i].in_use) {
            free_index = i;
            break;
        }
    }

    if (free_index < 0) {
        return -3;
    }

    make_path(path, sizeof(path), name);
    fp = fopen(path, "r");
    if (fp == NULL) {
        return -1;
    }
    fclose(fp);

    open_table[free_index].in_use = 1;
    strncpy(open_table[free_index].name, name, MAX_NAME_LEN - 1);
    open_table[free_index].name[MAX_NAME_LEN - 1] = '\0';
    save_open_table();
    return 0;
}

int fs_close_file(const char *name) {
    int index = find_open_index(name);
    if (index < 0) {
        return -1;
    }
    open_table[index].in_use = 0;
    open_table[index].name[0] = '\0';
    save_open_table();
    return 0;
}

int fs_search_file(const char *name) {
    char path[512];
    struct stat st;

    make_path(path, sizeof(path), name);
    return stat(path, &st) == 0 ? 1 : 0;
}

void fs_cleanup(void) {
    save_open_table();
}
