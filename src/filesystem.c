#include <stdio.h>
#include <string.h>

typedef struct {
    char name[32];
    char content[256];
} File;

static File files[32];
static int file_count = 0;

void fs_touch(const char* name) {
    strcpy(files[file_count++].name, name);
}

void fs_write(const char* name, const char* content) {
    for (int i = 0; i < file_count; i++) {
        if (strcmp(files[i].name, name) == 0) {
            strcpy(files[i].content, content);
        }
    }
}

void fs_cat(const char* name) {
    for (int i = 0; i < file_count; i++) {
        if (strcmp(files[i].name, name) == 0) {
            printf("%s\n", files[i].content);
        }
    }
}

void fs_ls() {
    for (int i = 0; i < file_count; i++) {
        printf("%s\n", files[i].name);
    }
}
