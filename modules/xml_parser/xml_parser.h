#ifndef XML_PARSER
#define XML_PARSER

#define TAG_TYPE_TAG 0
#define TAG_TYPE_VALUE 1
#define ATRIBUTE_LEN_DEFAULT 150
#define TAG_QTDY_DEFAULT 50
#define TAG_ARG_LEN_DEFAULT 2000

//estrutura de uma tag
struct st_tag{
    char tag_arg[TAG_QTDY_DEFAULT][TAG_ARG_LEN_DEFAULT]; // matriz de argumentos, onde [0] é a própria tag de abertura
    int argq; // quantidade de argumentos, incluso tag de abertura
    int tag_type; // vai para 1 quando for lido um valor, e 0 quando for uma tag
};
typedef struct st_tag tag;

//recorta string dentro de outra, fechando o espaço vazio
void strcrop(char *string, const char* token);

//delete ultimo caractere de uma string
void strdel_last(char *string, int qtd);

// retorna ponteiro para vetor contendo arquivo de texto desejado
char *read_asci(const char *path);

// retorna 1 ou 0 caso a comparação entre membro de tag "red->tagarg[id]" com string "str"
int atb_cmp(tag *ref, int id, const char *str);

// retorna string com o valo de um atributo em uma tag
char *atb_read_value(char *atb);

// retorna string com um atributo inteiro de numero "id"
char *atb_get(tag *ref, int id);

// lê uma tag e armazena em um struct "tag"
void read_tag(char *file, int *pos, tag *mytag, int *line_cursor);

#endif