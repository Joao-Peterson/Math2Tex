#ifndef __FILE_HANDLE
#define __FILE_HANDLE

#include <dirent.h>

// called every time the zip_extract() from libzip extracts a entry
int on_extract_entry(const char *filename, void *arg);

// search a entry within the current directory handle and change the handle to that directory
void cd_to(DIR **handle, const char *filename);

// list the contents of the directory pointed by the handle
void ld_dir(DIR *handle);

// remove a entry within a directory. Be careful to "cd" to the parent directory first
void rm_dir(char *filename);

#endif