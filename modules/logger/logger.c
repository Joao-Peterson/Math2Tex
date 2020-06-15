#include "logger.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int console_flag_out=0;

// enable console log
void console_log_enable(){
    console_flag_out=1;
}

// disable console log
void console_log_disable(){
    console_flag_out=0;
}

//self explanatory
int is_console_log_enable(){
    if (console_flag_out==1)
        return 1;
    else    
        return 0;
}

//função de log
void log_to_console(const char *type, const char *msg, int id, int *line){

    if (console_flag_out==1) // enables print
    {
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
    else if (console_flag_out!=0 | console_flag_out!= 1) // in case of unitialized value
    {
        console_log_disable();
    }

}