#include "xml_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define allocate(name,type) type *name=(type*)malloc(sizeof(type))

#define true 0
#define error 1

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
int atb_cmp(tag *ref, int id, const char *str){
    
    if(strcmp(ref->tag_arg[id],str)==0){
        return 1;
    }else{
        return 0;
    }
}

//lê valor de um atributo em uma tag e retorna o valor bruto
char *atb_read_value(char *atb){
    int i=0;
    int word=0;
    char *buffer = (char *)malloc(sizeof(char)*ATRIBUTE_LEN_DEFAULT); // aloca espaço para string entre aspas do atributo
    while(atb[i]!='"'){ // acha primeria aspas
        i++;
    }
    i++; // coloca cursor sobre o numero
    while(atb[i]!='"'){ // itera sobre o numero até a outras aspas
        buffer[word]=atb[i]; 
        i++;
        word++;
    }
    buffer[word]='\0'; // caractere de terminação
    return buffer; // retorna ponteiro do buffer
}

//lê um atributo inteiro de uma tag
char *atb_get(tag *ref, int id){
    return ref->tag_arg[id];
}

//lê tag e armezna argumentos   
void read_tag(char *file, int *pos, tag *mytag, int *line_cursor){ // recebe ponteiro do file, ponteiro para estrutura de tag, ponteiro para posição no arquivo
    int word=0;
    //zerar tag repassada
    mytag->argq=0;
    mytag->tag_type=TAG_TYPE_TAG;
    char *buffer=NULL;

    //percorre até encontrar um caractere
    while((file[*pos]==9)|(file[*pos]=='\n')|(file[*pos]==' ')|(file[*pos]==-17)|(file[*pos]==-65)|(file[*pos]==-69)|(file[*pos]==32)){ // caracteres vazios
        if(file[*pos]=='\n'){
            (*line_cursor)++;
        }
        (*pos)++;
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