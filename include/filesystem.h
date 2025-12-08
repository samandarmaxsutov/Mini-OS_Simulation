#ifndef FILESYSTEM_H
#define FILESYSTEM_H

void fs_touch(const char* name);
void fs_write(const char* name, const char* content);
void fs_cat(const char* name);
void fs_ls();
void fs_ls_long();
void fs_delete(const char* name);

#endif
