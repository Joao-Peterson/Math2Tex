#include "rtf_parser.h"
#include "../string_aux/string_aux.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

//extract png files from input rtf file and save them onto images path
img_list *extract_images(char *rtf_file, char *images_path){
    mkdir(images_path); // create directory to extract the images    
    img_list *return_list = NULL; // list header to be returned, "null" so the add funtion knows to allocate the first nodes

    size_t filepath_size = strlen(images_path)+20; // +20 to acomodate the image filename after the image path folder
    char *image_filename_buffer = (char*)malloc(sizeof(char)*filepath_size); 
    char *input = read_asci(rtf_file);
    if (input == NULL)
    {
        printf("Ao usar a opção -i verificar se há salvo uma versão .rtf do seu arquivo .mcdx. Arquivo requerido: %s\n",rtf_file);
        exit(1);
    }

    FILE *image_handle;

    char *input_cursor = input;
    char *input_end_cursor;
    char png_signature[]={"89504e470d0a1a0a"};
    char IEND_png[]={"49454e44"};
    char *png_hex;
    int how_many_images=0;

    int i=0;
    int j=0;
    uint8_t sym=0;

    // while any png signature is found
    while ( (input_cursor = strstr(input_cursor,png_signature)) != NULL)
    {
        input_end_cursor = strstr(input_cursor,IEND_png); // IEND
        input_end_cursor += strlen(IEND_png)+8; // jump over the last 8 hex digits of IEND chunk and his CRC

        png_hex = strcpyspan(input_cursor,input_end_cursor); // gets binary data from hex
        
        input_cursor = input_end_cursor; // put the two pointers to the end of the png binary span so it can read the next one

        snprintf(image_filename_buffer,filepath_size,"%s/img%d.png",images_path,how_many_images); // create path to the png to be read

        img_add(&return_list,image_filename_buffer);

        image_handle = fopen(image_filename_buffer,"w+b"); // open as binary to prevent windows end of line characaters modification, like "\n" = "\r\n"
        if (image_handle==NULL){
            printf("Arquivo nao pode ser aberto: FILE[\"%s\"]",image_filename_buffer);
            exit(1);
        }

        i=0;
        j=0;
        sym=0;
        
        while(png_hex[i]!='\0') // read symbol by 2 characters, print to file
        {
            sym = (charhex2value(png_hex[i])<<4); // high byte
            sym |= charhex2value(png_hex[i+1]); // lower byte

            fprintf(image_handle,"%c",sym);  

            j++; // each char in output
            i+=2; // each nibble
        }

        fclose(image_handle);    
        
        free(png_hex);

        how_many_images++;
    }

    free(input);
    return return_list;
}