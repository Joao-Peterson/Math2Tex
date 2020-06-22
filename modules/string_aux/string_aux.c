#include "string_aux.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//recorta string dentro de outra, fechando o espaço vazio
void strcrop(char *string, const char* token){
    char *token_begin = NULL;
    char *token_end=NULL;
    int token_len = strlen(token);

    if((token_begin=strstr(string,token))==NULL){
        return;
    }

    token_end=token_begin+token_len*sizeof(char);

    strncpy(token_begin,token_end,strlen(token_end)+1);
}

//exclúi ultimo caractere de uma string
void strdel_last(char *string, int qtd){
    int i=0;
    while(string[i]!='\0'){
        i++;
    }
    string[i-qtd]='\0';
}

int charhex2value(char char_hex){
    int symb;
    if (char_hex>='0' & char_hex<='9') // number
        symb = char_hex-48;
    else if (char_hex>='a' & char_hex<='f') // number
        symb = char_hex-87;
    else if (char_hex>='A' & char_hex<='F') // number
        symb = char_hex-55;
    else
        symb=0;

    return symb;
}

//get span of string between two pointers
char *strcpyspan(char *beggining, char *end){
    if (beggining == NULL | end == NULL)
        return NULL;

    size_t span_len = end - beggining + 1; //+1 to avoid corrupt the malloc metadata a the end of the block because a '\0' will be inserted later
    if (span_len<=0)
        return NULL;

    char *buffer = malloc(span_len*(sizeof(*buffer)));
    char *buffer_cursor = buffer;

    while(beggining!=end) // increments until the pointers coincide
    { 
        buffer_cursor[0]=beggining[0]; // get the char
        buffer_cursor++; // buffer has a cursor that runs along side the beggining pointer  
        beggining++;
    }
    buffer_cursor[0]='\0';
    return buffer;
}

//takes a string and a mask, every character that coincides bwtween the two will be deleted, copy the result and return
char *str_mask(char *string, char *mask){
    
}

//receive a path to a file, copy and return the same path without the extension
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

//receive a filepath to folder or file, copy and return the same path but only the parent to the directory specified
char *filename_parent_get(char *path){
    char *cursor = path;

    while(cursor[0]!='\0') // goes to the end
        cursor++;

    while(cursor[0]!='/') // goes back to the parent/child "/" separator
    {
        if (cursor==path)
            return "";
        cursor--;
    }

    return strcpyspan(path,cursor); // return a copy of the string between the beggining of *path an where cursor points to, the "/"
}

//receive a filepath to folder or file, copy and return only the name, removing all the parents
char *filename_subdirectory_get_name(char *path){
    char *end;
    char *cursor = path;
    
    while(cursor[0]!='\0') // goes to the end
        cursor++;

    end = cursor; // mark the end

    while(cursor[0]!='/') // goes back to the parent/child "/" separator
        cursor--;

    return strcpyspan(cursor,end); // return the content of *path  between the "/" separator and the end of the string
}
