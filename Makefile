# Projeto: Balizar
# Author: João peterson 
# Data de criação: 24.04.2020
# Ultima data de modificação: 24.04.2020 
# 
# Comandos:
# - Compilar e gravar: make 
# - Compilar .hex: make build 
# - Gravar apenas: make burn
# - Gerar dependencias: make depend

# Programa principal e diretórios
MAIN_EXE = parser
MAIN_APP = main
MAIN_DIR = $(shell pwd)

HEADERS_PATH =-Iinclude
LIB_PATH = -L./libs

LIBS =-lzip
LIBS += 

# Compilador
CC = gcc
CFLAGS = -g -std=c99 $(LIB_PATH) $(HEADERS_PATH) $(LIBS)

# Modulos a serem compilados
SOURCE_FILES = $(shell find -name "*.c") # lista arquivos .c
OBJECT_FILES = $(patsubst %.c,%.o,$(SOURCE_FILES)) # futuros arquivos .o 

# Regras:
#Genérica para compilação dos objetos
%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

#Padrão
BuildTask_default : build

.PHONY : clear clearall

#Geração de depêndencias
depend.d: $(SOURCE_FILES) #ATENÇÃO, PODE SER QUE DE PAU QUANDO SE INSERIREM NOVOS MÓDULOS, USAR "make depend.d" APÓS INSERIR NOVO MODULO SÓ PARA GARANTIR
	@echo $(SOURCE_FILES)
	@echo $(OBJECT_FILES)
	$(CC) -MM $(SOURCE_FILES) > $@

-include depend.d
#gravação

build : depend.d $(OBJECT_FILES)
	$(CC) $(OBJECT_FILES) $(CFLAGS) -o $(MAIN_EXE)

clear : 
	$(shell rm -f $(OBJECT_FILES))
	$(shell rm -f $(MAIN_EXE).exe)
	$(shell rm -f depend.d)
