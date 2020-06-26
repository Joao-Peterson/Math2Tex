#ifndef __WORKSHEET_OPERATORS
#define __WORKSHEET_OPERATORS

#include "../xml_parser/xml_parser.h"
#include "../logger/logger.h"
#include "../data_structures/data_structures.h"

/*Parsers------------------------------------------------------------------------*/

//parser de tags genéricas que, para o propósito deste programa, melhor serem ignoradas
int parser_generic(char *file, int *pos, char *tag_name, char *expression, int *line_cursor);

//matrix parser
int matrix_parser(char *file, int *pos, tag *header, char *expression, int *line_cursor);

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
int function_parser(char *file, int *pos, char *expression, int *line_cursor);

//recebe arquivo vetor, pos para começar, endereço de vetor expressão para armazenar após processar e endereço de linha e processa diferentes tipos
int type_parser(char *file, int *pos, tag *ref_tag, char *expression, int *line_cursor);

#endif