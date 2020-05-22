#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <zip.h>

#include "modules/worksheet_parser/worksheet_parser.h"
#include "modules/results_parser/results_parser.h"

#define allocate(name,type) type *name=(type*)malloc(sizeof(type))
#define true 0
#define error 1

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
    static int i = 0;
    int n = *(int *)arg;
    printf("Extracted: %s (%d of %d)\n", filename, ++i, n);

    return 0;
}

void cd_to(DIR **handle, const char *filename){
    struct dirent *entry = NULL;
    char *buffer = (char*)malloc(sizeof(char)*50);
    while((entry=readdir(*handle))!=NULL){
        printf(">> %s\n",entry->d_name);
        if(!strcmp(entry->d_name,filename)){  
            printf("Mudando diretorio para : %s\n",entry->d_name);
            //chdir(entry->d_name);
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

int main(int argc, char **argv){
    
    /* Argumentos do executavel */  
    if(argc!=2){
        printf("Sintaxe incorreta!");
        return 0;
    }

    /* Unzip arquivo mathcad*/
    int file_qtd = 1; // apenas um arquivo
    char *unziped_path = filename_to_folder(argv[1]); // path principal para arquivo mathcad descompactado
    zip_extract(argv[1], unziped_path, on_extract_entry, &file_qtd); // extraí
    
    /* Descobrindo arquivos internos*/
    char worksheet_path[200]={0};
    char result_path[200]={0};
    char xaml_path[200]={0};
    char buffer[200]={0};
    DIR *handle = opendir(unziped_path);
    struct dirent *entry;
    chdir(unziped_path); // vai para path de interesse
    if(handle==NULL){
        printf("Diretorio invalido!\n");
        return 0;
    }

    /* Diretórios de interesse */
    cd_to(&handle, "mathcad");
    //strncpy(worksheet_path,unziped_path,200);
    strcat(worksheet_path,"mathcad/worksheet.xml");
    //strncpy(result_path,unziped_path,200);    
    strcat(result_path,"mathcad/result.xml");
    //strncpy(xaml_path,unziped_path,200);
    strcat(xaml_path,"mathcad/xaml");
    ld_dir(handle);
    closedir(handle);

    /* Váriaveis parser*/
    int pos = 0;
    int cursor = 1;
    region myregion;
    worksheets *worksheets_xml=NULL;
    resultsList *result_xml=NULL;

    /* Leitura e parser de arquivos */    
    char *input = read_asci(result_path);
    if(input==NULL){
        return 0;
    }
    
    if(parser_results_xml(input,&pos,&result_xml, &cursor)!=true){
        return 0;
        log_to_console("error","Erro ao fazer parser",0,&cursor);
    }
    log_to_console("done","Arquivo lido com sucesso",0,&cursor);

    input = NULL;
    pos=0;
    cursor=1;
    input = read_asci(worksheet_path);
    if(input==NULL){
        return 0;
    }

    if(parse_worksheet_xml(input, &pos, &worksheets_xml, &result_xml, &cursor)!=true){
        return 0;
    }
    log_to_console("done","Arquivo results.xml lido com sucesso",0,&cursor);

    for(int i=0;i<9;i++){
        myregion = get_region(worksheets_xml,0,0,i);
        printf("Expressao [%d]: %s - Tipo: %s\n",i,myregion.expression,myregion.type);
    }

    return 0;
}