#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <lualib.h>
#include <lauxlib.h>

#include "include/zip.h"
#include "modules/data_structures/data_structures.h"
#include "modules/worksheet_parser/worksheet_parser.h"
#include "modules/results_parser/results_parser.h"
#include "modules/file_handle/file_handle.h"
#include "modules/xaml_parser/xaml_parser.h"

#define allocate(name,type) type *name=(type*)malloc(sizeof(type))
#define true 0
#define error 1

#define FILES_MAX 20
#define OPT_ARG_MAX_LEN 250

void check_luaVmachine(lua_State *L, int ret){
    if(ret!=LUA_OK){
        printf("[LUA] ERROR: %s\n",lua_tostring(L,-1));
        lua_close(L);
        exit(1);
    }
}

int main(int argc, char **argv){
    
    /* Arguments ---------------------------------------------------------------------------------------------- */  
    char files[FILES_MAX][OPT_ARG_MAX_LEN];
    int f_counter=0;
    char script[OPT_ARG_MAX_LEN];
    int s_counter=0;
    char output_file[OPT_ARG_MAX_LEN];
    int o_counter=0;
    int help=0;
    int opt;
    int remove_files=1;
    console_log_disable();

    while( (opt = getopt(argc, argv, ":f:s:o:hcv")) != -1 ){
        switch (opt)
        {
        case 'f':
            files[f_counter][0]='\0';
            strncpy(files[f_counter],optarg,OPT_ARG_MAX_LEN);
            f_counter++;
            break;
        case 's':
            script[0]='\0';
            strncpy(script,optarg,OPT_ARG_MAX_LEN);
            s_counter++;
            break;
        case 'o':
            output_file[0]='\0';
            strncpy(output_file,optarg,OPT_ARG_MAX_LEN);
            o_counter++;
            break;
        case 'h':
            help=1; // show help
            break;
        case 'c':
            remove_files=0; // donot remove files
            break;
        case 'v':
            console_log_enable();
            break;
        case '?':
            if(optopt == 'f' | optopt == 's' | optopt == 'o'){
                printf("A opcao %c necessita de um argumento\n",optopt);
                exit(1);
            }else{
                printf("Opcao %c invalida\n",optopt);
                exit(1);
            }
            break;
        default:
            printf("Erro getopt\n");
            exit(1);
            break;
        }
    }

    if (help==1) // show help
    {
        printf("\nOpcoes de utilizacao do programa, arquivos devem ser especificados sem a extencao de arquivo. Ex: \".exe\" : \n");
        printf("\t-f: nome de um arquivo a ser processado, use um para cada arquivo.\n");
        printf("\t-s: nome de um arquivo script lua para ser usado na geracao do latex.\n");
        printf("\t-o: nome do arquiuvo de saida unico, caso nao especificado, cada arquivo de entrada vira um arquivo de saida separado.\n");
        printf("\n");
        exit(1);
    }
    else if (f_counter<1) // at least one file
    {
        printf("Especifique ao menos um arquivo com -f\n");
        exit(1);
    }
    else if(s_counter!=1) // one defined script
    {
        printf("Especifique um e apenas um arquivo de script lua com -s\n");
        exit(1);
    }
    else if(o_counter>1) // one defined script
    {
        printf("Especifique um ou nenhum arquivo de sáida com -o\n");
        exit(1);
    }

    for(int i=0; i<f_counter; i++){ // file extensions
        strcat(files[i],".mcdx");
    }
    strcat(script,".lua");
    strcat(output_file,".tex");


    /* Process each file --------------------------------------------------------------------------------------------------*/


    // Auxiliar string making buffer
    char *buffer = (char*)malloc(sizeof(char)*REGION_EXPRESSION_LEN_DEFAULT);
    buffer[0]='\0';

    // Directory explorer variables
    char *unziped_path;
    struct dirent *entry;

    // File explorer variables
    int pos;
    int cursor;
    char *input;
    
    // Parser lists
    text_field *text_list;
    resultsList *result_xml;
    worksheets *worksheets_xml;

    // Lua process variables
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    int output_lock=0;
    region myregion;
    int x=0,y=0,z=0;

    // unpack and process all files
    for(int i=0; i<f_counter; i++) 
    { 

        /* Unpack mathcad file ----------------------------------------*/

        unziped_path = filename_to_folder(files[i]); // path to unpacked folder os specified file
        rm_dir(unziped_path); // del recursivily last unpacked folder if it exist
        zip_extract(files[i], unziped_path, on_extract_entry, NULL); // unpack
        
        //chdir(unziped_path); // change binary wd to the unpacked file
        buffer[0]='\0';
        strncpy(buffer,unziped_path,OPT_ARG_MAX_LEN);
        strcat(buffer,"/mathcad/xaml");

        /* Parsers ----------------------------------------------------*/
        
        text_list = extract_docs(buffer); // text parser

        if(text_list==NULL){
            log_to_console("error","Erro ao processar documento .XamlPackage",0,&cursor);
            return error;
        }

        if (is_console_log_enable()==1)
            print_text_field(&text_list);

        input = NULL;
        pos=0;
        cursor=1;

        buffer[0]='\0';
        strncpy(buffer,unziped_path,OPT_ARG_MAX_LEN);
        strcat(buffer,"/mathcad/result.xml");

        input = read_asci(buffer);
        if(input==NULL){
            return 0;
        }
        
        result_xml = parser_results_xml(input,&pos,&cursor); // results parser

        if(result_xml==NULL){
            log_to_console("error","Erro ao processar arquivo result.xml",0,&cursor);
            return 0;
        }
        log_to_console("done","Arquivo result.xml lido com sucesso",0,&cursor);
        free(input);

        input = NULL;
        pos=0;
        cursor=1;

        buffer[0]='\0';
        strncpy(buffer,unziped_path,OPT_ARG_MAX_LEN);
        strcat(buffer,"/mathcad/worksheet.xml");

        input = read_asci(buffer);
        if(input==NULL){
            return 0;
        }

        worksheets_xml = parse_worksheet_xml(input, &pos, &text_list, &result_xml, &cursor); // worksheet parser

        if(worksheets_xml==NULL){
            log_to_console("error","Erro ao processar arquivo worksheet.xml",0,&cursor);
            return 0;
        }
        log_to_console("done","Arquivo worskheet.xml lido com sucesso",0,&cursor);
        free(input);


        if (remove_files==1) // if the flag is set, delete unpacked file
            rm_dir(unziped_path);

        /* LUA CODE --------------------------------------------------- */

        if (o_counter==0) // each file is separate
        {
            buffer[0]='\0';
            snprintf(buffer,OPT_ARG_MAX_LEN,"fp = io.open(\"%s.tex\",\"w+\")",filename_to_folder(files[i])); // take the file minus the extension
        }
        else // one unque file
        {
            if (output_lock == 0)
            {
                buffer[0]='\0';
                snprintf(buffer,OPT_ARG_MAX_LEN,"fp = io.open(\"%s\",\"w+\")",output_file); // new file just one time
                output_lock=1;
            }
            else
            {
                buffer[0]='\0';
                snprintf(buffer,OPT_ARG_MAX_LEN,"fp = io.open(\"%s\",\"a+\")",output_file); // opens to append
            }
        }
        
        check_luaVmachine(L, luaL_dofile(L,script)); // open script
        check_luaVmachine(L, luaL_dostring(L,buffer)); // open file
        
        myregion = get_region(worksheets_xml,x,y,z);
        while (myregion.region_id!=-1)
        {
            while (myregion.region_id!=-1)
            {
                while(myregion.region_id!=-1) // myregion has -1 on regio_id if the region read doesnt exist, see get_region() on data_structure.c
                {
                    if (is_console_log_enable()==1)
                        printf("Expressao [%d]: %s - Tipo: %s\n",z,myregion.expression,myregion.type); // for show
                    
                    snprintf(buffer,REGION_EXPRESSION_LEN_DEFAULT,"add_field(%s,\"%s\",fp)",myregion.expression,myregion.type); // lua command
                    check_luaVmachine(L,luaL_dostring(L, buffer)); // lua exec command
                    z++; // next region
                    myregion = get_region(worksheets_xml,x,y,z);
                }
                z=0;
                y++;
                myregion = get_region(worksheets_xml,x,y,z);
            }
                y=0;
                x++;
                myregion = get_region(worksheets_xml,x,y,z);
        }

        x=0;
        y=0;
        z=0;
        check_luaVmachine(L, luaL_dostring(L, "fp:close()"));



    } // for 

    lua_close(L); // close lua vm
    return 0;
} // main