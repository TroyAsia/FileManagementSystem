#include "file_system.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static OpenFileEntry open_table[MAX_OPEN_FILES];
static const char *OPEN_TABLE_FILE = "./virtual_disk/.open_table";

static void make_path(char *buffer, size_t size, const char *name) {
    snprintf(buffer, size, "%s/%s", ROOT_DIR, name);
}

/* Reject absolute paths and any path segment that is empty, ".", or "..". */
static int path_is_safe(const char *name) {
    const char *p = name;

    if (name == NULL || name[0] == '\0' || name[0] == '/') {
        return 0;
    }

    while (*p) {
        const char *slash = strchr(p, '/');
        size_t len = slash ? (size_t)(slash - p) : strlen(p);

        if (len == 0) {
            return 0;
        }
        if (len == 1 && p[0] == '.') {
            return 0;
        }
        if (len == 2 && p[0] == '.' && p[1] == '.') {
            return 0;
        }
        if (!slash) {
            break;
        }
        p = slash + 1;
    }
    return 1;
}

/* Create parent directories for ROOT_DIR/relative_path (file or final dir). */
static int ensure_parent_dirs(const char *relative_path) {
    char buf[512];
    char *slash;

    if (!path_is_safe(relative_path)) {
        return -1;
    }

    strncpy(buf, relative_path, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';

    slash = strrchr(buf, '/');
    if (slash == NULL) {
        return 0;
    }
    *slash = '\0';

    if (buf[0] == '\0') {
        return 0;
    }

    {
        char build[512];
        size_t root_len = strlen(ROOT_DIR);
        const char *seg = buf;
        size_t pos = 0;

        memcpy(build, ROOT_DIR, root_len);
        pos = root_len;

        while (seg && *seg) {
            const char *next = strchr(seg, '/');
            size_t seglen = next ? (size_t)(next - seg) : strlen(seg);

            if (seglen == 0) {
                return -1;
            }
            if (pos + 1 + seglen >= sizeof(build)) {
                return -1;
            }
            build[pos++] = '/';
            memcpy(build + pos, seg, seglen);
            pos += seglen;
            build[pos] = '\0';

            if (mkdir(build, 0755) != 0 && errno != EEXIST) {
                return -1;
            }

            if (next) {
                seg = next + 1;
            } else {
                break;
            }
        }
    }
    return 0;
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

int fs_mkdir(const char *path) {
    char full[512];
    struct stat st;

    if (!path_is_safe(path)) {
        return -1;
    }
    if (ensure_parent_dirs(path) != 0) {
        return -1;
    }
    make_path(full, sizeof(full), path);
    if (stat(full, &st) == 0) {
        return -2;
    }
    if (mkdir(full, 0755) != 0) {
        return -1;
    }
    return 0;
}

int fs_create_file(const char *name) {
    char path[512];
    FILE *fp;

    if (!path_is_safe(name)) {
        return -1;
    }
    if (ensure_parent_dirs(name) != 0) {
        return -1;
    }

    make_path(path, sizeof(path), name);
    fp = fopen(path, "wx");
    if (fp == NULL) {
        if (errno == EEXIST) {
            return -2;
        }
        return -1;
    }
    fclose(fp);
    return 0;
}

int fs_open_file(const char *name) {
    char path[512];
    FILE *fp;
    int free_index = -1;
    struct stat st;

    if (!path_is_safe(name)) {
        return -1;
    }

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
    if (stat(path, &st) != 0 || S_ISDIR(st.st_mode)) {
        return -1;
    }

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

int fs_read_file(const char *name) {
    char path[512];
    FILE *fp;
    unsigned char buf[4096];
    size_t n;

    if (find_open_index(name) < 0) {
        return -4;
    }

    make_path(path, sizeof(path), name);
    fp = fopen(path, "rb");
    if (fp == NULL) {
        return -1;
    }

    while ((n = fread(buf, 1, sizeof(buf), fp)) > 0) {
        if (fwrite(buf, 1, n, stdout) != n) {
            fclose(fp);
            return -1;
        }
    }
    fclose(fp);
    return 0;
}

int fs_write_file(const char *name, const void *data, size_t data_len) {
    char path[512];
    FILE *fp;

    if (find_open_index(name) < 0) {
        return -4;
    }

    make_path(path, sizeof(path), name);
    fp = fopen(path, "wb");
    if (fp == NULL) {
        return -1;
    }
    if (data_len > 0 && fwrite(data, 1, data_len, fp) != data_len) {
        fclose(fp);
        return -1;
    }
    if (fclose(fp) != 0) {
        return -1;
    }
    return 0;
}

int fs_search_file(const char *name) {
    char path[512];
    struct stat st;

    if (!path_is_safe(name)) {
        return 0;
    }

    make_path(path, sizeof(path), name);
    return stat(path, &st) == 0 && S_ISREG(st.st_mode) ? 1 : 0;
}

int fs_delete_file(const char *name) {
    char path[512];
    int idx;
    struct stat st;

    if (!path_is_safe(name)) {
        return -1;
    }

    make_path(path, sizeof(path), name);
    if (stat(path, &st) != 0 || !S_ISREG(st.st_mode)) {
        return -1;
    }

    idx = find_open_index(name);
    if (idx >= 0) {
        open_table[idx].in_use = 0;
        open_table[idx].name[0] = '\0';
        save_open_table();
    }

    if (unlink(path) != 0) {
        return -1;
    }
    return 0;
}

void fs_cleanup(void) {
    save_open_table();
}
