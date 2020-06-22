#ifndef _STRING_AUX
#define _STRING_AUX

//get span of string between two pointers
char *strcpyspan(char *beggining, char *end);

//converts a char hex value to a number
int charhex2value(char char_hex);

//deletes "qtd" elements counting from the end of the string to the left
void strdel_last(char *string, int qtd);

//removes the "token" from the "string" closing the gap where it was 
void strcrop(char *string, const char* token);

//receive a path to a file, copy and return the same path without the extension
char *filename_to_folder(char *filename_path);

//receive a filepath to folder or file, copy and return the same path but only the parent to the directory specified
char *filename_parent_get(char *path);

//receive a filepath to folder or file, copy and return only the name, removing all the parents
char *filename_subdirectory_get_name(char *path);

#endif