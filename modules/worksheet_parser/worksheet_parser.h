#ifndef WORKSHEET_PARSER
#define WORKSHEET_PARSER

#include "../xml_parser/xml_parser.h"
#include "../logger/logger.h"
#include "../data_structures/data_structures.h"

extern int img_counter; // counts how many "imageble" fields are, that is, all regions of the xml except the text regions 

void img_count_add();

int img_count_get();

void img_counter_zero();

/*Parsers------------------------------------------------------------------------*/

//parser de tags genéricas que, para o propósito deste programa, melhor serem ignoradas
int parser_generic(char *file, int *pos, char *tag_name, char *expression, int *line_cursor);

//recebe arquivo vetor, pos para começar, endereço de vetor expressão para armazenar após processar e endereço de linha
int real_parser(char *file, int *pos, char *expression, int *line_cursor);

//recebe arquivo vetor, pos para começar, endereço de vetor expressão para armazenar após processar e endereço de linha
int parens_parser(char *file, int *pos, char *expression, int*line_cursor);

//recebe arquivo vetor, pos para começar, endereço de vetor expressão para armazenar após processar e endereço de linha
int neg_parser(char *file, int *pos, char *expression, int *line_cursor);

//recebe arquivo vetor, pos para começar, endereço de vetor expressão para armazenar após processar e endereço de linha
int apply_parser(char *file, int *pos, char *expression, int *line_cursor);

//recebe arquivo vetor, pos para começar, endereço de vetor expressão para armazenar após processar e endereço de linha
int range_parser(char *file, int *pos, char *expression, int *line_cursor);

//recebe arquivo vetor, pos para começar, endereço de vetor expressão para armazenar após processar e endereço de linha
int id_parser(char *file, int *pos, char *expression, int *line_cursor);

//recebe arquivo vetor, pos para começar, endereço de vetor expressão para armazenar após processar e endereço de linha
int symEval_parser(char *file, int *pos, char *expression, int *line_cursor);

//recebe arquivo vetor, pos para começar, endereço de vetor expressão para armazenar após processar e endereço de linha
int function_parser(char *file, int *pos, char *expression, int *line_cursor);

////recebe arquivo vetor, pos para começar, endereço de vetor expressão para armazenar após processar e endereço de linha e processa diferentes tipos
int type_parser(char *file, int *pos, tag *ref_tag, char *expression, int *line_cursor);

//recebe arquivo vetor, pos para começar, endereço de vetor expressão para armazenar após processar e endereço de linha
int math_parser(char *file, int *pos, char *expression, int result_ref, resultsList **resultlist_ref, int *line_cursor);

//recebe arquivo vetor, pos para começar, endereço de vetor expressão para armazenar após processar e endereço de linha
int text_parser(char *file, int *pos, char *expression, int result_ref, text_field **text_list, resultsList **resultslist_ref, img_list **img_list_ref, int *line_cursor);

//recebe arquivo vetor, pos para começar, inicio da lista a ser guardado após processar e endereço de linha
int parse_region(char *file, int *pos, region **region_obj, tag *header, text_field **text_list, resultsList **resultlist_ref, img_list **img_list_ref, int *line_cursor);

//recebe arquivo vetor, pos para começar, inicio da lista a ser guardado após processar e endereço de linha
int parse_regions(char *file, int *pos, regions **regions_obj, tag *header, text_field **text_list, resultsList **resultlist_ref, img_list **img_list_ref, int *line_cursor);

//recebe arquivo vetor, pos para começar, inicio da lista a ser guardado após processar e endereço de linha
int parse_worksheet(char *file, int *pos, worksheet **worksheet_obj, tag *header, text_field **text_list, resultsList **resultlist_ref, img_list **img_list_ref, int *line_cursor);

//recebe arquivo vetor, pos para começar, inicio da lista a ser guardado após processar e endereço de linha
worksheets *parse_worksheet_xml(char *file, int *pos, text_field **text_list, resultsList **resultlist_ref, img_list **img_list_ref, int *line_cursor);

#endif