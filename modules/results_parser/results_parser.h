#ifndef RESULTS_PARSER
#define RESULTS_PARSER

#include "../xml_parser/xml_parser.h"
#include "../logger/logger.h"
#include "../data_structures/data_structures.h"

#define DATA_POINTS_CHAR_MAX 50000

/*Parsers------------------------------------------------------------------------*/

void stringdata2floatarray(char* expression1, char* expression2, char* expression3,  plotdata **plotdata_ref);

resultsList *parser_results_xml(char *file, int *pos, int *line_cursor);

#endif