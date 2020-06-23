#include "data_structures.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define allocate(name,type) type *name=(type*)malloc(sizeof(type))

#define true 0
#define error 1

void begin_worksheet_list(worksheets **myworksheets){
    (*myworksheets)->next=NULL; // nulo pois é o primeiro elemento
    return ;
}

//anexar um nó em uma lista de worksheet, recebe endereço do ponteiro para a lista e struct de informação worksheet
void add_worksheet(worksheets **worksheets_ref, worksheet *data){
    worksheets *worksheets_temp = *worksheets_ref; // temporário q recebe ponteiro
    allocate(node,worksheets); // aloca futuro nó
    node->worksheet_data=data; // recebe informação
    node->next=NULL; // inicializa

    if((*worksheets_ref)==NULL){ // se ponteiro rpassado é nulo
        (*worksheets_ref)=node; // recebe node já montado
        return;
    }

    while(worksheets_temp->next!=NULL){ // percorre lista
        worksheets_temp=worksheets_temp->next; 
    }

    worksheets_temp->next=node;
    return;
}

//retorna endereço de uma worksheet da lista worksheets
worksheets *read_worksheet(worksheets **worksheets_ref, int id){
    worksheets *worksheets_temp=*worksheets_ref; // recebe ponteiro repassado
    allocate(node,worksheets); // nó para retornar depois
    
    if((*worksheets_ref)==NULL){ // se ponteiro repassado é nulo
        return node; // retorna memória alocada vazia de propósito
    }
    free(node); // libera espaço não utilizado em caso de ponterio inválido, como acima

    while(id>0){ // percorre até chegar em id
        worksheets_temp=worksheets_temp->next;
        id--;
    }

    node=worksheets_temp; // recebe endereço da worksheet
    return node; // retorna dados da worksheet
}

//inicializa lista de regions
void begin_worksheet(worksheet **myworksheet){
    (*myworksheet)->next=NULL; // nulo pois é o primeiro elemento
    return ;
}

//anexar uma regions em uma worksheet, recebe endereço do ponteiro para a worksheet e struct de informação regions
void add_regions(worksheet **worksheet_ref, regions *data){
    worksheet *worksheet_temp = *worksheet_ref;
    allocate(node,worksheet); // aloca futuro nó
    node->regions_data=data; // recebe informação
    node->next=NULL; // inicializa

    if((*worksheet_ref)==NULL){ // se ponteiro rpassado é nulo
        (*worksheet_ref)=node; // recebe node já montado
        return;
    }

    worksheet_temp=*worksheet_ref; // recebe ponteiro

    while(worksheet_temp->next!=NULL){ // percorre lista
        worksheet_temp=worksheet_temp->next; 
    }

    worksheet_temp->next=node;
    return;
}

//retorna endereço de uma regions da lista worksheet
worksheet *read_regions(worksheet **worksheet_ref, int id){
    worksheet *worksheet_temp=*worksheet_ref; // recebe ponteiro repassado
    allocate(node,worksheet); // nó para retornar depois
    
    if((*worksheet_ref)==NULL){ // se ponteiro repassado é nulo
        return node; // retorna memória alocada vazia de propósito
    }
    free(node); // libera espaço não utilizado em caso de ponterio inválido, como acima

    while(id>0){ // percorre até chegar em id
        worksheet_temp=worksheet_temp->next;
        id--;
    }

    node=worksheet_temp; // recebe endereço 
    return node; // retorna dados
}

//inicializa lista de region
void begin_regions(regions **myregions){
    (*myregions)->next=NULL; // nulo pois é o primeiro elemento
    return ;
}

//anexar uma region em uma regions, recebe endereço do ponteiro para a regions e struct de informação region
void add_region(regions **regions_ref, region *data){
    regions *regions_temp=*regions_ref; // recebe ponteiro
    allocate(node,regions); // aloca futuro nó
    node->region_data=data; // recebe informação
    node->next=NULL; // inicializa

    if((*regions_ref)==NULL){ // se ponteiro repassado é nulo
        (*regions_ref)=node; // recebe node já montado
        return;
    }

    while(regions_temp->next!=NULL){ // percorre lista
        regions_temp=regions_temp->next; 
    }

    regions_temp->next=node;
    return;
}

//retorna endereço de uma regions da lista regions
regions *read_region(regions **regions_ref, int id){
    
    if((*regions_ref)==NULL){ // se ponteiro repassado é nulo
        return NULL; 
    }

    regions *regions_temp = *regions_ref;
    
    while(id>0){ // percorre até chegar em id
        if(regions_temp==NULL)
            return NULL;

        id--;            
        regions_temp=regions_temp->next;
    }

    return regions_temp;
}

//retorna objeto region de uma lista regions de uma lista worksheets
region get_region(worksheets *xml, int worksheet_id, int regions_id, int region_id){
    allocate(node, region);
    node->region_id=-1; // to indicate failure
    
    worksheets *myworksheet = read_worksheet(&xml,worksheet_id); // retorna endereço de um nó worksheets que contém um worksheet 
    if (myworksheet==NULL)
        return *node;
    worksheet *myregions = read_regions(&(myworksheet->worksheet_data),regions_id); // retorna endereço de um nó worksheet que contém um regions 
    if (myregions==NULL)
        return *node;
    regions *myregion = read_region(&(myregions->regions_data), region_id); // retorna endereço de um nó regions que contém um region
    if (myregion==NULL)
        return *node;
    
    free(node);
    return *(myregion->region_data); // retorna objeto region
}

//inicializa plotdata list
void begin_plotdata_list(plotdata **plotdata_ref){
    (*plotdata_ref)->next=NULL;
}

//adiciona nó plotdata
void add_plotdata(plotdata **plotdata_ref, float x, float y, float z){
    allocate(node,plotdata);
    plotdata *temp=*plotdata_ref;
    node->value1=x;
    node->value2=y;
    node->value3=z;
    node->next=NULL;

    if((*plotdata_ref)==NULL){
        (*plotdata_ref)=node;
        return;
    }

    while(temp->next!=NULL){
        temp=temp->next;
    }

    temp->next=node;
}

//lê nós de uma posição especifica
plotdata *read_plotdata(plotdata **plotdata_ref, int id){
    plotdata *temp=*plotdata_ref;
    allocate(node,plotdata);

    if((*plotdata_ref)==NULL){
        return node;
    }
    free(node);

    while(id<0){
        if(temp->next!=NULL){
            temp=temp->next;
        }else{
            break;
        }
        id--;
    }

    return temp;
}

//inicializa results list
void begin_results_list(resultsList **myresultslist){
    (*myresultslist)->next=NULL; // nulo pois é o primeiro elemento
    return ;
}

//anexar um nó em uma lista de resultslist, recebe endereço do ponteiro para a lista e struct de informação worksheet
void add_result(resultsList **resultslist_ref, int id, char *type, plotdata *plot_data){
    resultsList *resultslist_temp = *resultslist_ref; // temporário q recebe ponteiro
    allocate(node,resultsList); // aloca futuro nó
    // recebe informação
    node->result_data=plot_data; 
    strcpy(node->type,type);
    node->id=id;
    node->next=NULL; // inicializa

    if((*resultslist_ref)==NULL){ // se ponteiro rpassado é nulo
        (*resultslist_ref)=node; // recebe node já montado
        return;
    }

    while(resultslist_temp->next!=NULL){ // percorre lista
        resultslist_temp=resultslist_temp->next; 
    }

    resultslist_temp->next=node;
    return;
}

//retorna endereço de um nó da lista resultslist
resultsList *read_result(resultsList **resultslist_ref, int id){
    resultsList *resultslist_temp=*resultslist_ref; // recebe ponteiro repassado
    allocate(node,resultsList); // nó para retornar depois
    
    if((*resultslist_ref)==NULL){ // se ponteiro repassado é nulo
        return node; // retorna memória alocada vazia de propósito
    }
    free(node); // libera espaço não utilizado em caso de ponterio inválido, como acima

    while(id!=resultslist_temp->id){ // percorre até chegar em id
        if(resultslist_temp->next!=NULL){
            resultslist_temp=resultslist_temp->next;
        }else{
            break;
        }
    }

    node=resultslist_temp; // recebe endereço da worksheet
    return node; // retorna dados da worksheet
}

//retorna valor expecifico de uma fila de dados em uma fila de resultados
float get_result(resultsList **resultslist_ref, int id){
    resultsList *result_address = read_result(resultslist_ref, id); // pega endereço do nó de resultado
    plotdata *data_head_addres = result_address->result_data; // endereço da cabeça da lista de vetor resultado
    plotdata *data_address = read_plotdata(&data_head_addres,0); // endereço do primeiro valor
    return data_address->value1; // retorna valor da primeira posição
}

//initialize list by the head
void begin_text_field(text_field **head){
    if(*head==NULL)
        return;

    *head = (text_field*)malloc(sizeof(text_field));
    (*head)->next=NULL;
}

//add a node a the last positions of the list
void add_text_field(text_field **head, int id, char *text, tag tags){
    text_field *cursor = *head;
    allocate(entry,text_field);
    
    entry->id=id;
    entry->tags=tags;
    entry->text=text;
    entry->next=NULL;

    if(cursor==NULL){
        *head = entry;
        return;
    }

    while(cursor->next!=NULL){
        cursor=cursor->next;
    }

    cursor->next=entry;
}

//get the text from the node that contains the right id
char *get_text_field(text_field **head, int id){
    text_field *cursor = *head;
    
    if(cursor==NULL){
        return NULL;
    }

    while(cursor->id!=id){
        cursor=cursor->next;
    }

    return cursor->text;
}

//run trought the list printing entries
void print_text_field(text_field **head){
    char print_buffer[100];
    text_field *cursor = *head;
    log_to_console("msg","!---------------Lista de textos----------------!",0,0);
    while(cursor!=NULL){
        snprintf(print_buffer,100,"ID: %i , TEXTO: %s",cursor->id,cursor->text);
        log_to_console("msg",print_buffer,0,0);
        cursor=cursor->next;
    }
}

//add new nide o list of images
void img_add(img_list **head, char* filename){
    img_list *cursor = *head;
    
    allocate(node,img_list);

    if (strlen(filename)>PATH_LENGTH_MAX)
    {
        printf("[ERROR]: Path da string é maior que \"PATH_LENGTH_MAX\" definido em \"data_structures.h\"\n");
    }
    
    strncpy(node->path,filename,PATH_LENGTH_MAX);
    node->next=NULL;

    if (*head == NULL)
    {
        *head = node;
        return;
    }
    else
    {
        while(cursor->next!=NULL)
            cursor = cursor->next;

        cursor->next = node;
        return;
    }
}

//get filename from node list in position "pos"
char *img_get_path(img_list **head,int pos){
    if (*head==NULL)
        return NULL;

    img_list *cursor = *head;

    while(pos>0)
    {
        cursor=cursor->next;

        if(cursor==NULL)
            return NULL;
        
        pos--;
    }

    return cursor->path;
}

//change the path of a node in a img_list
void img_modify(img_list **head, char *new_path, int pos){
    if (*head==NULL)
        return;

    img_list *cursor = *head;

    while(pos>0)
    {
        cursor=cursor->next;

        if(cursor==NULL)
            return;        
        pos--;
    }

    strncpy(cursor->path,new_path,PATH_LENGTH_MAX);
    return;
}

//runs trught the list printing it
void img_print_list(img_list **head){
    char print_buffer[100];
    img_list *cursor = *head;
    if(cursor == NULL)
        return;

    int i=0;

    log_to_console("msg","----------- Lista de imagens -----------",0,0);
    while(cursor->next!=NULL){
        snprintf(print_buffer,100,"Imagem [%d] : %s\n",i,cursor->path);
        log_to_console("msg",print_buffer,0,0);
        i++;
        cursor = cursor->next;
    }
}