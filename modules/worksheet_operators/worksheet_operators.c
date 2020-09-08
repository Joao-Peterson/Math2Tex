#include "worksheet_operators.h"
#include "../worksheet_parser/worksheet_parser.h"
#include "../xaml_parser/xaml_parser.h"
#include "../../include/types_config.h"

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

//matrix parser
int matrix_parser(char *file, int *pos, tag *header, char *expression, int *line_cursor){
    allocate(mytag,tag);
    char buffer[100];
    read_tag(file,pos,mytag,line_cursor);

    // grab the rows and colunns
    int rows = atoi(atb_read_value(atb_get(header,1)));
    int cols = atoi(atb_read_value(atb_get(header,2)));

    strcat(expression,"mat(");
    snprintf(buffer,100,"%d,%d,",rows,cols); 
    strcat(expression,buffer);

    while (!atb_cmp(mytag,0,"/matrix"))
    {
        if (type_parser(file,pos,mytag,expression,line_cursor)!=true)
        {
            return error;
        }
        
        strcat(expression,","); // separador

        read_tag(file,pos,mytag,line_cursor);
    }
    
    strdel_last(expression,1); // delete last comma
    strcat(expression,")");

    return true;
}

//imaginary number parser
int imag_parser(char *file, int *pos, tag *header, char *expression, int *line_cursor){
    allocate(mytag,tag);
    read_tag(file,pos,mytag,line_cursor);

    strcat(expression,"imag(");
    strcat(expression,"\"");
    strcat(expression,atb_read_value(atb_get(header,1))); // pick imaginary symbol
    strcat(expression,"\",");

    while(!atb_cmp(mytag,0,"/imag"))
    {
        if (mytag->tag_type==TAG_TYPE_VALUE)
        {
            strcat(expression,atb_get(mytag,0)); // pick the value
        }
        else{
            if(type_parser(file,pos,mytag,expression,line_cursor)!=true)
                return error;
        } 

        strcat(expression,","); // separator

        read_tag(file,pos,mytag,line_cursor);
    }

    strdel_last(expression,1); // delete last comma
    strcat(expression,")"); // closure

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

        strcat(expression,"\"");
        strcat(expression, atb_get(mytag,0)); // concatena na expressão o valor real lido
        strcat(expression,"\"");
                         
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

    if (atb_cmp(mytag, 0, "plus"))
    { // verifica qual operador matemático e adiciona a expressão
        strcat(expression, "add(");
        log_to_console("tag", "<plus>", 0, line_cursor);
        read_tag(file,pos,mytag,line_cursor); // lê nova tag após tag de operação matemática
    }
    else if (atb_cmp(mytag, 0, "minus"))
    {
        strcat(expression, "sub(");
        log_to_console("tag", "<minus>", 0, line_cursor);
        read_tag(file,pos,mytag,line_cursor); // lê nova tag após tag de operação matemática
    }
    else if (atb_cmp(mytag, 0, "mult"))
    {
        strcat(expression, "mul(");
        log_to_console("tag", "<mult>", 0, line_cursor);
        read_tag(file,pos,mytag,line_cursor); // lê nova tag após tag de operação matemática
    }
    else if (atb_cmp(mytag, 0, "div"))
    {
        strcat(expression, "div(");
        log_to_console("tag", "<div>", 0, line_cursor);
        read_tag(file,pos,mytag,line_cursor); // lê nova tag após tag de operação matemática
    }
    else if (atb_cmp(mytag, 0, "pow"))
    {
        strcat(expression, "pow(");
        log_to_console("tag", "<pow>", 0, line_cursor);
        read_tag(file,pos,mytag,line_cursor); // lê nova tag após tag de operação matemática
    }
    else if (atb_cmp(mytag, 0, "greaterThan"))
    {
        strcat(expression, "greaterThan(");
        log_to_console("tag", "<greaterThan>", 0, line_cursor);
        read_tag(file,pos,mytag,line_cursor); // lê nova tag após tag de operação matemática
    }
    else if (atb_cmp(mytag, 0, "greaterOrEqual"))
    {
        strcat(expression, "greaterOrEqual(");
        log_to_console("tag", "<greaterOrEqual>", 0, line_cursor);
        read_tag(file,pos,mytag,line_cursor); // lê nova tag após tag de operação matemática
    }
    else if (atb_cmp(mytag, 0, "lessThan"))
    {
        strcat(expression, "lessThan(");
        log_to_console("tag", "<lessThan>", 0, line_cursor);
        read_tag(file,pos,mytag,line_cursor); // lê nova tag após tag de operação matemática
    }
    else if (atb_cmp(mytag, 0, "lessOrEqual"))
    {
        strcat(expression, "lessOrEqual(");
        log_to_console("tag", "<lessOrEqual>", 0, line_cursor);
        read_tag(file,pos,mytag,line_cursor); // lê nova tag após tag de operação matemática
    }
    else if (atb_cmp(mytag, 0, "equal"))
    {
        strcat(expression, "equal(");
        log_to_console("tag", "<equal>", 0, line_cursor);
        read_tag(file,pos,mytag,line_cursor); // lê nova tag após tag de operação matemática
    }
    else if (atb_cmp(mytag, 0, "notEqual"))
    {
        strcat(expression, "notEqual(");
        log_to_console("tag", "<notEqual>", 0, line_cursor);
        read_tag(file,pos,mytag,line_cursor); // lê nova tag após tag de operação matemática
    }
    else if (atb_cmp(mytag, 0, "xor"))
    {
        strcat(expression, "f_xor(");
        log_to_console("tag", "<xor>", 0, line_cursor);
        read_tag(file,pos,mytag,line_cursor); // lê nova tag após tag de operação matemática
    }
    else if (atb_cmp(mytag, 0, "or"))
    {
        strcat(expression, "f_or(");
        log_to_console("tag", "<or>", 0, line_cursor);
        read_tag(file,pos,mytag,line_cursor); // lê nova tag após tag de operação matemática
    }
    else if (atb_cmp(mytag, 0, "and"))
    {
        strcat(expression, "f_and(");
        log_to_console("tag", "<and>", 0, line_cursor);
        read_tag(file,pos,mytag,line_cursor); // lê nova tag após tag de operação matemática
    }
    else if (atb_cmp(mytag, 0, "not"))
    {
        strcat(expression, "f_not(");
        log_to_console("tag", "<not>", 0, line_cursor);
        read_tag(file,pos,mytag,line_cursor); // lê nova tag após tag de operação matemática
    }
    else if (atb_cmp(mytag, 0, "parens"))
    {
        strcat(expression, "parens(");
        log_to_console("tag", "<parens>", 0, line_cursor);
        read_tag(file,pos,mytag,line_cursor); // lê nova tag após tag de operação matemática
    }
    else if (atb_cmp(mytag, 0, "neg"))
    {
        strcat(expression, "neg(");
        log_to_console("tag", "<neg>", 0, line_cursor);
        read_tag(file,pos,mytag,line_cursor); // lê nova tag após tag de operação matemática
    }
    else if (atb_cmp(mytag,0,"limit"))
    {
        strcat(expression,"lim(");
        if (mytag->argq>1){ // if there is a limit direction, that is described in the first argument of <limit>
            strcat(expression,"\"");
            strcat(expression,atb_read_value(atb_get(mytag,1))); // cat to the expression the direction
            strcat(expression,"\",");
        }
        log_to_console("tag","<limit>",0,line_cursor);
        read_tag(file,pos,mytag,line_cursor); // lê nova tag após tag de operação matemática
    }
    else if (atb_cmp(mytag,0,"derivative"))
    {
        strcat(expression,"deri(");
        log_to_console("tag","<derivative>",0,line_cursor);
        read_tag(file,pos,mytag,line_cursor); // lê nova tag após tag de operação matemática
    }
    else if (atb_cmp(mytag,0,"integral"))
    {
        strcat(expression,"int(");
        log_to_console("tag","<integral>",0,line_cursor);
        read_tag(file,pos,mytag,line_cursor); // lê nova tag após tag de operação matemática
    }
    else if (atb_cmp(mytag,0,"summation"))
    {
        strcat(expression,"summation(");
        log_to_console("tag","<summation>",0,line_cursor);
        read_tag(file,pos,mytag,line_cursor); // lê nova tag após tag de operação matemática
    }
    else if (atb_cmp(mytag,0,"convolution"))
    {
        strcat(expression,"convolution(");
        log_to_console("tag","<convolution>",0,line_cursor);
        read_tag(file,pos,mytag,line_cursor); // lê nova tag após tag de operação matemática
    }
    else if (atb_cmp(mytag,0,"scale"))
    {
        strcat(expression,"sca(");            
        log_to_console("tag","<scale>",0,line_cursor);
        read_tag(file,pos,mytag,line_cursor); // lê nova tag após tag de operação matemática
    }
    else if (atb_cmp(mytag,0,"nthRoot"))
    {
        strcat(expression,"nroot(");            
        log_to_console("tag","<nthroot>",0,line_cursor);
        read_tag(file,pos,mytag,line_cursor); // lê nova tag após tag de operação matemática
    }
    else if (atb_cmp(mytag,0,"degree_op"))
    {
        strcat(expression,"degree(");            
        log_to_console("tag","<degree_op>",0,line_cursor);
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

    while(!atb_cmp(mytag,0,"/id"))  // espera tag de fechamento
    {
        if(mytag->tag_type==TAG_TYPE_VALUE) // se inicia com variavel(value), guardar ele
        { 
            strcat(expression, "leg(\"");
            strcat(expression,atb_get(mytag,0)); // copia variavel
            strcat(expression, "\",\" \",\" \")");
        }
        else if(atb_cmp(mytag, 0, "Span")) // senão, então esperamos uma tag <Span> que contem a variavel
        { 
            log_to_console("tag","<Span>",0,line_cursor);
              // necessário a cada leitura
            read_tag(file, pos, mytag, line_cursor); // lê de novo

            while(!atb_cmp(mytag, 0, "/Span")) // espera fechamento da tag
            {
                // Valor da variavel 
                if(mytag->tag_type==TAG_TYPE_VALUE) // espera uma variavel
                {
                    strcpy(var,atb_get(mytag,0)); // pega variavel
                }
                

                if(atb_cmp(mytag, 0, "Subscript")) // Subescrito
                {
                     
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
                
                
                if(atb_cmp(mytag, 0, "Upperscript")) // Superscrito
                {
                     
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
                
                             
                read_tag(file, pos, mytag, line_cursor);
            }

                strcat(expression, "leg(\""); // adiciona legenda a expressão
                strcat(expression, var); // adiciona variavel
                strcat(expression, "\",\""); // separação virgula

                if (subs[0]!='\0')
                    strcat(expression, subs); // adiciona variavel
                else
                    strcat(expression," "); // adiciona espaço vazio
                
                strcat(expression, "\",\""); // separação virgula
                
                if (upper[0]!='\0')
                    strcat(expression, upper); // adiciona variavel
                else
                    strcat(expression," "); // adiciona espaço vazio
                
                strcat(expression, "\")"); // fecha legenda

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
    }else if(atb_cmp(ref_tag,0,"imag")){ 
        log_to_console("tag","<imag>",0,line_cursor);
        if(imag_parser(file,pos,ref_tag,expression,line_cursor)!=true){
            log_to_console("error","Erro ao processar tag <imag>",0,line_cursor);
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
    }else if(atb_cmp(ref_tag,0,"matrix")){
        log_to_console("tag","<matrix>",0,line_cursor);
        if(matrix_parser(file,pos,ref_tag,expression,line_cursor)!=true){
            log_to_console("error","Erro ao processar tag <matrix>",0,line_cursor);
            return error;
        }
    }else if(atb_cmp(ref_tag,0,"sequence") | atb_cmp(ref_tag,0,"upperBound") | atb_cmp(ref_tag,0,"lowerBound") | atb_cmp(ref_tag,0,"degree")){
        if(parser_generic(file,pos,atb_get(ref_tag,0),expression,line_cursor)!=true){
            return error;
        }
    }else if(atb_cmp(ref_tag,0,"placeholder")){
        log_to_console("tag","<placeholder>",0,line_cursor);
        strcat(expression,EMPTY_ARG);
    }else{
        log_to_console("error","Tag invalida encontrada",0,line_cursor);
        log_to_console("tag?",atb_get(ref_tag,0),0,line_cursor);
        return error;
    }

    return true;
}
