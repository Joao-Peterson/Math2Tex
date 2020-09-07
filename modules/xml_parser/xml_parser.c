#include "xml_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#if defined (__WIN32__) || defined (__WIN64__) /*-------------------------------------------------------------------------*/

#include <windows.h>

// Get win system error as string
char *get_win_error()
{
    DWORD error_code = GetLastError();

    char *error_msg = malloc(sizeof(*error_msg)*200);
    
    FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, // format as error from system
                    NULL, // aditional parameter
                    error_code, // DWORD error code
                    MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT), // language to format string
                    (LPSTR)error_msg, // string, cast before passing
                    200, // size
                    NULL // aditional parameters
                    );

    return error_msg;
}

//map file to memory using windows memory api, returns adress pointer
char *fmap_win(char *filename, void **file_handle_ptr, void **file_map_obj_ptr, uint8_t options)
{
    // #define FMAP_READ 0x1
    // #define FMAP_WRITE 0x2

    DWORD FileHandle_options;
    DWORD FileObjMap_options;
    DWORD FileView_options;

    if(options & 0x1) // Read only
    {
        FileHandle_options = GENERIC_READ;
        FileObjMap_options = PAGE_READONLY;
        FileView_options = FILE_MAP_READ;
    }
    else if (options & 0x2) // Write only
    {
        FileHandle_options = GENERIC_WRITE;
        FileObjMap_options = PAGE_WRITECOPY;
        FileView_options = FILE_MAP_WRITE;
    }
    else // Read/Write
    {
        FileHandle_options = (GENERIC_READ | GENERIC_WRITE);
        FileObjMap_options = PAGE_READWRITE;
        FileView_options = FILE_MAP_ALL_ACCESS;
    }
    
    // SECURITY_ATTRIBUTES file_acess_security;
    LPSECURITY_ATTRIBUTES file_acess_security = malloc(sizeof(*file_acess_security));
    file_acess_security->bInheritHandle=1; // file can be inherited by child processes 
    file_acess_security->lpSecurityDescriptor = NULL; // null because CreatefileA will append a default security descriptor later

    // OPENS FILE 
    *file_handle_ptr = CreateFileA(filename, // filename
                                    FileHandle_options, // open as read 
                                    0, // no sharing with other non child processes of the system, makes the file exlusive for this aplication
                                    file_acess_security, // the struct that contains secutity data, see above SECURITY_ATTRIBUTES
                                    OPEN_ALWAYS, // always try to open a existing file
                                    FILE_ATTRIBUTE_NORMAL, // atributes and flags when opening the file, in this case normal
                                    NULL // file handle template when creating a new file, in this case it's only for read, therefore, NULL 
                                    );

    printf("- %s",get_win_error());

    // FILE MAP
    *file_map_obj_ptr =  CreateFileMappingA(*file_handle_ptr, // the file handle
                                                    file_acess_security, // the struct that contains secutity data, see above SECURITY_ATTRIBUTES
                                                    FileObjMap_options, // to be read, copied and executed
                                                    0,0, // size is the same as the file
                                                    NULL // name
                                                    );

    printf("- %s",get_win_error());
    
    LPVOID myfile_view_base_adress = MapViewOfFile(*file_map_obj_ptr, // filemap object
                                                    FileView_options, // acess permissions
                                                    0,0, // offset
                                                    0 // size to view, if 0 all of it
                                                    );

    printf("- %s",get_win_error());
    
    char *myfile_view = (char*)myfile_view_base_adress;

    FlushViewOfFile(myfile_view_base_adress,0); // flush all cached modifications to the view before closing it

    return myfile_view;
}

// closes view and handles
void fmap_close(char *filename, void **file_handle_ptr, void **file_map_obj_ptr)
{
    FlushViewOfFile(filename,0); // flush all cached modifications to the view before closing it
    printf("- %s",get_win_error());
    UnmapViewOfFile(filename); // closes file view  
    printf("- %s",get_win_error());
    CloseHandle(*file_handle_ptr); // close the mapped object
    printf("- %s",get_win_error());
    CloseHandle(*file_map_obj_ptr); // close file 
    printf("- %s",get_win_error());
}

#endif /*--------------------------------------------------------------------------------------------------------------*/

//grava arquivo txt na memória, uma string
char *read_asci(const char *path){
FILE *file = fopen(path, "r"); //abre arquivo
if(file==NULL){ // verifica se ponteiro lido é nulo
    printf("Arquivo nao encontrado ou invalido\n");
    return NULL;
}

fseek(file,0,SEEK_END); // vai até o fim do file
int len = ftell(file); // pega tamanho
fseek(file,0,SEEK_SET); // volta ao inicio

char *buf = (char*)malloc(sizeof(char)*(len+1)); //aloca buffer com tamanho +1
fread(buf,sizeof(char),len,file); // lê para o buffer
buf[len]='\0'; // introduz fechamento de string

fclose(file);
return buf;
}


//compara tag com string e retorna 1 ou 0
bool atb_cmp(Tag *ref, int id, const char *str)
{
    char *atb_begin = ref->atb[id];
    char *token_buf;

    if(!(ref->atbUsePrefix)) // dont use prefix
    {
        token_buf = strstr(atb_begin,":"); // look for the prefix, 
        if (token_buf!=NULL) // if it was found
            atb_begin = token_buf + 1; // then points to char next to prefix
    }

    if(strcmp(atb_begin,str)==0)
        return true;
    else
        return false;
}

//lê um atributo inteiro de uma tag
char *atb_get(Tag *ref, int id){
    char *atb_begin = (char*)malloc(strlen(ref->atb[id])+1);
    atb_begin = ref->atb[id];
    if (atb_begin!=NULL)
        return atb_begin;     
    else
        return NULL;
}

//lê valor de um atributo em uma tag e retorna o valor bruto
char *atb_get_value(Tag *ref,int id){
    char *atb_begin = ref->atb[id];
    char *atb_value_begin = strchr(atb_begin,'"') + 1;
    char *atb_value_end = strchr(atb_value_begin,'"');
    size_t len = atb_value_end-atb_value_begin;

    char *value = (char*)malloc(len+1); // +1 for the null termiantor
    strncpy(value,atb_value_begin,len);
    
    return value;
}

//lê tag e armezna argumentos   
void read_tag(Lex *ref){ // recebe ponteiro do file, ponteiro para estrutura de tag, ponteiro para posição no arquivo

    // int word=0;
    // //zerar tag repassada
    // mytag->argq=0;
    // mytag->tag_type=TAG_TYPE_TAG;
    // char *buffer=NULL;

    // jump empty spaces
    while((ref->fileInMemory[ref->filePos]=='\r')|(ref->fileInMemory[ref->filePos]=='\n')|(ref->fileInMemory[ref->filePos]=='\t')|(ref->fileInMemory[ref->filePos]==' '))
        ref->filePos++;

    if(ref->fileInMemory[ref->filePos]=='<') // tag type
    {
        ref->type=LEX_TYPE_TAG;
        
        free(ref);
        

        ref->filePos++; // after "<"
        int i=0;

        while(ref->fileInMemory[ref->filePos]!='>')
        {
            ref /// 
            i++;
        }
    }
    else // value type
    {
        ref->type=LEX_TYPE_VALUE;
    }



    if(file[*pos]=='<'){
        // indica que tag to tipo tag 
        mytag->tag_type=TAG_TYPE_TAG;
        //aponta para catactere depois de "<"
        (*pos)++;
        //lê até o final da tag
        while(file[*pos]!='>'){
            if(file[*pos]=='\n'){ // conta linhas lidas
                (*line_cursor)++;
            }
            if((file[*pos]==9)|(file[*pos]==' ')|(file[*pos]==32)){ // separação por espaço
                mytag->tag_arg[mytag->argq][word]='\0'; // adiciona terminador
                mytag->argq++; // próxima linha
                word=0; // retorna pro inicio da linha
                (*pos)++; // próximo caractere do arquivo
            }
            mytag->tag_arg[mytag->argq][word]=file[*pos]; // pega caractere
            (*pos)++;
            word++;
        }
        //terminador
        mytag->tag_arg[mytag->argq][word]='\0';
        // verifica prefixos no nome da tag do tipo <ml:something> e muda para <something>, argq=0 indica o nome da tag
        if(strstr(mytag->tag_arg[0],"ml:")!=NULL)
        { 
            strcrop(mytag->tag_arg[0],"ml:");
        }
        else if(strstr(mytag->tag_arg[0],"pw:")!=NULL)
        { 
            strcrop(mytag->tag_arg[0],"pw:");
        }
        //aponta para catactere depois de ">"
        (*pos)++;
    }else if((file[*pos]==0)|(file[*pos]==13)|(file[*pos]=='\0')|(file[*pos]==3)){ // caracteres não válidos (<0), terminadores '\0' e especiais como (3) mas que estão ao final do arquivo 
        strcpy(mytag->tag_arg[mytag->argq],"the_end"); // determina fim do arquivo em *file
    }else{
        //tag de tipo valor
        mytag->tag_type=TAG_TYPE_VALUE;

        //percorre enquanto lê
        while((file[*pos]!=9)&(file[*pos]!='\n')&(file[*pos]!=' ')&(file[*pos]!='<')){
            if(file[*pos]=='\n'){ // conta linhas lidas
                (*line_cursor)++;
            }
            
            mytag->tag_arg[mytag->argq][word]=file[*pos]; // pega caractere
            (*pos)++;
            word++;
        }
        mytag->tag_arg[mytag->argq][word]='\0';
    }
    return;
}

Lex* LexNew(char *file_mem, int file_pos)
{
    Lex *entry = malloc(sizeof(*entry));
    entry->fileInMemory = file_mem;
    entry->filePos = file_pos;
    entry->type = LEX_TYPE_TAG;
    return entry;
}

//constructor 1
Tag* TagNew()
{
    Tag *entry = (Tag*)malloc(sizeof(*entry)); 
    //members
    memset(entry->atb,0,sizeof(entry->atb));
    entry->atbq=0;
    entry->atbUsePrefix = 1;
    //methods
    entry->atb_cmp=*atb_cmp;
    entry->atb_get=*atb_get;
    entry->atb_get_value=*atb_get_value;
    entry->read_tag=*read_tag;
    return entry;
}

//constructor 2
Tag* TagCopy(Tag* ref)
{
    Tag *entry = (Tag*)malloc(sizeof(Tag)); 
    //members
    memcpy(entry->atb,ref->atb,sizeof(entry->atb));
    entry->atbq=ref->atbq;
    entry->atbUsePrefix = ref->atbUsePrefix;
    //methods
    entry->atb_cmp=*atb_cmp;
    entry->atb_get=*atb_get;
    entry->atb_get_value=*atb_get_value;
    entry->read_tag=*read_tag;
    return entry;
}