#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <lualib.h>
#include <lauxlib.h>

#include "include/zip.h"
#include "modules/string_aux/string_aux.h"
#include "modules/data_structures/data_structures.h"
#include "modules/worksheet_parser/worksheet_parser.h"
#include "modules/results_parser/results_parser.h"
#include "modules/file_handle/file_handle.h"
#include "modules/xaml_parser/xaml_parser.h"
#include "modules/rtf_parser/rtf_parser.h"

#define allocate(name,type) type *name=(type*)malloc(sizeof(type))
#define true 0
#define error 1

#define FILES_MAX 20
#define OPT_ARG_MAX_LEN 400

void check_luaVmachine(lua_State *L, int ret){
    if(ret!=LUA_OK){
        log_to_console("lua_erro",lua_tostring(L,-1),0,0);
        lua_close(L);
        exit(1);
    }
}

int main(int argc, char **argv){

    /* Arguments ---------------------------------------------------------------------------------------------- */  

    char files[FILES_MAX][OPT_ARG_MAX_LEN];
    char files_rtf[FILES_MAX][OPT_ARG_MAX_LEN];
    int f_counter=0;
    char script[OPT_ARG_MAX_LEN];
    int s_counter=0;
    char output_file[OPT_ARG_MAX_LEN];
    char output_images[OPT_ARG_MAX_LEN];
    int o_counter=0;
    int help=0;
    int opt;
    int remove_files=1;
    int rtf_flag=0;
    console_log_disable();

    while( (opt = getopt(argc, argv, ":f:is:o:hdcv")) != -1 ){
        switch (opt)
        {
        case 'f':
            files[f_counter][0]='\0';
            strncpy(files[f_counter],optarg,OPT_ARG_MAX_LEN);
            f_counter++;
            break;
        case 'i':
            rtf_flag=1; // program will read .rtf counterparts of input files
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
        case 'd':
            enable_log_file();
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
        printf("\nOpcoes de utilizacao do programa, arquivos devem ser especificados com a extencao de arquivo. Ex: \".exe\" : \n");
        printf("\t-f: nome de um arquivo a ser processado, use um para cada arquivo.\n");
        printf("\t-i: indica que se espera um arquivo de mesmo nome que o arquivo de netrada mas salvo como .rtf,\n\t o qual sera usado para extrair as imagens.\n");
        printf("\t-s: nome de um arquivo script lua para ser usado na geracao do latex.\n");
        printf("\t-o: nome do arquiuvo de saida unico, caso nao especificado, cada arquivo de entrada vira um arquivo de saida separado.\n");
        printf("\t-d: modo de debug, cria um arquivo log de saída que pode dar maiores informacoes ao utilizador e desenvolvedor.\n");
        printf("\t-v: o programa produzira um log de saida na linha de comando mostrando o processo.\n");
        printf("\t-c: quando o programa terminar os diretorios temporarios usados para processamento nao serao deletados.\n");
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

    for(int i=0; i<f_counter; i++){ // rtf files
        if (rtf_flag==1){
            strncpy(files_rtf[i],filename_to_folder(files[i]),OPT_ARG_MAX_LEN); // removes the extension and cat a .rtf at the end
            strcat(files_rtf[i],".rtf");
        }
    }

    /* Process each file ----------------------------------------------------------------------------------------------------------------------*/

    // Auxiliar string making buffer
    char *buffer = (char*)malloc(sizeof(char)*REGION_EXPRESSION_LEN_DEFAULT);
    char *lua_file_cmd = (char*)malloc(sizeof(char)*REGION_EXPRESSION_LEN_DEFAULT);
    char *lua_log_buffer = (char*)malloc(sizeof(char)*REGION_EXPRESSION_LEN_DEFAULT);
    lua_file_cmd[0]='\0';
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
    img_list *my_img_list;

    // Lua process variables
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    int output_lock=0;
    region myregion;
    int x=0,y=0,z=0;

    /* UNPACK AND PROCESS ALL FILES ---------------------------------------------------------------------------------------------------------------*/

    for(int i=0; i<f_counter; i++) 
    {   

        /* Output file ---------------------------------------------------------------------------------------------------------------- */

        if (o_counter==0) // each file is separate
        {
            lua_file_cmd[0]='\0';
            snprintf(output_file,OPT_ARG_MAX_LEN,"%s.tex",filename_to_folder(files[i])); // path to output ".tex" file
            snprintf(lua_file_cmd,OPT_ARG_MAX_LEN,"fp = io.open(\"%s\",\"w+\")",output_file); // command to create and edit the ".tex" file
        }
        else // one unique file
        {
            if (output_lock == 0)
            {
                lua_file_cmd[0]='\0';
                snprintf(lua_file_cmd,OPT_ARG_MAX_LEN,"fp = io.open(\"%s\",\"w+\")",output_file); // new file just one time
                output_lock=1;
            }
            else
            {
                lua_file_cmd[0]='\0';
                snprintf(lua_file_cmd,OPT_ARG_MAX_LEN,"fp = io.open(\"%s\",\"a+\")",output_file); // opens to append
            }
        }

        /* PARSERS --------------------------------------------------------------------------------------------------------------------*/
        
        /* RTF parser - Images ------------------------------------------------------------------------------------ */  
        
        if (rtf_flag==1)
        {
            if (o_counter==0) // if each .mcdx file has it's owm .tex, then it also have a unique folder in the same directory for the images
            {
                snprintf(output_images,OPT_ARG_MAX_LEN,"%s_images",filename_to_folder(output_file) ); // extract images to the same path as the .tex file with the suffix "_images"
            }
            else // if the output .tex is the same for all files, then each image folder has the name of the original file, but in the same directory as the .tex file
            {
                output_images[0]='\0';
                strcat(output_images,filename_parent_get(output_file)); // parent of the .tex to output
                strcat(output_images,filename_subdirectory_get_name(files[i])); // the filename of the .mcdx file
                snprintf(output_images,OPT_ARG_MAX_LEN,"%s_images",filename_to_folder(output_file)); // extract images to the same path as the .tex file, with the name of the .mcdx file, with the suffix "_images". filename_to_folder because before, the extension .mcdx was left by the strcat
            }
            
            my_img_list = extract_images(files_rtf[i],output_images); // extract to the respective folder and return list of paths 

            /* CONCEPT: 
            We need to extract the images to a folder that will be in the same directory as the output .tex file, but,
            since this directorie to the image fodler is handled inside this program, or, relative to the program path of execution, this path needs to be
            modified because the output.tex will have to look for images relative to itself. Therefore, the image path relative to the program needs to be 
            cut so that it will be relative to the future .tex output file, like this example:

            -In program path :      
                    
                    -./output_files/output.tex
                    -./output_files/mymathcad_file.mcdx
                    -./output_files/mymathcad_file.rtf
                    -./output_files/mymathcad_file_images/

            -In the output.tex context: 
                    
                    -./output.tex
                    -./mymathcad_file.mcdx
                    -./mymathcad_file.rtf
                    -./mymathcad_file_images/

            As you can see "/output_files/" needs to be removed from the image folder path if the .tex file desires to acess the images, wich we want.
            So we pick the parents directories from the ourput file and delete these parents from the image folder filepath, leaving just the relative path to images in relation
            to the .tex file.
            */

            img_print_list(&my_img_list);

            int j=0;

            char *tmp_path;
            while ( (tmp_path=img_get_path(&my_img_list,j)) !=NULL )
            {
                strcrop(tmp_path,filename_parent_get(output_file)); // crops off the parents directory of the .tex file off the *tmp_path, wich points to the path of the image in the list
                if (tmp_path[0]=='/') // crops a residual "/" at the beginning of the path, just to make the filepath nicier
                    tmp_path++;
                    
                j++;
            }
        }

        /* Unpack mathcad file -------------------------------------------------------------------------------------------------------------*/

        unziped_path = filename_to_folder(files[i]); // path to unpacked folder os specified file
        rm_dir(unziped_path); // del recursivily last unpacked folder if it exist
        int file_status = zip_extract(files[i], unziped_path, on_extract_entry, NULL); // unpack

        if (file_status<0) // check to see file consistency
        {
            log_to_console("error","Erro ao abrir arquivo: %s\n Certifique se que o arquivo existe e que se use a opcao \"-f\" seguida de apenas 1 arquivo\n",0,0);
            exit(1);
        }
        
        //chdir(unziped_path); // change binary wd to the unpacked file
        buffer[0]='\0';
        strncpy(buffer,unziped_path,OPT_ARG_MAX_LEN);
        strcat(buffer,"/mathcad/xaml");

        /* Text parser --------------------------------------------------------------------------------------------- */  
        
        text_list = extract_docs(buffer); // text parser

        if(text_list!=NULL){
            print_text_field(&text_list);
        }

        /* Result parser --------------------------------------------------------------------------------------------- */  

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

        /* Worksheet parser --------------------------------------------------------------------------------------------- */  
        
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

        worksheets_xml = parse_worksheet_xml(input, &pos, &text_list, &result_xml, &my_img_list, &cursor); // worksheet parser

        if(worksheets_xml==NULL){
            log_to_console("error","Erro ao processar arquivo worksheet.xml",0,&cursor);
            return 0;
        }
        log_to_console("done","Arquivo worskheet.xml lido com sucesso",0,&cursor);
        free(input);


        if (remove_files==1) // if the flag is set, delete unpacked file
            rm_dir(unziped_path);


        /* LUA CODE --------------------------------------------------- */
        
        check_luaVmachine(L, luaL_dofile(L,script)); // open script
        check_luaVmachine(L, luaL_dostring(L,lua_file_cmd)); // open file
        
        myregion = get_region(worksheets_xml,x,y,z);
        while (myregion.region_id!=-1)
        {
            while (myregion.region_id!=-1)
            {
                while(myregion.region_id!=-1) // myregion has -1 on regio_id if the region read doesnt exist, see get_region() on data_structure.c
                {
                    snprintf(lua_log_buffer,REGION_EXPRESSION_LEN_DEFAULT,"Expressao [%d]: %s - Tipo: %s",z,myregion.expression,myregion.type); // for show
                    log_to_console("lua",lua_log_buffer,0,0);
                    
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