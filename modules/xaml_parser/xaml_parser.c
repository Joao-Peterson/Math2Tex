#include "xaml_parser.h"

#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdio.h>
#include <zip.h>

#define allocate(name,type) type *name=(type*)malloc(sizeof(type))
#define true 0
#define error 1

int paragraph_parser(char *file, int *pos, char *expression, int *line_cursor){
    allocate(mytag,tag);
    char *buffer = (char*)malloc(REGION_EXPRESSION_LEN_DEFAULT/10);
    buffer[0]='\0';
    read_tag(file,pos,mytag,line_cursor);
    log_to_console("tag","Paragraph",0,line_cursor);
    int placeholder; // flag tags where there's supposed to be a future math field expression, 1 for present, 0 for non necessary

    strcat(expression,"para(");
    while(!atb_cmp(mytag,0,"/Paragraph")){

        if(atb_cmp(mytag,0,"Run")){
            placeholder=1;
            read_tag(file,pos,mytag,line_cursor);

            strcat(buffer,"⨌"); // to indicate that it is a text string and not a math string
            while(!atb_cmp(mytag,0,"/Run")){
                placeholder=0;

                if(mytag->tag_type==TAG_TYPE_VALUE){
                    strcat(buffer,atb_get(mytag,0)); // cat every word of a <Run> tag
                    strcat(buffer," ");
                }

                read_tag(file,pos,mytag,line_cursor);
            }

            strdel_last(buffer,1); // del last " " space added above

            if(placeholder==1){
                strcat(expression,TEXT_PLACE_HOLDER); // cat a placeholder 
            }else{
                strcat(expression,"\""); // to pass the expression as string to the lua script
                strcat(expression,buffer);
                strcat(expression,"\"");
            }
        }

        buffer[0]='\0'; // set buffer as empty
        strcat(expression,","); // separate 
        read_tag(file,pos,mytag,line_cursor);
    }
   
    strdel_last(expression,1); // del last comma
    strcat(expression,")"); // separation between paragraphs

    free(buffer);
    log_to_console("/tag","/Paragraph",0,line_cursor);

    return true;
}

int list_parser(char *file, int *pos, char *type, char *expression, int *line_cursor){
    allocate(mytag,tag);
    read_tag(file,pos,mytag,line_cursor);
    log_to_console("tag","List",0,line_cursor);
    strcat(expression,type); // cat Markerstyle to expression
    strcat(expression,"(");

    while(!atb_cmp(mytag,0,"/List")){

        if(atb_cmp(mytag,0,"ListItem")){
            read_tag(file,pos,mytag,line_cursor);
            while(!atb_cmp(mytag,0,"/ListItem")){

                if(text_format(file,pos,mytag,expression,line_cursor)){
                    return error;
                }

                read_tag(file,pos,mytag,line_cursor);
            }
        }

        strcat(expression,",");
        read_tag(file,pos,mytag,line_cursor);
    }

    strdel_last(expression,1); // remover last comma
    strcat(expression,")");
    return true;
}

int text_format(char *file, int *pos, tag *tag_ref, char *expression, int *line_cursor){
    
    if(atb_cmp(tag_ref,0,"Paragraph")){
        if(paragraph_parser(file,pos,expression,line_cursor)!=true){
            log_to_console("error","Error ao ler tag <Paragraph>",0,line_cursor);            
            return error;
        }
    }else if(atb_cmp(tag_ref,0,"List")){
        if(list_parser(file,pos, atb_read_value(atb_get(tag_ref,1)),expression,line_cursor)!=true){
            log_to_console("error","Error ao ler tag <List>",0,line_cursor);            
            return error;
        }
    }else{
        log_to_console("error","Tag inválida encontrada",0,line_cursor);
        log_to_console("tag?",atb_get(tag_ref,0),0,line_cursor);
        return error;
    }

    return true;
}

int document_parser(char *filename, int id, text_field **head){
    char *file = read_asci(filename);
    if(file==NULL){
        return error;
    }

    int pos=0,line_cursor=0;
    
    allocate(mytag,tag);
    tag tag_temp;

    char *expression = (char*)malloc(sizeof(char)*TEXT_FIELD_MAX_CHR);
    expression[0]='\0';

    strcat(expression,"text(");

    log_to_console("File",filename,0,&line_cursor);

    read_tag(file,&pos,mytag,&line_cursor);
    tag_temp=*mytag;

    if(!atb_cmp(mytag,0,"Section")){ // verify that it is the expectaded tag
        log_to_console("tag?",atb_get(mytag,0),0,&line_cursor);
        return error;
    }

    log_to_console("tag","Section",0,&line_cursor);

    read_tag(file,&pos,mytag,&line_cursor);
    while(!atb_cmp(mytag,0,"/Section")){ // wait for the closing tag

        if(text_format(file, &pos, mytag, expression, &line_cursor)){ // call generic tag handler for .xaml            
            return error;
        }
        
        strcat(expression,","); // comma separator
        read_tag(file,&pos,mytag,&line_cursor);
    }

    strdel_last(expression,1); // del last comma
    strcat(expression,")");
    add_text_field(head, id, expression, tag_temp); // add node 
    log_to_console("File","Documento lido com sucesso!",0,&line_cursor);
    return true;
}

text_field *extract_docs(char *path){ //expects the xaml dir containing the packages
    DIR *handle = opendir(path);
    if(handle==NULL){
        printf("Diretorio invalido!\n");
        return NULL;
    }

    struct dirent *entry=NULL;
    char entry_name[REL_PATH_LEN_MAX]={0};
    char entry_name_id[REL_PATH_LEN_MAX]={0};
    int id;
    text_field *head = NULL;

    while((entry=readdir(handle))!=NULL){ // read all the xaml directory 
        
        snprintf(entry_name,REL_PATH_LEN_MAX,"%s/%s",path,entry->d_name); // path to file
        strncpy(entry_name_id,entry_name,REL_PATH_LEN_MAX); // copy for later use 

        if(is_console_log_enable()==1)
            printf(">> %s\n",entry->d_name); // just for show
        
        if(strstr(entry_name,".XamlPackage")!=NULL){ // if it is a package file
            strcrop(entry_name_id,".XamlPackage"); // cut the file extension, so it will be the directory to unzip

            zip_extract(entry_name, entry_name_id, on_extract_entry, NULL); // extract to no file extension path

            strcrop(entry_name_id,"/FlowDocument"); // cut the prefix
            strcrop(entry_name_id,path); // crop the relative path
            id=atoi(entry_name_id); // pick the id

            strcrop(entry_name,".XamlPackage"); // cut the file extension
            strcat(entry_name,"/Xaml/Document.xaml"); // path to the .xaml text file

            if(document_parser(entry_name,id,&head)!=true){
                return NULL;
            }
        }
    }

    return head;
}