#include "results_parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define allocate(name,type) type *name=(type*)malloc(sizeof(type))
#define true 0
#define error 1

void stringdata2floatarray(char* expression1, char* expression2, char* expression3,  plotdata **plotdata_ref){
    float value_tmp[3];

    // ATENÇÃO, REFAZER ESTA MERDA, A FUNÇÃO read_tag()  NÃO AGUENTA LER ESSES VETOR DO DJANHO DE 15000 LINHAS EM UM VETOR ALOCADO NA STACK
    // ASSIM, REFAZER A read_tag OU ACHAR OUTRO JEITO DE FAZER, ASSIM, PARA EVITAR QUE ESSA FUNÇÃO RETORNE MERDA E QUITTAR O PROGRAMA, VERIFICASE SE 
    // VETORES ESTÃO LIDOS, VERIFICANDO SE A EXPRESSÃO 2 NÃO É NULA, E SAINDO DA FUNÇÃO.
    if(expression2[0]!='\0') 
    {
        add_plotdata(plotdata_ref,0,0,0);
        return;
    }

    while(1){
        if((*expression1)=='['){ // avança primeiro '['
            expression1++; 
            expression2++;
            expression3++;
        }

        value_tmp[0]=strtof(expression1,&expression1);
        value_tmp[1]=strtof(expression2,&expression2);
        value_tmp[2]=strtof(expression3,&expression3);

        add_plotdata(plotdata_ref, value_tmp[0], value_tmp[1], value_tmp[2]);

        if(((*expression1)==']')|((*expression1)=='\0')){
            break;
        }else{
            expression1++; 
            expression2++;
            expression3++;
        }
    }
}

resultsList *parser_results_xml(char *file, int *pos, int *line_cursor){
    allocate(mytag,tag);
    //variaveis para futuro result
    plotdata *node = NULL;
    resultsList *resultslist_ref=NULL;
    int id_temp;
    char type_temp[25];
    float value_temp;
    char *temp_vector_1 = malloc(sizeof(*temp_vector_1)*DATA_POINTS_CHAR_MAX+1);
    char *temp_vector_2 = malloc(sizeof(*temp_vector_2)*DATA_POINTS_CHAR_MAX+1);
    char *temp_vector_3 = malloc(sizeof(*temp_vector_3)*DATA_POINTS_CHAR_MAX+1);
    int i=0; // contagem de vetores de valores

    read_tag(file,pos,mytag,line_cursor); // lê tag <xml> 
    read_tag(file,pos,mytag,line_cursor); // lê <resultsList>
    read_tag(file,pos,mytag,line_cursor); // lê primeira <resultData>

    if (!atb_cmp(mytag,0,"resultData"))
    {
        resultslist_ref = malloc(sizeof(resultsList));
        resultslist_ref->id=-1; // to indicate that there is none results to be read
        return resultslist_ref;
    }

    while(!atb_cmp(mytag,0,"/resultsList")){ // espera fechamento </resultsList>
        log_to_console("tag","<ResultData>",atoi(atb_read_value(atb_get(mytag,2))),line_cursor);

        if(!atb_cmp(mytag,mytag->argq,"/")){ // caso tag for auto fechada não há dado
            id_temp=atoi(atb_read_value(atb_get(mytag,1))); // retorna inteiro de string que é valor do segundo argumento da tag, o result-id="N"
            read_tag(file,pos,mytag,line_cursor);
            i=0; // zera contador antes de armazenar os vetores

            while(!atb_cmp(mytag,0,"/resultData")){ // espera fechamento </resultData>

                if(atb_cmp(mytag,0,"Trace2dResult")){
                    log_to_console("tag","<Trace2dResult>",0,line_cursor);
                    strcpy(type_temp,"2dplot");
                }else if(atb_cmp(mytag,0,"result")){
                    log_to_console("tag","<result>",0,line_cursor);
                    strcpy(type_temp,"result");
                }else if(mytag->tag_type==TAG_TYPE_VALUE){
                    switch (i)
                    {
                    case 0:
                        strncpy(temp_vector_1,atb_get(mytag,0),DATA_POINTS_CHAR_MAX);
                        break;
                    case 1:
                        strncpy(temp_vector_2,atb_get(mytag,0),DATA_POINTS_CHAR_MAX);
                        break;
                    case 2:
                        strncpy(temp_vector_3,atb_get(mytag,0),DATA_POINTS_CHAR_MAX);
                        break;
                    default:
                        break;
                    }

                    i++;
                }

                read_tag(file,pos,mytag,line_cursor); // a cada </resultData> cria novo nó de resultsList
            }

            i=0; // zera contagem de strings de valores lidos
            node=NULL; // null a cada conversão para forçar alocação mais adiante
            stringdata2floatarray(temp_vector_1,temp_vector_2,temp_vector_3,&node); // converte e retorna vetor de floats organizado
            
            temp_vector_1[0] = '\0';
            temp_vector_2[0] = '\0';
            temp_vector_3[0] = '\0';
            
            add_result(&resultslist_ref,id_temp,type_temp,node);

        }else{
            id_temp=atoi(atb_read_value(atb_get(mytag,1))); // retorna inteiro de string que é valor do segundo argumento da tag, o result-id="N"
            add_result(&resultslist_ref,id_temp,"empty",NULL);
        }
        
        read_tag(file,pos,mytag,line_cursor);
    }

    free(temp_vector_1);
    free(temp_vector_2);
    free(temp_vector_3);

    return resultslist_ref;
}