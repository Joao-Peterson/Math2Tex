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
    read_tag(file,pos,mytag,line_cursor);
    log_to_console("tag","Paragraph",0,line_cursor);
    while(!atb_cmp(mytag,0,"/Paragraph")){

        if(mytag->tag_type==TAG_TYPE_VALUE){
            strcat(expression,atb_get(mytag,0));
            strcat(expression," ");
        }

        read_tag(file,pos,mytag,line_cursor);
    }
    
    log_to_console("/tag","/Paragraph",0,line_cursor);

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

    log_to_console("File",filename,0,&line_cursor);

    read_tag(file,&pos,mytag,&line_cursor);
    tag_temp=*mytag;

    if(!atb_cmp(mytag,0,"Section")){ // verify that it is the expectaded tag
        log_to_console("tag?",atb_get(mytag,0),0,&line_cursor);
        return error;
    }

    log_to_console("tag","Section",0,&line_cursor);

    while(!atb_cmp(mytag,0,"/Section")){ // wait for the closing tag

        if(atb_cmp(mytag,0,"Paragraph")){
            if(paragraph_parser(file,&pos,expression,&line_cursor)!=true){
                log_to_console("error","Error ao ler tag <Paragraph>",0,&line_cursor);            
                return error;
            }

            strcat(expression,"\n"); // separation between paragraphs
        }

        read_tag(file,&pos,mytag,&line_cursor);
    }

    strdel_last(expression,1); // delete last separation
    add_text_field(head, id, expression, tag_temp); // add node 
    log_to_console("File","Documento lido com sucesso!",0,&line_cursor);
    return true;
}

text_field *extract_docs(DIR *handle){ //expects the xaml dir containing the packages
    struct dirent *entry=NULL;
    char entry_name[REL_PATH_LEN_MAX]={0};
    char entry_name_id[REL_PATH_LEN_MAX]={0};
    char *buffer;
    int id;
    text_field *head = NULL;

    while((entry=readdir(handle))!=NULL){ // read all the xaml directory 
        strncpy(entry_name,"xaml/",REL_PATH_LEN_MAX); 
        strncpy(entry_name_id,"xaml/",REL_PATH_LEN_MAX); // fixes the path to be relative to mathcad dir
        strcat(entry_name,entry->d_name);
        strcat(entry_name_id,entry->d_name); // copy the name two times
        printf(">> %s\n",entry_name);
        
        if(strstr(entry_name,".XamlPackage")!=NULL){ // if it is a package file
            strcrop(entry_name_id,".XamlPackage"); // cut the file extension, so it will be the directory to unzip

            zip_extract(entry_name, entry_name_id, on_extract_entry, NULL); // extract

            strcrop(entry_name_id,"FlowDocument"); // cut the prefix
            strcrop(entry_name_id,"xaml/");
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