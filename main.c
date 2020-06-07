#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <zip.h>
#include <lualib.h>
#include <lauxlib.h>

#include "modules/data_structures/data_structures.h"
#include "modules/worksheet_parser/worksheet_parser.h"
#include "modules/results_parser/results_parser.h"
#include "modules/file_handle/file_handle.h"
#include "modules/xaml_parser/xaml_parser.h"

#define allocate(name,type) type *name=(type*)malloc(sizeof(type))
#define true 0
#define error 1

void check_luaVmachine(lua_State *L, int ret){
    if(ret!=LUA_OK){
        printf("[LUA] ERROR: %s\n",lua_tostring(L,-1));
        lua_close(L);
        exit(1);
    }
}

int main(int argc, char **argv){
    
    /* Arguments */  
    if(argc!=2){
        printf("Sintaxe incorreta!");
        return 0;
    }


    // /* Unpack mathcad file */
    char *unziped_path = filename_to_folder(argv[1]); // path to unpacked folder os specified file
    rm_dir(unziped_path); // del recursivily last unpacked folder if it exist
    zip_extract(argv[1], unziped_path, on_extract_entry, NULL); // unpack
    
    chdir(unziped_path); // change binary wd to the unpacked file
    
    DIR *handle = opendir("mathcad/xaml");
    if(handle==NULL){
        printf("Diretorio invalido!\n");
        return 0;
    }

    struct dirent *entry;

    /* Parsers */
    int pos;
    int cursor;
    char *input;
    region myregion;

    text_field *text_list = extract_docs(handle);
    if(text_list==NULL){
        log_to_console("error","Erro ao processar documento .XamlPackage",0,&cursor);
        return error;
    }
    closedir(handle);

    print_text_field(&text_list);



    input = NULL;
    pos=0;
    cursor=1;
    input = read_asci("mathcad/result.xml");
    if(input==NULL){
        return 0;
    }
    
    resultsList *result_xml = parser_results_xml(input,&pos,&cursor);
    if(result_xml==NULL){
        log_to_console("error","Erro ao processar arquivo result.xml",0,&cursor);
        return 0;
    }
    log_to_console("done","Arquivo result.xml lido com sucesso",0,&cursor);
    free(input);



    input = NULL;
    pos=0;
    cursor=1;
    input = read_asci("mathcad/worksheet.xml");
    if(input==NULL){
        return 0;
    }

    worksheets *worksheets_xml = parse_worksheet_xml(input, &pos, &text_list, &result_xml, &cursor);
    if(worksheets_xml==NULL){
        log_to_console("error","Erro ao processar arquivo worksheet.xml",0,&cursor);
        return 0;
    }
    log_to_console("done","Arquivo worskheet.xml lido com sucesso",0,&cursor);
    free(input);


    //**-----------------LUA CODE----------------**


    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    char *buffer = (char*)malloc(sizeof(char)*REGION_EXPRESSION_LEN_DEFAULT);
    buffer[0]='\0';

    check_luaVmachine(L, luaL_dofile(L, "../../scripts/type.lua"));
    check_luaVmachine(L, luaL_dostring(L, "fp = io.open(\"../../tex_files/output.tex\",\"w+\")"));
    
    for(int i=0;i<=24;i++){
        myregion = get_region(worksheets_xml,0,0,i);
        if (myregion.region_id == -1) // in case of non existing id
            continue;
        printf("Expressao [%d]: %s - Tipo: %s\n",i,myregion.expression,myregion.type);
        snprintf(buffer,REGION_EXPRESSION_LEN_DEFAULT,"add_field(%s,\"%s\",fp)",myregion.expression,myregion.type);
        check_luaVmachine(L,luaL_dostring(L, buffer));
    }

    //check_luaVmachine(L,luaL_dostring(L,"add_field(leg(\"B\",\" \",\" \"),\"math\",fp)"));

    check_luaVmachine(L, luaL_dostring(L, "fp:close()"));

    lua_close(L);

    return 0;
}