#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "modules/worksheet_parser/worksheet_parser.h"
#include "modules/results_parser/results_parser.h"

#define allocate(name,type) type *name=(type*)malloc(sizeof(type))
#define true 0
#define error 1

int main(int argc, char **argv){
    int pos = 0;
    int cursor = 1;
    region myregion;
    worksheets *worksheets_xml=NULL;
    resultsList *result_xml=NULL;
    
    /* Argumentos do executavel */
    /*
    if(argc<2){
        printf("Especifique um arquivo!");
        return 0;
    }
    char *input = read_text(argv[1]); 
    */

    /* Leitura e parser de arquivos */    
    char *input = read_asci("test/result.xml");
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
    input = read_asci("test/worksheet.xml");
    if(input==NULL){
        return 0;
    }

    if(parse_worksheet_xml(input, &pos, &worksheets_xml, &result_xml, &cursor)!=true){
        return 0;
    }
    log_to_console("done","Arquivo results.xml lido com sucesso",0,&cursor);

    myregion=get_region(worksheets_xml,0,0,10);
    printf("Expressao [1]: %s - Tipo: %s\n",myregion.expression,myregion.type);

    // myregion=get_region(worksheets_xml,0,0,1);
    // printf("Expressao [2]: %s - Tipo: %s\n",myregion.expression,myregion.type);

    return 0;
}