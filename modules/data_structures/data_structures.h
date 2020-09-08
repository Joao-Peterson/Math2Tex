#ifndef DATA_STRUCTURES
#define DATA_STRUCTURES

#include "../xml_parser/xml_parser.h"
#include "../logger/logger.h"

#define WORKSHEET_QTDY_DEFAULT 10
#define REGIONS_QTDY_DEFAULT 20
#define REGION_QTDY_DEFAULT 100
#define REGION_TYPE_LEN_DEFAULT 25
#define REGION_EXPRESSION_LEN_DEFAULT 12000
#define PATH_LENGTH_MAX 250

/*Estruturas--------------------------------------------------------------------*/

//estrutura de uma região
struct st_region{
    tag tags;
    int region_id; 
    float size[2]; // tamanho do campo [largura,altura]
    float pos[2]; // posição [do topo, da esquerda] da folha
    char type[REGION_TYPE_LEN_DEFAULT]; // tipo de região, "math", "text", "plot"
    int resultRef; // referência para result.xml caso haja calculo numérico
    char expression[REGION_EXPRESSION_LEN_DEFAULT]; // expressão, avaliada conforme o tipo
};
typedef struct st_region region;

//estrutura das regiões
struct st_regions{
    tag tags;
    region *region_data;
    struct st_regions *next;
};
typedef struct st_regions regions;

//estrutura worksheet
struct st_worksheet{
    tag tags;
    regions *regions_data;
    struct st_worksheet *next;
};
typedef struct st_worksheet worksheet;

//estrutura worksheets
struct st_worksheets{
    worksheet *worksheet_data;
    struct st_worksheets *next;
};
typedef struct st_worksheets worksheets;

//estrutura de fila para vetores de pontos
struct st_plotdata{
    double value1;
    double value2;
    double value3;
    struct st_plotdata *next;
};
typedef struct st_plotdata plotdata;  

//estrutura de fila para resultados
struct st_resultsList{
    int id;
    char type[25];
    plotdata *result_data;
    struct st_resultsList *next;
};
typedef struct st_resultsList resultsList;

struct st_text_field{
    int id;
    tag tags;
    char *text;
    struct st_text_field *next;
};
typedef struct st_text_field text_field;

struct img_list_st{
    char path[PATH_LENGTH_MAX];
    struct img_list_st *next;
};
typedef struct img_list_st img_list;


/*Listas------------------------------------------------------------------------*/

void begin_worksheet_list(worksheets **myworksheets);

void add_worksheet(worksheets **worksheets_ref, worksheet *data);

worksheets *read_worksheet(worksheets **worksheets_ref, int id);


void begin_worksheet(worksheet **myworksheet);

void add_regions(worksheet **worksheet_ref, regions *data);

worksheet *read_regions(worksheet **worksheet_ref, int id);


void begin_regions(regions **myregions);

void add_region(regions **regions_ref, region *data);

regions *read_region(regions **regions_ref, int id);

//retorna objeto region do arquivo, folha e lista desejada
region get_region(worksheets *xml, int worksheet_id, int regions_id, int region_id);


void begin_plotdata_list(plotdata **plotdata_ref);

void add_plotdata(plotdata **plotdata_ref, float x, float y, float z);

plotdata *read_plotdata(plotdata **plotdata_ref, int id);


void begin_results_list(resultsList **myresultslist);

void add_result(resultsList **resultslist_ref, int id, char *type, plotdata *plot_data);

resultsList *read_result(resultsList **resultslist_ref, int id);

float get_result(resultsList **resultslist_ref, int id);


void begin_text_field(text_field **head);

void add_text_field(text_field **head, int id, char *text, tag tags);

char *get_text_field(text_field **head, int id);

//run trought the list printing entries
void print_text_field(text_field **head);


//add new nide o list of images
void img_add(img_list **head, char* filename);

//get filename from node list in position "pos"
char *img_get_path(img_list **head,int pos);

//change the path of a node in a img_list
void img_modify(img_list **head, char *new_path, int pos);

//runs trught the list printing it
void img_print_list(img_list **head);

#endif