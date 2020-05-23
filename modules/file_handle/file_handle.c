#include "file_handle.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char *filename_to_folder(char *filename_path){
    char *buffer = (char*)malloc(sizeof(char)*strlen(filename_path));
    int i=0;
    while(filename_path[i]!='\0'){
        buffer[i]=filename_path[i];
        i++;
    }
    while(filename_path[i]!='.'){
        i--;
    }
    buffer[i]='\0';
    return buffer;
}

int on_extract_entry(const char *filename, void *arg){ // chama ao extrair uma entry, arquivo, todo vez
    // static int i = 0;
    // int n = 1;
    // printf("Extracted: %s (%d of %d)\n", filename, ++i, n);
    return 0;
}

void cd_to(DIR **handle, const char *filename){
    struct dirent *entry = NULL;
    char *buffer = (char*)malloc(sizeof(char)*50);
    while((entry=readdir(*handle))!=NULL){
        printf(">> %s\n",entry->d_name);
        if(!strcmp(entry->d_name,filename)){  
            printf("Mudando diretorio para : %s\n",entry->d_name);
            chdir(entry->d_name);
            //printf("PWN: %s\n",getcwd(buffer,50));
            *handle = opendir(entry->d_name);
            return;
        }
    }
    printf("Diretorio nao encontrado!\n");
}

void ld_dir(DIR *handle){
    struct dirent *entry = NULL;
    while((entry=readdir(handle))!=NULL){
        printf(">> %s\n",entry->d_name);
    }
    rewinddir(handle);
    return;
}