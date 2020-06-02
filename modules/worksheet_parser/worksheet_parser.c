#include "worksheet_parser.h"
#include "../xaml_parser/xaml_parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define allocate(name,type) type *name=(type*)malloc(sizeof(type))
#define true 0
#define error 1

//parser de tags genéricas que, para o propósito deste programa, melhor serem ignoradas
int parser_generic(char *file, int *pos, char *tag_name, char *expression, int *line_cursor){
    char *tag_closing = (char*)malloc(sizeof(char)*(strlen(tag_name)+10));
    tag_closing[0]='\0';
    char *tag_brackets = (char*)malloc(sizeof(char)*(strlen(tag_name)+10));
    tag_brackets[0]='\0';
    strcat(tag_closing,"/");
    strcat(tag_closing,tag_name);
    strcat(tag_brackets,"<");
    strcat(tag_brackets,tag_name);
    strcat(tag_brackets,">");
    log_to_console("tag",tag_brackets,0,line_cursor);
    
    allocate(mytag,tag);
    read_tag(file,pos,mytag,line_cursor);

    while(!atb_cmp(mytag,0,tag_closing)){

        if(type_parser(file,pos,mytag,expression,line_cursor)!=true){
            return error;
        }

        strcat(expression,",");
        read_tag(file,pos,mytag,line_cursor);
    }

    tag_brackets[0]='\0';
    strcat(tag_brackets,"</");
    strcat(tag_brackets,tag_name);
    strcat(tag_brackets,">");

    log_to_console("/tag",tag_brackets,0,line_cursor);
    strdel_last(expression,1);
    return true;
} 

//parser de real
int real_parser(char *file, int *pos, char *expression, int *line_cursor){
    allocate(mytag,tag);                
    read_tag(file, pos, mytag, line_cursor);

    while(!atb_cmp(mytag, 0, "/real")){
        if(mytag->tag_type!=TAG_TYPE_VALUE){
            log_to_console("error","Esperado valor após <real>",0,line_cursor);
            return error;
        }

        strcat(expression, atb_get(mytag,0)); // concatena na expressão o valor real lido
                         
        read_tag(file, pos, mytag, line_cursor);
    }

    log_to_console("/tag","</real>",0,line_cursor);
    
    return true;
}

//parser de parenteses
int parens_parser(char *file, int *pos, char *expression, int *line_cursor){
    allocate(mytag,tag);
    read_tag(file,pos,mytag,line_cursor);
            
    while(!atb_cmp(mytag,0,"/parens")){

        if(type_parser(file,pos,mytag,expression,line_cursor)!=true){ // identitifica tipo genérico
            return error;
        }

        read_tag(file,pos,mytag,line_cursor);
    }

    log_to_console("tag","</parens>",0,line_cursor);
    return true;
}

//parser de neg
int neg_parser(char *file, int *pos, char *expression, int *line_cursor){
    allocate(mytag,tag);
    read_tag(file,pos,mytag,line_cursor);

    strcat(expression, "neg("); // abre parenteses

    if(type_parser(file,pos,mytag,expression,line_cursor)!=true){ // identitifica tipo genérico
        return error;
    }

    strcat(expression,")"); // fecha expressão

    return true;
}

// parser de apply
int apply_parser(char *file, int *pos, char *expression, int *line_cursor){
    allocate(mytag,tag);
    read_tag(file, pos, mytag, line_cursor);

     // se iniciar com operador é uma operação
    if (atb_cmp(mytag, 0, "plus") | atb_cmp(mytag, 0, "minus") | atb_cmp(mytag, 0, "mult") | atb_cmp(mytag, 0, "div") | atb_cmp(mytag, 0, "pow") | atb_cmp(mytag, 0, "equal")|atb_cmp(mytag,0,"parens")|atb_cmp(mytag,0,"neg")|atb_cmp(mytag,0,"limit")|atb_cmp(mytag,0,"derivative")|(atb_cmp(mytag,0,"integral")))
    {
        if (atb_cmp(mytag, 0, "plus"))
        { // verifica qual operador matemático e adiciona a expressão
            strcat(expression, "add(");
            log_to_console("tag", "<plus>", 0, line_cursor);
        }
        else if (atb_cmp(mytag, 0, "minus"))
        {
            strcat(expression, "sub(");
            log_to_console("tag", "<minus>", 0, line_cursor);
        }
        else if (atb_cmp(mytag, 0, "mult"))
        {
            strcat(expression, "mul(");
            log_to_console("tag", "<mult>", 0, line_cursor);
        }
        else if (atb_cmp(mytag, 0, "div"))
        {
            strcat(expression, "div(");
            log_to_console("tag", "<div>", 0, line_cursor);
        }
        else if (atb_cmp(mytag, 0, "pow"))
        {
            strcat(expression, "pow(");
            log_to_console("tag", "<pow>", 0, line_cursor);
        }
        else if (atb_cmp(mytag, 0, "equal"))
        {
            strcat(expression, "equal(");
            log_to_console("tag", "<equal>", 0, line_cursor);
        }
        else if (atb_cmp(mytag, 0, "parens"))
        {
            strcat(expression, "parens(");
            log_to_console("tag", "<parens>", 0, line_cursor);
        }
        else if (atb_cmp(mytag, 0, "neg"))
        {
            strcat(expression, "neg(");
            log_to_console("tag", "<neg>", 0, line_cursor);
        }
        else if (atb_cmp(mytag,0,"limit"))
        {
            strcat(expression,"lim(");
            log_to_console("tag","<limit>",0,line_cursor);
        }
        else if (atb_cmp(mytag,0,"derivative"))
        {
            strcat(expression,"deri(");
            log_to_console("tag","<derivative>",0,line_cursor);
        }
        else if (atb_cmp(mytag,0,"integral"))
        {
            strcat(expression,"int(");
            log_to_console("tag","<integral>",0,line_cursor);
        }
        
        read_tag(file,pos,mytag,line_cursor); // lê nova tag após tag de operação matemática
    }
    else // se não, é uma aplicação da forma. EX. V(3).
    {
        strcat(expression, "func(");
    }

    while(!atb_cmp(mytag,0,"/apply")){ // espera tag de fechamento

        if(type_parser(file,pos,mytag,expression,line_cursor)!=true){ // identitifica tipo genérico
            return error;
        }

        strcat(expression,","); // separador

        read_tag(file, pos, mytag, line_cursor);
    }

    strdel_last(expression,1); // tira ultima virgula colocada
    strcat(expression,")"); // fecha expressão

    log_to_console("/tag","</apply>",0,line_cursor);
    return true;
}

//parser de range
int range_parser(char *file, int *pos, char *expression, int *line_cursor){
    allocate(mytag,tag);
    read_tag(file,pos,mytag,line_cursor);

    strcat(expression,"rang("); // abre expressão range

    while(!atb_cmp(mytag,0,"/range")){
        
        if(atb_cmp(mytag,0,"sequence")){ // primeira tag sequência
            
            log_to_console("tag","<sequence>",0,line_cursor);

            read_tag(file,pos,mytag,line_cursor);

            while(!atb_cmp(mytag,0,"/sequence")){ // espera </sequence>

                if(type_parser(file,pos,mytag,expression,line_cursor)!=true){ // identitifica tipo genérico
                    return error;
                }

                strcat(expression,","); // adiciona virgula a cada leitura dentro de <sequence>

                read_tag(file,pos,mytag,line_cursor);
            }

            // não se colcoa outra virgula pois acima já adicionou dentro do while

        }else{

            if(type_parser(file,pos,mytag,expression,line_cursor)!=true){ // identitifica tipo genérico
                return error;
            }

        }

        read_tag(file,pos,mytag,line_cursor);
    }

    strdel_last(expression,1); // remove ultima virgula colcoada
    strcat(expression,")"); // fecha expressão range
    log_to_console("/tag","</range>",0,line_cursor);
    return true;
}

//parser de id
int id_parser(char *file, int *pos, char *expression, int *line_cursor){
    char var[50]={0}; // temp para variavel
    char subs[50]={0}; // temp para subescrito
    char upper[50]={0}; // temp para superscrito
    allocate(mytag,tag);
     
    read_tag(file, pos, mytag, line_cursor); // lê primeira tag dentro de <math>

    while(!atb_cmp(mytag,0,"/id")){ // espera tag de fechamento
        if(mytag->tag_type==TAG_TYPE_VALUE){ // se inicia com variavel(value), guardar ele
            strcat(expression,atb_get(mytag,0)); // copia para buffer
        }else if(atb_cmp(mytag, 0, "Span")){ // senão, então esperamos uma tag <Span> que contem a variavel
            log_to_console("tag","<Span>",0,line_cursor);
              // necessário a cada leitura
            read_tag(file, pos, mytag, line_cursor); // lê de novo
            while(!atb_cmp(mytag, 0, "/Span")){ // espera fechamento da tag
                
                // Valor da variavel 
                if(mytag->tag_type!=TAG_TYPE_VALUE){ // espera uma variavel
                    log_to_console("error","Variavel esperada",0,line_cursor);
                return error;
                }

                strcpy(var,atb_get(mytag,0)); // pega variavel

                // Subescrito
                 
                read_tag(file, pos, mytag, line_cursor);
                if(atb_cmp(mytag, 0, "Subscript")){
                     
                    read_tag(file, pos, mytag, line_cursor);
                    while(!atb_cmp(mytag, 0, "/Subscript")){
                        if(mytag->tag_type!=TAG_TYPE_VALUE){
                            log_to_console("error","Variavel esperada",0,line_cursor);
                            return error;
                        }

                        strcpy(subs,atb_get(mytag,0)); // pega subscrito
                         
                        read_tag(file, pos, mytag, line_cursor);
                    }
                }

                // Superscrito
                if(atb_cmp(mytag, 0, "Upperscript")){
                     
                    read_tag(file, pos, mytag, line_cursor);
                    while(!atb_cmp(mytag, 0, "/Upperscript")){
                        if(mytag->tag_type!=TAG_TYPE_VALUE){
                            log_to_console("error","Variavel esperada",0,line_cursor);
                            return error;
                        }

                        strcpy(upper,atb_get(mytag,0)); // pega subscrito
                         
                        read_tag(file, pos, mytag, line_cursor);
                    }
                }

                strcat(expression, "leg("); // adiciona legenda a expressão
                strcat(expression, var); // adiciona variavel
                strcat(expression, ","); // separação virgula
                strcat(expression, subs); // adiciona variavel
                strcat(expression, ","); // separação virgula
                strcat(expression, upper); // adiciona variavel
                strcat(expression, ")"); // fecha legenda

                 
                read_tag(file, pos, mytag, line_cursor);
            }

            log_to_console("/tag","</Span>",0,line_cursor);
        }else{
            log_to_console("error","Tag <Span> ou variavel esperada>",0,line_cursor);
            return error;
        }
         
        read_tag(file, pos, mytag, line_cursor);
    }

    log_to_console("/tag","</id>",0,line_cursor);
    return true;
}

//parser de function
int function_parser(char *file, int *pos, char *expression, int *line_cursor){
    allocate(mytag,tag);
    read_tag(file,pos,mytag,line_cursor);

    strcat(expression,"func("); // abre expressão da função

    while(!atb_cmp(mytag,0,"/function")){

        if(atb_cmp(mytag,0,"boundVars")){ // se for tag <boundVars>
            
            log_to_console("tag","<boundVars>",0,line_cursor);

            read_tag(file,pos,mytag,line_cursor);

            while(!atb_cmp(mytag,0,"/boundVars")){ // espera fechamento </boundVars> pois podem haver vários <id>
                
                if(type_parser(file,pos,mytag,expression,line_cursor)!=true){ // identitifica tipo genérico
                    return error;
                }
                
                strcat(expression,","); // separador

                read_tag(file,pos,mytag,line_cursor);
            }

            strdel_last(expression,1); // ultima virgula posta
            log_to_console("/tag","</boundVars>",0,line_cursor);

        }else{

            if(type_parser(file,pos,mytag,expression,line_cursor)!=true){ // identitifica tipo genérico
                return error;
            }

        }

        strcat(expression,","); // virgula entre tags dentro de <function>, independente quem é a variavel da função e variaveis indepedentes

        read_tag(file,pos,mytag,line_cursor);
    }
    
    strdel_last(expression,1); // deleta ultima virgula posta
    strcat(expression,")"); // fecha expressão de <function>

    log_to_console("/tag","</function>",0,line_cursor);
    return true;
}

//parser de symEval
int symEval_parser(char *file, int *pos, char *expression, int *line_cursor){
    allocate(mytag,tag);
    read_tag(file,pos,mytag,line_cursor);

    strcat(expression,"symb("); // abre expressão symb()

    while (!atb_cmp(mytag,0,"/symEval"))
    {
        
        if(atb_cmp(mytag,0,"command")){
            
            log_to_console("tag","<command>",0,line_cursor);
            read_tag(file,pos,mytag,line_cursor);

            while(!atb_cmp(mytag,0,"/command")){
                
                if(atb_cmp(mytag,0,"placeholder")){
                    strcat(expression," "); // coloca espaço caso não houver texto da operação de avaliação simbólica
                }else{
                    if(type_parser(file,pos,mytag,expression,line_cursor)!=true){
                        return error;
                    }
                }

                read_tag(file,pos,mytag,line_cursor);
            }

            log_to_console("/tag","</command>",0,line_cursor);

        }else if(atb_cmp(mytag,0,"symResult")){

            log_to_console("tag","<symResult>",0,line_cursor);
            read_tag(file,pos,mytag,line_cursor);

            while (!atb_cmp(mytag,0,"/symResult")){
                
                if(type_parser(file,pos,mytag,expression,line_cursor)!=true){ // identitifica tipo genérico
                    return error;
                }

                read_tag(file,pos,mytag,line_cursor);
            }
            
        }else{

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

//parser do tipo lambda
int lambda_parser(char *file, int *pos, char *expression, int *line_cursor){
    allocate(mytag,tag);
    read_tag(file,pos,mytag,line_cursor);

    while(!atb_cmp(mytag,0,"/lambda")){

        if(atb_cmp(mytag,0,"boundVars")){

            read_tag(file,pos,mytag,line_cursor);

            while(!atb_cmp(mytag,0,"/boundVars")){

                if(type_parser(file,pos,mytag,expression,line_cursor)!=true){
                    return 0;
                }

                strcat(expression,","); // separador entre variaveis

                read_tag(file,pos,mytag,line_cursor);
            }
        }else{
            if(type_parser(file,pos,mytag,expression,line_cursor)!=true){
                return error;
            }
        }

        read_tag(file,pos,mytag,line_cursor);
    }

    return true;
}

//verifica o tipo, apply, id, real, etc. e aplica
int type_parser(char *file, int *pos, tag *ref_tag, char *expression, int *line_cursor){

    if(atb_cmp(ref_tag,0,"id")){ 
        log_to_console("tag","<id>",0,line_cursor);
        if(id_parser(file,pos,expression,line_cursor)!=true){
            log_to_console("error","Erro ao processar tag <id>",0,line_cursor);
            return error;
        }
    }else if(atb_cmp(ref_tag,0,"real")){ 
        log_to_console("tag","<real>",0,line_cursor);
        if(real_parser(file,pos,expression,line_cursor)!=true){
            log_to_console("error","Erro ao processar tag <real>",0,line_cursor);
            return error;
        }
    }else if(atb_cmp(ref_tag,0,"parens")){ 
        log_to_console("tag","<parens>",0,line_cursor);
        if(parens_parser(file,pos,expression,line_cursor)!=true){
            log_to_console("error","Erro ao processar tag <parens>",0,line_cursor);
            return error;
        }
    }else if(atb_cmp(ref_tag,0,"neg")){ 
        log_to_console("tag","<neg>",0,line_cursor);
        if(neg_parser(file,pos,expression,line_cursor)!=true){
            log_to_console("error","Erro ao processar tag <neg>",0,line_cursor);
            return error;
        }
    }else if(atb_cmp(ref_tag,0,"apply")){ 
        log_to_console("tag","<apply>",0,line_cursor);
        if(apply_parser(file,pos,expression,line_cursor)!=true){
            log_to_console("error","Erro ao processar tag <apply>",0,line_cursor);
            return error;
        }
    }else if(atb_cmp(ref_tag,0,"range")){ 
        log_to_console("tag","<range>",0,line_cursor);
        if(range_parser(file,pos,expression,line_cursor)!=true){
            log_to_console("error","Erro ao processar tag <range>",0,line_cursor);
            return error;
        }
    }else if(atb_cmp(ref_tag,0,"function")){ 
        log_to_console("tag","<function>",0,line_cursor);
        if(function_parser(file,pos,expression,line_cursor)!=true){
            log_to_console("error","Erro ao processar tag <function>",0,line_cursor);
            return error;
        }
    }else if(atb_cmp(ref_tag,0,"symEval")){ 
        log_to_console("tag","<symEval>",0,line_cursor);
        if(symEval_parser(file,pos,expression,line_cursor)!=true){
            log_to_console("error","Erro ao processar tag <symEval>",0,line_cursor);
            return error;
        }
    }else if(atb_cmp(ref_tag,0,"lambda")){
        log_to_console("tag","<lambda>",0,line_cursor);
        if(lambda_parser(file,pos,expression,line_cursor)!=true){
            log_to_console("error","Erro ao processar tag <lambda>",0,line_cursor);
            return error;
        }
    }else if(atb_cmp(ref_tag,0,"sequence") | atb_cmp(ref_tag,0,"upperBound") | atb_cmp(ref_tag,0,"lowerBound") | atb_cmp(ref_tag,0,"degree")){
        if(parser_generic(file,pos,atb_get(ref_tag,0),expression,line_cursor)!=true){
            return error;
        }
    }else if(atb_cmp(ref_tag,0,"placeholder")){
        log_to_console("tag","<placeholder>",0,line_cursor);
        strcat(expression," ");
    }else{
        log_to_console("error","Tag invalida encontrada",0,line_cursor);
        log_to_console("tag?",atb_get(ref_tag,0),0,line_cursor);
        return error;
    }

    return true;
}

//parser de matemática
int math_parser(char *file, int *pos, char *expression, int result_ref, resultsList **resultlist_ref, int *line_cursor){
    allocate(mytag,tag);
    read_tag(file, pos, mytag, line_cursor);
    float evaluation_result;
    char evaluation_result_string[50];

    while(!atb_cmp(mytag,0,"/math")){ // espera fechamento

        if(atb_cmp(mytag,0,"define")){ // tipo definição

            log_to_console("tag","<define>",0,line_cursor);
            
            strcpy(expression,"def("); // adiciona abertura de definição à expressão, strcpy pois é primeiro argumento da expressão
            
            read_tag(file, pos, mytag, line_cursor);
            
            while(!atb_cmp(mytag,0,"/define")){ // espera fechamento de <define>
                
                if(type_parser(file,pos,mytag,expression,line_cursor)!=true){ // identitifica tipo genérico
                    return error;
                }
                
                strcat(expression,","); // adiciona virgula após tipo
                 
                read_tag(file, pos, mytag, line_cursor);
            }

            strdel_last(expression,1); // deleta ultima virgula 
            strcat(expression,")"); // fecha definição

        }else if(atb_cmp(mytag,0,"eval")){ // tipo eval 
            
            log_to_console("tag","<eval>",0,line_cursor);

            strcpy(expression,"eval("); // abre expressão eval

            read_tag(file,pos,mytag,line_cursor);

            while(!atb_cmp(mytag,0,"/eval")){ // espera fechamento

                if(atb_cmp(mytag,0,"unitOverride")){

                    log_to_console("tag","<unitOverride>",0,line_cursor);
                    read_tag(file,pos,mytag,line_cursor);

                    while(!atb_cmp(mytag,0,"/unitOverride")){
                        
                        if(atb_cmp(mytag,0,"placeholder")){ // resolve a tag para o valor correto
                            evaluation_result = get_result(resultlist_ref,result_ref); // pega valor
                            sprintf(evaluation_result_string,"%.15f",evaluation_result); // faz para string
                            strcat(expression,evaluation_result_string); // concatena valor
                        }

                        read_tag(file,pos,mytag,line_cursor);
                    }

                    log_to_console("tag","</unitOverride>",0,line_cursor);

                }else{
                    if(type_parser(file,pos,mytag,expression,line_cursor)!=true){ // identitifica tipo genérico
                        return error;
                    }
                }

                strcat(expression,","); // separador

                read_tag(file,pos,mytag,line_cursor);
            }

            strdel_last(expression,1); // deletea ultima virgula posta
            strcat(expression,")"); // fecha expressão eval

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

//text parser
int text_parser(char *file, int *pos, char *expression, int result_ref, text_field **text_list, resultsList **resultslist_ref, int *line_cursor){
    allocate(mytag,tag);
    regions *myregions = NULL;

    read_tag(file,pos,mytag,line_cursor);
    while(!atb_cmp(mytag,0,"/text")){

        if(atb_cmp(mytag,0,"regions")){
            if(parse_regions(file,pos,&myregions,mytag,text_list, resultslist_ref,line_cursor)!=true){
                log_to_console("error","Erro ao processar tag <regions>",0,line_cursor);
                return error;
            }
        }

        read_tag(file,pos,mytag,line_cursor);
    }

    char *raw_text = (char*)malloc(sizeof(char)*REGION_EXPRESSION_LEN_DEFAULT); // raw text from xaml parser
    char *buffer = (char*)malloc(sizeof(char)*REGION_EXPRESSION_LEN_DEFAULT); 
    strncpy(raw_text,get_text_field(text_list, result_ref),REGION_EXPRESSION_LEN_DEFAULT);
    int placeholder_offset = strlen(TEXT_PLACE_HOLDER)*sizeof(char); // lenght of placeholder
    char *token_start;
    char *token_end;

    regions *cursor = myregions;
    while(cursor!=NULL){  // substitute placeholders
        token_start = strstr(raw_text,TEXT_PLACE_HOLDER);
        token_end = token_start + placeholder_offset;
        strncpy(buffer,token_end,strlen(token_end)+1); // save string after the token, +1 to acomodate the '\0' at the end 
        strncpy(token_start,cursor->region_data->expression,strlen(cursor->region_data->expression)+1); // substitute de place holder
        strcat(token_start,buffer); // bring back the rest 

        cursor=cursor->next;
    }

    strncpy(expression,raw_text,strlen(raw_text)+1); // pass to the main expression
    free(buffer);
    free(raw_text);

    return true;
}

//parser de region
int parse_region(char *file, int *pos, region **region_obj, tag *header, text_field **text_list, resultsList **resultlist_ref, int *line_cursor){
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

    while(!atb_cmp(mytag, 0, "/region")){
        if(strcmp(mytag->tag_arg[0],"math")==true){ // math tag
            log_to_console("tag","<math>",0,line_cursor);

            strcpy(myregion->type,"math"); // save type
            myregion->resultRef = atoi(atb_read_value(atb_get(mytag,1))); // picks the "resultRef" id from <math> for evaluation crossreference 
            
            if(math_parser(file, pos, (char*)&myregion->expression, myregion->resultRef, resultlist_ref, line_cursor)!=true){
                log_to_console("error","Erro ao processar tag <math>",myregion->region_id,line_cursor);
                return error;
            }
        }else if(strcmp(mytag->tag_arg[0],"text")==true){ // text tag
            log_to_console("tag","<text>",0,line_cursor);

            strcpy(myregion->type,"text"); // save type
            myregion->resultRef = atoi(atb_read_value(atb_get(header,1))); // pick the <region> caller "region-id", necessary to crossreference with xaml text
            
            if(text_parser(file, pos, (char*)&myregion->expression, myregion->resultRef, text_list, resultlist_ref, line_cursor)!=true){
                log_to_console("error","Erro ao processar tag <text>",myregion->region_id,line_cursor);
                return error;
            }
        }else if(strcmp(mytag->tag_arg[0],"plot")==true){ // tipo plot
            //ESCREVER
            log_to_console("Missing","Escrever plot parser",0,line_cursor);
            read_tag(file, pos, mytag, line_cursor);
            while(!atb_cmp(mytag,0,"/plot")){
                read_tag(file, pos, mytag, line_cursor);
            }
        }else{
            log_to_console("error","Nenhuma tag válida dentro da região encontrada!",0,line_cursor);
            return error;
        }
         
        read_tag(file, pos, mytag, line_cursor);
    }

    *region_obj=myregion; // devolve ponteiro para objeto repassado
    log_to_console("/tag","</region>",0,line_cursor);
    return true;
}

//parser de regions
int parse_regions(char *file, int *pos, regions **regions_obj, tag *header, text_field **text_list, resultsList **resultlist_ref, int *line_cursor){
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

        if(parse_region(file, pos, &myregion, mytag, text_list, resultlist_ref, line_cursor)!=true){
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
int parse_worksheet(char *file, int *pos, worksheet **worksheet_obj, tag *header, text_field **text_list, resultsList **resultlist_ref, int *line_cursor){
    int tag_counter; //contador de tags
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
        if(parse_regions(file, pos, &myregions, mytag, text_list, resultlist_ref, line_cursor)!=true){ // realizar parse da regions e verificar se deu boa ou não
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
int parse_worksheet_xml(char *file, int *pos, worksheets **worksheets_obj, text_field **text_list, resultsList **resultlist_ref, int *line_cursor){
    int tag_counter=0; // contador de tags
    allocate(mytag,tag);
    worksheet *myworksheet; // cria worksheet para ser armazenada posteriormente
    worksheets *worksheets_tmp = NULL; // objeto a ser montado e devolvido
    read_tag(file, pos, mytag, line_cursor); // lê primeira tag

    while(!atb_cmp(mytag, 0, "the_end")){ // verifica fim do arquivo
        //verifica se é a tag esperada
        if(atb_cmp(mytag,0,"worksheet")){ // verifica se é tipo worksheet
            
            log_to_console("tag","<worksheet>",tag_counter,line_cursor);
            // processa a tag, myworksheet retorna com tag e regions embutida
            if(parse_worksheet(file, pos, &myworksheet, mytag, text_list, resultlist_ref, line_cursor)!=true){ // chama parse para tag worksheet e verifica se foi feita com sucesso
                return error;
            }

            // adiciona worksheet para lista worksheets
            add_worksheet(&worksheets_tmp, myworksheet);

            // lê para próxima iteração
            tag_counter++;
        }

        read_tag(file, pos, mytag, line_cursor); 
    }
    
    *worksheets_obj=worksheets_tmp; // recebe ponteiro de novo
    return true;
}