#ifndef RESULTS_PARSER
#define RESULTS_PARSER

#include "../xml_parser/xml_parser.h"
#include "../logger/logger.h"
#include "../data_structures/data_structures.h"

/*Parsers------------------------------------------------------------------------*/

void stringdata2floatarray(char* expression1, char* expression2, char* expression3,  plotdata **plotdata_ref);

int parser_results_xml(char *file, int *pos, resultsList **resultslist_ref, int *line_cursor);

#endif