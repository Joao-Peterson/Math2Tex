#include "worksheet_parser.h"
#include "../worksheet_operators/worksheet_operators.h"
#include "../xaml_parser/xaml_parser.h"
#include "../../include/types_config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define allocate(name,type) type *name=(type*)malloc(sizeof(type))
#define true 0
#define error 1

int img_counter = 0; // counts how many "imageble" fields are, that is, all regions of the xml except the text regions 

void img_count_add(){
    img_counter++;
}

int img_count_get(){
    return img_counter;
}

void img_counter_zero(){
    img_counter=0;
}

//parser de symEval
int symEval_parser(char *file, int *pos, char *expression, int *line_cursor){
    allocate(mytag,tag);
    read_tag(file,pos,mytag,line_cursor);

    strcat(expression,"symb("); // abre expressão symb()

    while (!atb_cmp(mytag,0,"/symEval"))
    {
        
        if(atb_cmp(mytag,0,"command"))
        {
            
            log_to_console("tag","<command>",0,line_cursor);
            read_tag(file,pos,mytag,line_cursor);

            while(!atb_cmp(mytag,0,"/command")){
                
                if(atb_cmp(mytag,0,"placeholder")){
                    strcat(expression,EMPTY_ARG); // coloca espaço caso não houver texto da operação de avaliação simbólica
                }else{
                    if(type_parser(file,pos,mytag,expression,line_cursor)!=true){
                        return error;
                    }
                }

                read_tag(file,pos,mytag,line_cursor);
            }

            log_to_console("/tag","</command>",0,line_cursor);

        }
        else if(atb_cmp(mytag,0,"symResult"))
        {

            log_to_console("tag","<symResult>",0,line_cursor);
            read_tag(file,pos,mytag,line_cursor);

            while (!atb_cmp(mytag,0,"/symResult")){
                
                if(type_parser(file,pos,mytag,expression,line_cursor)!=true){ // identitifica tipo genérico
                    return error;
                }

                read_tag(file,pos,mytag,line_cursor);
            }
            
        }
        else
        {

            if(type_parser(file,pos,mytag,expression,line_cursor)!=true){ // identitifica tipo genérico
                return error;
            }

        }

        strcat(expression,","); // separador
        read_tag(file,pos,mytag,line_cursor);
    }

    strdel_last(expression,1); // tira ultima virgula posta
    strcat(expression,")"); // fecha expressão symb()    

    log_to_console("/tag","</symEval>",0,line_cursor);
    return true;
}

// define parser
int define_parser(char *file, int *pos, char *expression, int result_ref, resultsList **resultlist_ref, int *line_cursor){
    allocate(mytag,tag);

    log_to_console("tag","<define>",0,line_cursor);
            
    strcat(expression,"def("); // adiciona abertura de definição à expressão, strcpy pois é primeiro argumento da expressão
    
    read_tag(file, pos, mytag, line_cursor);
    
    while(!atb_cmp(mytag,0,"/define")){ // espera fechamento de <define>
        
        if(atb_cmp(mytag,0,"eval"))// tipo eval 
        { 
            
            if (eval_parser(file,pos,expression,result_ref,resultlist_ref,line_cursor)==error){
                log_to_console("error","Erro ao ler tag <eval>",0,0);
                return error;
            }
           
        }
        else 
        {
            if (type_parser(file,pos,mytag,expression,line_cursor)!=true) // identitifica tipo genérico
            { 
                return error;
            }
        } 
        
        strcat(expression,","); // adiciona virgula após tipo
            
        read_tag(file, pos, mytag, line_cursor);
    }

    strdel_last(expression,1); // deleta ultima virgula 
    strcat(expression,")"); // fecha definição

    return true;
}

// eval parser
int eval_parser(char *file, int *pos, char *expression, int result_ref, resultsList **resultlist_ref, int *line_cursor){
    float evaluation_result = 0.0;
    char evaluation_result_string[50];
    allocate(mytag,tag);

    log_to_console("tag","<eval>",0,line_cursor);

    strcat(expression,"eval("); // abre expressão eval

    read_tag(file,pos,mytag,line_cursor);

    while(!atb_cmp(mytag,0,"/eval")){ // espera fechamento

        if(atb_cmp(mytag,0,"unitOverride")){

            log_to_console("tag","<unitOverride>",0,line_cursor);
            read_tag(file,pos,mytag,line_cursor);

            while(!atb_cmp(mytag,0,"/unitOverride")){
                
                if(atb_cmp(mytag,0,"placeholder")){ // resolve a tag para o valor correto
                    evaluation_result = get_result(resultlist_ref,result_ref); // pega valor
                    sprintf(evaluation_result_string,"\"%.15f\"",evaluation_result); // faz para string
                    strcat(expression,evaluation_result_string); // concatena valor
                }

                read_tag(file,pos,mytag,line_cursor);
            }

            log_to_console("tag","</unitOverride>",0,line_cursor);

        }
        else if(atb_cmp(mytag,0,"define")) // tipo definição
        { 
            if (define_parser(file,pos,expression,result_ref,resultlist_ref,line_cursor)==error){
                log_to_console("error","Erro ao ler tag <define>",0,0);
                return error;
            }
        }
        else
        {
            if(type_parser(file,pos,mytag,expression,line_cursor)!=true){ // identitifica tipo genérico
                return error;
            }
        }

        strcat(expression,","); // separador

        read_tag(file,pos,mytag,line_cursor);
    }

    strdel_last(expression,1); // deletea ultima virgula posta
    strcat(expression,")"); // fecha expressão eval

    return true;
}

//parser de matemática
int math_parser(char *file, int *pos, char *expression, int result_ref, resultsList **resultlist_ref, int *line_cursor){
    allocate(mytag,tag);
    read_tag(file, pos, mytag, line_cursor);

    while(!atb_cmp(mytag,0,"/math")){ // espera fechamento

        if(atb_cmp(mytag,0,"define")){ // tipo definição

            if (define_parser(file,pos,expression,result_ref,resultlist_ref,line_cursor)==error){
                log_to_console("error","Erro ao ler tag <define>",0,0);
                return error;
            }

        }else if(atb_cmp(mytag,0,"eval")){ // tipo eval 
            
            if (eval_parser(file,pos,expression,result_ref,resultlist_ref,line_cursor)==error){
                log_to_console("error","Erro ao ler tag <eval>",0,0);
                return error;
            }
            
        }else if(atb_cmp(mytag,0,"resultFormat")){ // tag <resultFormat> com função desconhecida
            read_tag(file,pos,mytag,line_cursor);
            while(!atb_cmp(mytag,0,"/resultFormat")){
                read_tag(file,pos,mytag,line_cursor);
            }
        }else{ // casos do tipo type_parser
            if(type_parser(file, pos, mytag, expression, line_cursor)!=true){
                return error;
            }
        }
         
        read_tag(file, pos, mytag, line_cursor);
    }

    log_to_console("/tag","</math>",0,line_cursor);
    return true;
}

//text parser, write to expression the text read from the .xaml files in mathcad/Xaml processed to swap the placeholder's by the actual symbols and fields in the text field
int text_parser(char *file, int *pos, char *expression, int result_ref, text_field **text_list, resultsList **resultslist_ref, img_list **img_list_ref, int *line_cursor){
    allocate(mytag,tag);
    regions *myregions = NULL;

    read_tag(file,pos,mytag,line_cursor);
    while(!atb_cmp(mytag,0,"/text")){ // read <text> body

        if(atb_cmp(mytag,0,"regions")){ // parse regions
            if(parse_regions(file,pos,&myregions,mytag,text_list,resultslist_ref,img_list_ref,line_cursor)!=true){
                log_to_console("error","Erro ao processar tag <regions>",0,line_cursor);
                return error;
            }
        }

        read_tag(file,pos,mytag,line_cursor);
    }

    char *raw_text = (char*)malloc(sizeof(char)*REGION_EXPRESSION_LEN_DEFAULT);
    char *buffer = (char*)malloc(sizeof(char)*REGION_EXPRESSION_LEN_DEFAULT); 
    strncpy(raw_text,get_text_field(text_list, result_ref),REGION_EXPRESSION_LEN_DEFAULT); // raw text from xaml parser
    int placeholder_offset = strlen(TEXT_PLACE_HOLDER)*sizeof(char); // lenght of placeholder
    char *token_start;
    char *token_end;

    regions *cursor = myregions;
    while(cursor!=NULL){  // substitute placeholders running trought the linked list
        token_start = strstr(raw_text,TEXT_PLACE_HOLDER); // find placeholder
        token_end = token_start + placeholder_offset;
        strncpy(buffer,token_end,strlen(token_end)+1); // save string after the token, +1 to acomodate the '\0' at the end
        snprintf(token_start,strlen(cursor->region_data->expression)+1,"%s",cursor->region_data->expression); // substitute the place holder
        //strncpy(token_start,cursor->region_data->expression,strlen(cursor->region_data->expression)+1); 
        strcat(token_start,buffer); // bring back the rest 
 
        cursor=cursor->next;
    }

    strncpy(expression,raw_text,strlen(raw_text)+1); // pass to the main expression
    free(buffer);
    free(raw_text);

    return true;
}

//parser de region
int parse_region(char *file, int *pos, region **region_obj, tag *header, text_field **text_list, resultsList **resultlist_ref, img_list **img_list_ref, int *line_cursor){
    allocate(mytag,tag);
    allocate(myregion,region); // objeto a ser montado e devolvido
    read_tag(file, pos, mytag, line_cursor);
    
    myregion->tags=*header; // recebe tags lidas anteriormente
    myregion->region_id=atoi(atb_read_value(myregion->tags.tag_arg[1])); // parametro "region_id" recebe inteiro da string que é valor do atributo de numero "1" correpondente ao atributo "region-id" da tag <region>
    myregion->size[0]=strtof(atb_read_value(myregion->tags.tag_arg[2]),NULL); // lê largura da tag
    myregion->size[1]=strtof(atb_read_value(myregion->tags.tag_arg[3]),NULL); // lê altura da tag
    if(myregion->tags.argq>=5){
        myregion->pos[0]=strtof(atb_read_value(myregion->tags.tag_arg[4]),NULL); // parametro pos[top] recebe string para float do valor do argumento da tag <region> "top", 4 da lista de argumentos
        myregion->pos[1]=strtof(atb_read_value(myregion->tags.tag_arg[5]),NULL); // parametro pos[left] recebe string para float do valor do argumento da tag <region> "leaft", 5 da lista de argumentos
    }
    myregion->expression[0]='\0';

    while(!atb_cmp(mytag, 0, "/region"))
    {
        if(atb_cmp(mytag,0,"math")) // MATH TAG
        { 
            log_to_console("tag","<math>",0,line_cursor);

            strcpy(myregion->type,"math"); // save type
            myregion->resultRef = atoi(atb_read_value(atb_get(mytag,1))); // picks the "resultRef" id from <math> for evaluation crossreference 
            
            if(math_parser(file, pos, (char*)&myregion->expression, myregion->resultRef, resultlist_ref, line_cursor)!=true){
                log_to_console("error","Erro ao processar tag <math>",myregion->region_id,line_cursor);
                return error;
            }

            img_count_add(); // as math is a image field in rtf files, it must me counted
        }
        else if(atb_cmp(mytag,0,"text")) // TEXT TAG
        { 
            log_to_console("tag","<text>",0,line_cursor);

            strcpy(myregion->type,"text"); // save type
            myregion->resultRef = atoi(atb_read_value(atb_get(header,1))); // pick the <region> caller "region-id", necessary to crossreference with xaml text
            
            if(text_parser(file, pos, (char*)&myregion->expression, myregion->resultRef, text_list, resultlist_ref, img_list_ref, line_cursor)!=true){
                log_to_console("error","Erro ao processar tag <text>",myregion->region_id,line_cursor);
                return error;
            }

        }
        else if(atb_cmp(mytag,0,"plot")) // PLOT TAG
        { 
            img_count_add(); // as plot is a image field in rtf files, it must me counted, and before picking the image 

            snprintf(myregion->expression,REGION_EXPRESSION_LEN_DEFAULT,"img(\"%s\")",img_get_path(img_list_ref,img_count_get()-1) ); // add the field img expression, "img_count_get()-1" because "img_count" counts how many, but the index starts in 0, so 9 images, the last one will be the 8 one

            strcpy(myregion->type,"image"); // save type

            read_tag(file, pos, mytag, line_cursor);
            while(!atb_cmp(mytag,0,"/plot"))
                read_tag(file, pos, mytag, line_cursor);
        }
        else if (atb_cmp(mytag,0,"chartComponent"))
        {
            img_count_add(); // as plot is a image field in rtf files, it must me counted, and before picking the image 

            snprintf(myregion->expression,REGION_EXPRESSION_LEN_DEFAULT,"img(\"%s\")",img_get_path(img_list_ref,img_count_get()-1) ); // add the field img expression, "img_count_get()-1" because "img_count" counts how many, but the index starts in 0, so 9 images, the last one will be the 8 one

            strcpy(myregion->type,"image"); // save type

            read_tag(file, pos, mytag, line_cursor);
            while(!atb_cmp(mytag,0,"/chartComponent"))
                read_tag(file, pos, mytag, line_cursor);
        }
        else if (atb_cmp(mytag,0,"picture"))
        {
            img_count_add(); // as plot is a image field in rtf files, it must me counted, and before picking the image 

            snprintf(myregion->expression,REGION_EXPRESSION_LEN_DEFAULT,"img(\"%s\")",img_get_path(img_list_ref,img_count_get()-1) ); // add the field img expression, "img_count_get()-1" because "img_count" counts how many, but the index starts in 0, so 9 images, the last one will be the 8 one

            strcpy(myregion->type,"image"); // save type

            read_tag(file, pos, mytag, line_cursor);
            while(!atb_cmp(mytag,0,"/picture"))
                read_tag(file, pos, mytag, line_cursor);
        }
        else
        {
            log_to_console("error","Nenhuma tag valida dentro da regiao",0,line_cursor);
            return error;
        }
         
        read_tag(file, pos, mytag, line_cursor);
    }

    *region_obj=myregion; // devolve ponteiro para objeto repassado
    log_to_console("/tag","</region>",0,line_cursor);
    return true;
}

//parser de regions
int parse_regions(char *file, int *pos, regions **regions_obj, tag *header, text_field **text_list, resultsList **resultlist_ref, img_list **img_list_ref, int *line_cursor){
    int tag_counter=0;
    allocate(mytag,tag);// tag temporária
    region *myregion; // objeto a ser processado
    regions *regions_tmp = NULL; // objeto a ser montado e devolvido
    read_tag(file, pos, mytag, line_cursor);

    while(!atb_cmp(mytag, 0, "/regions")){
        if(!atb_cmp(mytag, 0, "region")){
            log_to_console("error","Tag <region> esperada!",0,line_cursor);
            return error;
        }

        log_to_console("tag","<region>",tag_counter,line_cursor);

        if(parse_region(file, pos, &myregion, mytag, text_list, resultlist_ref, img_list_ref, line_cursor)!=true){
            return error;
        }
        
        add_region(&regions_tmp, myregion);
         
        tag_counter++;
        read_tag(file, pos, mytag, line_cursor);
    }

    regions_tmp->tags=*header; // recebe tags lidas anteriormente

    *regions_obj=regions_tmp; // devolve ponteiro para lista de tipo regions
    log_to_console("/tag","</regions>",0,line_cursor);
    return true;
}

//parser de worksheet.xml , regiões de expressões e avaliações, mas não cálculos numéricos
int parse_worksheet(char *file, int *pos, worksheet **worksheet_obj, tag *header, text_field **text_list, resultsList **resultlist_ref, img_list **img_list_ref, int *line_cursor){
    int tag_counter=0; //contador de tags
    allocate(mytag,tag); // tag temporária
    regions *myregions; // objeto a ser processado, ponteiro para cabeça da lista regions
    worksheet *worksheet_tmp = NULL; // objeto a ser alocado, montado e devolvido, ponteiro para 
    read_tag(file, pos, mytag, line_cursor); // lê primeira tag

    while(!atb_cmp(mytag, 0, "/worksheet")){ // enquanto houverem tags <regions>
        if(!atb_cmp(mytag, 0, "regions")){ // verifica se é tipo regions
            log_to_console("error","Tag <regions> esperada",0,line_cursor);
            return error; 
        }

        log_to_console("tag","<regions>",tag_counter,line_cursor); // deu boa
        
        myregions=NULL;
        if(parse_regions(file, pos, &myregions, mytag, text_list, resultlist_ref, img_list_ref, line_cursor)!=true){ // realizar parse da regions e verificar se deu boa ou não
            return error;
        }
        
        add_regions(&worksheet_tmp, myregions); // adiciona regions a worksheet

        tag_counter++;
        read_tag(file, pos, mytag, line_cursor); // lê próxima
    }

    worksheet_tmp->tags=*header; // adiciona a tag e os argumentos ao worksheet, lidos anteriormente

    *worksheet_obj=worksheet_tmp; // devolve ponteiro da worksheet alocada para a repassada    
    log_to_console("/tag","</worksheet>",0,line_cursor);

    return true;
}

//parse do arquivo worksheet.xml
worksheets *parse_worksheet_xml(char *file, int *pos, text_field **text_list, resultsList **resultlist_ref, img_list **img_list_ref, int *line_cursor){
    int tag_counter=0; // contador de tags
    allocate(mytag,tag);
    worksheet *myworksheet; // cria worksheet para ser armazenada posteriormente
    worksheets *worksheets_tmp = NULL; // objeto a ser montado e devolvido
    read_tag(file, pos, mytag, line_cursor); // lê primeira tag

    img_counter_zero(); // zera contagem de imagens a cada worksheet lido

    while(!atb_cmp(mytag, 0, "the_end")){ // verifica fim do arquivo
        //verifica se é a tag esperada
        if(atb_cmp(mytag,0,"worksheet")){ // verifica se é tipo worksheet
            
            log_to_console("tag","<worksheet>",tag_counter,line_cursor);
            // processa a tag, myworksheet retorna com tag e regions embutida
            if(parse_worksheet(file, pos, &myworksheet, mytag, text_list, resultlist_ref, img_list_ref, line_cursor)!=true){ // chama parse para tag worksheet e verifica se foi feita com sucesso
                return NULL;
            }

            // adiciona worksheet para lista worksheets
            add_worksheet(&worksheets_tmp, myworksheet);

            // lê para próxima iteração
            tag_counter++;
        }

        read_tag(file, pos, mytag, line_cursor); 
    }
    
    return worksheets_tmp; // recebe ponteiro de novo
}