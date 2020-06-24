#include "logger.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <errno.h>

/* Verbose mode members ------------------------------------------*/

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


/* Log file members  ------------------------------------------*/

FILE *log_file = NULL; // default log file

int log_file_flag = 0;

// enable a log file
void enable_log_file(){
    time_t raw_time = time(&raw_time);
    struct tm *local_time = localtime(&raw_time);

    char log_filename[FILENAME_MAX] = {0}; //log file name
    strftime(log_filename,FILENAME_MAX,"m2tex_%a_%d-%b-%Y-%Hh%M.log",local_time);

    log_file = fopen(log_filename,"w+");
    if(log_file==NULL){
        printf("Erro ao criar arquivo log, erro: %s\n",strerror(errno));
        exit(1);
    }

    log_file_flag=1;
}


/* Log function  ------------------------------------------*/

void log_to_console(const char *type, const char *msg, int id, int *line){
    if (line==NULL) // if line argument is null
    {
        int line_tmp = 0;
        line = &line_tmp;
    }

    if (strcmp(type, "done") == 0)
    {
        if (console_flag_out == 1) // enables print
            fprintf(stdout, "[Complete] - %s - line %d - %s\n", type, *line, msg);
        if (log_file_flag == 1 ) // enables lof file print
            fprintf(log_file, "[Complete] - %s - line %d - %s\n", type, *line, msg);
    }
    else if (strcmp(type, "tag") == 0)
    {
        if (console_flag_out == 1) // enables print
            fprintf(stdout, "[Tag] - %s [%d] - line %d \n", msg, id, *line);
        if (log_file_flag == 1 ) // enables lof file print
            fprintf(log_file, "[Tag] - %s [%d] - line %d \n", msg, id, *line);
    }
    else if (strcmp(type, "/tag") == 0)
    {
        if (console_flag_out == 1) // enables print
            fprintf(stdout, "[Tag] - %s - line %d \n", msg, *line);
        if (log_file_flag == 1 ) // enables lof file print
            fprintf(log_file, "[Tag] - %s - line %d \n", msg, *line);
    }
    else if (strcmp(type, "parser") == 0)
    {
        if (console_flag_out == 1) // enables print
            fprintf(stdout, "[Parser] - %s [%d] - line %d \n", msg, id, *line);
        if (log_file_flag == 1 ) // enables lof file print
            fprintf(log_file, "[Parser] - %s [%d] - line %d \n", msg, id, *line);
    }
    else if (strcmp(type, "tag?") == 0)
    {
        if (console_flag_out == 1) // enables print
            fprintf(stdout, "[Unknown] - <%s> - line %d \n", msg, *line);
        if (log_file_flag == 1 ) // enables lof file print
            fprintf(log_file, "[Unknown] - <%s> - line %d \n", msg, *line);
    }
    else if (strcmp(type, "File") == 0)
    {
        if (console_flag_out == 1) // enables print
            fprintf(stdout, "[File] - Arquivo lido com sucesso : %s\n", msg);
        if (log_file_flag == 1 ) // enables lof file print
            fprintf(log_file, "[File] - Arquivo lido com sucesso : %s\n", msg);
    }
    else if (strcmp(type, "lua_erro") == 0)
    {
        if (console_flag_out == 1) // enables print
            fprintf(stdout, "[LUA] - ERRO: %s\n", msg);
        if (log_file_flag == 1 ) // enables lof file print
            fprintf(log_file, "[LUA] - ERRO: %s\n", msg);
    }
    else if (strcmp(type, "lua") == 0)
    {
        if (console_flag_out == 1) // enables print
            fprintf(stdout, "[LUA] - %s\n", msg);
        if (log_file_flag == 1 ) // enables lof file print
            fprintf(log_file, "[LUA] - %s\n", msg);
    }
    else if (strcmp(type, "msg") == 0)
    {
        if (console_flag_out == 1) // enables print
            fprintf(stdout, "[MESSAGE] - %s\n", msg);
        if (log_file_flag == 1 ) // enables lof file print
            fprintf(log_file, "[MESSAGE] - %s\n", msg);
    }
    else
    {
        if (console_flag_out == 1) // enables print
            fprintf(stdout, "[ERROR] - %s - line %d - %s\n", type, *line, msg);
        if (log_file_flag == 1 ) // enables lof file print
            fprintf(log_file, "[ERROR] - %s - line %d - %s\n", type, *line, msg);
    }
}