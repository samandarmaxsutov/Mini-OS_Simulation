#include <stdio.h>
#include <string.h>
#include <time.h>

typedef struct {
    char name[32];
    char content[256];
    int size;
    time_t created_at;
    time_t modified_at;
} File;

static File files[32];
static int file_count = 0;

static void print_time(time_t t) {
    struct tm *tm = localtime(&t);
    printf("%02d-%02d-%04d %02d:%02d:%02d",
           tm->tm_mday, tm->tm_mon + 1, tm->tm_year + 1900,
           tm->tm_hour, tm->tm_min, tm->tm_sec);
}

void fs_touch(const char* name) {
    strcpy(files[file_count].name, name);
    files[file_count].content[0] = '\0';  // empty file
    files[file_count].size = 0;

    time(&files[file_count].created_at);
    files[file_count].modified_at = files[file_count].created_at;

    file_count++;
}

void fs_write(const char* name, const char* content) {
    for (int i = 0; i < file_count; i++) {
        if (strcmp(files[i].name, name) == 0) {

            strcpy(files[i].content, content);
            files[i].size = strlen(content);

            time(&files[i].modified_at);
            return;
        }
    }
}

void fs_cat(const char* name) {
    for (int i = 0; i < file_count; i++) {
        if (strcmp(files[i].name, name) == 0) {
            printf("%s\n", files[i].content);
            return;
        }
    }
    printf("File not found: %s\n", name);
}

void fs_ls() {
    for (int i = 0; i < file_count; i++) {
        printf("%s (size: %d bytes)\n",
               files[i].name,
               files[i].size);
    }
}

void fs_ls_long() {
    printf("NAME\tSIZE\tCREATED\t\t\tMODIFIED\n");
    printf("-------------------------------------------------------------\n");

    for (int i = 0; i < file_count; i++) {
        printf("%s\t%d\t", files[i].name, files[i].size);
        print_time(files[i].created_at);
        printf("\t");
        print_time(files[i].modified_at);
        printf("\n");
    }
}

void fs_delete(const char* name) {
    for (int i = 0; i < file_count; i++) {
        if (strcmp(files[i].name, name) == 0) {

            // shift array left
            for (int j = i; j < file_count - 1; j++)
                files[j] = files[j + 1];

            file_count--;
            printf("Deleted file %s\n", name);
            return;
        }
    }
    printf("File not found: %s\n", name);
}
