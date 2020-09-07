#ifndef _XML_PARSER
#define _XML_PARSER

#include <stdint.h>
#include <stdbool.h>

// Tag class defines
#define TAG_TYPE_TAG 0
#define TAG_TYPE_VALUE 1
#define ATRIBUTE_LEN_DEFAULT 150
#define TAG_ATB_QTD_DEFAULT 100
#define TAG_ATB_LEN_DEFAULT 2000
#define TAG_FILENAME_LEN 150

// File mapping
#define FMAP_READ 0x1
#define FMAP_WRITE 0x2

/* ----------------------------------------------------- Lex class ------------------------------------------------------ */

// Lex is the smallest unit of a xml file in terms of reading, that is, a tag or a simple plain value 

typedef struct FMAP
{
    
}
FMAP;

// types of lex
typedef enum LexType
{
    LEX_TYPE_TAG = 1,
    LEX_TYPE_VALUE,
    LEX_TYPE_COUNT
} 
LexType;

typedef struct Lex
{
    LexType type;
    char *fileInMemory; // pointer to file in memory
    int filePos; // pos in file to read after
} 
Lex;

typedef struct Tag 
{
    //parent
    Lex header;
    //members
    char atb[TAG_ATB_QTD_DEFAULT][TAG_ATB_LEN_DEFAULT]; // arguments list
    int atbq; // total atributes quantity
    bool atbUsePrefix; // 1 if atb_cmp should ignore the prefix on tag names
    //methods
    bool (*atb_cmp)(Tag*,int,const char*); // compares the atribute int to the constant expression, returns 1 if true, 0 if false
    char* (*atb_get)(Tag*,int); // gets the atribute int from the tag
    char* (*atb_get_value)(Tag*,int); // dismantles the atribute, returning only the part inside ""
    void (*read_tag)(Tag*); // reads tag an writes info to the object
} 
Tag;

//constructors
Lex* LexNew(char *file_mem, int file_pos); // creates a new lex object
Tag* TagNew(); // creates a new tag object
Tag* TagCopy(Tag* ref); // copies one to another

/* ----------------------------------------------------- Windows File handling ------------------------------------------------------ */

#if defined (__WIN32__) || defined (__WIN64__)

// Get win system error as string
char *get_win_error();

//map file to memory using windows memory api, needs two pointers, to file and to map object, returns adress pointer
char *fmap_win(char *filename, void **file_handle_ptr, void **file_map_obj_ptr, uint8_t options);

// closes view, file handle and map object handle
void fmap_close(char *filename, void **file_handle_ptr, void **file_map_obj_ptr);

#endif


/* ----------------------------------------------------- Windows File handling ------------------------------------------------------ */

//recorta string dentro de outra, fechando o espaço vazio
void strcrop(char *string, const char* token);

//delete ultimo caractere de uma string
void strdel_last(char *string, int qtd);

// retorna ponteiro para vetor contendo arquivo de texto desejado
char *read_asci(const char *path);

#endif