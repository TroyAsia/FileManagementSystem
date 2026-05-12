#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include <stddef.h>

#define MAX_OPEN_FILES 32
#define MAX_NAME_LEN 256
#define ROOT_DIR "./virtual_disk"

typedef struct {
    char name[MAX_NAME_LEN];
    int in_use;
} OpenFileEntry;

void fs_init(void);
void fs_cleanup(void);

/** Create an empty file; parent directories are created as needed. */
int fs_create_file(const char *name);
/** Create a subdirectory under virtual_disk (and parents as needed). */
int fs_mkdir(const char *path);

int fs_open_file(const char *name);
int fs_close_file(const char *name);

/** Read entire file to stdout; file must be open in the open table. */
int fs_read_file(const char *name);
/** Overwrite file with data; file must be open. */
int fs_write_file(const char *name, const void *data, size_t data_len);

int fs_search_file(const char *name);
/** Remove file; if it was open, it is removed from the open table. */
int fs_delete_file(const char *name);

#endif
