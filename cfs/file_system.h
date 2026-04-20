#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#define MAX_OPEN_FILES 32
#define MAX_NAME_LEN 256
#define ROOT_DIR "./virtual_disk"

typedef struct {
    char name[MAX_NAME_LEN];
    int in_use;
} OpenFileEntry;

void fs_init(void);
int fs_create_file(const char *name);
int fs_open_file(const char *name);
int fs_close_file(const char *name);
int fs_search_file(const char *name);
void fs_cleanup(void);

#endif
