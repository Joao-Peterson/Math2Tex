#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <zip.h>

#include "modules/data_structures/data_structures.h"
#include "modules/worksheet_parser/worksheet_parser.h"
#include "modules/results_parser/results_parser.h"
#include "modules/file_handle/file_handle.h"
#include "modules/xaml_parser/xaml_parser.h"

#define allocate(name,type) type *name=(type*)malloc(sizeof(type))
#define true 0
#define error 1

int main(int argc, char **argv){
    
    /* Argumentos do executavel */  
    if(argc!=2){
        printf("Sintaxe incorreta!");
        return 0;
    }

    /* Unzip arquivo mathcad*/
    char *unziped_path = filename_to_folder(argv[1]); // path principal para arquivo mathcad descompactado
    zip_extract(argv[1], unziped_path, on_extract_entry, NULL); // extraí
    
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

    cd_to(&handle, "mathcad");
    handle = opendir("xaml");

    /* Váriaveis parser*/
    int pos = 0;
    int cursor = 1;
    region myregion;
    worksheets *worksheets_xml=NULL;
    resultsList *result_xml=NULL;

    text_field *text_list = extract_docs(handle);
    if(text_list==NULL){
        log_to_console("error","Erro ao processar documento .XamlPackage",0,&cursor);
        return error;
    }
    strcat(worksheet_path,"worksheet.xml");   
    strcat(result_path,"result.xml");
    strcat(xaml_path,"xaml");
    ld_dir(handle);
    closedir(handle);

    print_text_field(&text_list);

    // /* Leitura e parser de arquivos */    
    // char *input = read_asci(result_path);
    // if(input==NULL){
    //     return 0;
    // }
    
    // if(parser_results_xml(input,&pos,&result_xml, &cursor)!=true){
    //     return 0;
    //     log_to_console("error","Erro ao fazer parser",0,&cursor);
    // }
    // log_to_console("done","Arquivo results.xml lido com sucesso",0,&cursor);

    // input = NULL;
    // pos=0;
    // cursor=1;
    // input = read_asci(worksheet_path);
    // if(input==NULL){
    //     return 0;
    // }

    // if(parse_worksheet_xml(input, &pos, &worksheets_xml, &result_xml, &cursor)!=true){
    //     return 0;
    // }
    // log_to_console("done","Arquivo worskheet.xml lido com sucesso",0,&cursor);

    // for(int i=0;i<9;i++){
    //     myregion = get_region(worksheets_xml,0,0,i);
    //     printf("Expressao [%d]: %s - Tipo: %s\n",i,myregion.expression,myregion.type);
    // }

    return 0;
}