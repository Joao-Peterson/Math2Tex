#ifndef _XAML_PARSER
#define _XAML_PARSER

#define TEXT_FIELD_MAX_CHR 5000
#define REL_PATH_LEN_MAX 60

#include "../xml_parser/xml_parser.h"
#include "../logger/logger.h"
#include "../data_structures/data_structures.h"
#include "../file_handle/file_handle.h"
#include <dirent.h>

//called when reading a <Paragraph>, concatenates text to expression
int paragraph_parser(char *file, int *pos, char *expression, int *line_cursor);

//called on a Document.xaml, write node to text_field list
int document_parser(char *filename, int id, text_field **head);

//receives handle for the xaml directory, unpack and parser it, return pointer to the head of the list text_field
text_field *extract_docs(DIR *handle);

#endif