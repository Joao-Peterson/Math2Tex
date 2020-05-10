#include "logger.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//função de log
void log_to_console(const char *type, const char *msg, int id, int *line){
    if(strcmp(type,"done")==0){
        printf("[Complete] - %s - line %d - %s\n",type,*line,msg);
    }else if(strcmp(type,"tag")==0){
        printf("[Tag] - %s [%d] - line %d \n",msg,id,*line);
    }else if(strcmp(type,"/tag")==0){
        printf("[Tag] - %s - line %d \n",msg,*line);
    }else if(strcmp(type,"parser")==0){
        printf("[Parser] - %s [%d] - line %d \n",msg,id,*line);
    }else if(strcmp(type,"tag?")==0){
        printf("[Unknown] - <%s> - line %d \n",msg,*line);
    }else{
        printf("[ERROR] - %s - line %d - %s\n",type,*line,msg);
    }
}