#include "file_handle.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

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

void rm_dir(char *filename){
    DIR *handle = opendir(filename);
    if (handle==NULL)
        return;
    
    struct dirent *entry = NULL;
    size_t path_len = strlen(filename);
    size_t len;
    char *rel_path;

    while((entry=readdir(handle))!=NULL){
        if((strcmp(entry->d_name,".")!=0)&(strcmp(entry->d_name,"..")!=0)){
            
            len = path_len + strlen(entry->d_name) + 2; // 2 = '/' and '\0' , the two '\0' of both strings were not counted
            rel_path = (char*)malloc(len);
            snprintf(rel_path,len,"%s/%s",filename,entry->d_name);
            
            unlink(rel_path);
            //printf("Debug errno: %d\n",errno);
            if(errno==EISDIR | errno==EACCES){ // if it is a directory. acess can be retricted
                rm_dir(rel_path);
                free(rel_path);
            }
        }
    }

    rmdir(filename);
    closedir(handle);
    return;
}