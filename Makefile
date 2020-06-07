# Projeto: Parser
# Author: João peterson 
# Data de criação: 24.04.2020
# Ultima data de modificação: 03.06.2020 
# 
# Comandos:
# - Compilar .exe: make build 
# - Limpar arquivos compilados: make clear

# Programa principal e diretórios
MAIN_EXE = parser

HEADERS_PATH =-Iinclude
LIB_PATH = -L./libs

LIBS = ./libs/libzip.a
LIBS += ./libs/lua53.dll

# Compilador
CC = gcc
CFLAGS = -g -std=c99 $(HEADERS_PATH)
LFLAGS = $(LIB_PATH) $(LIBS)

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

build : $(OBJECT_FILES)
	$(CC) $(OBJECT_FILES) $(LFLAGS) -o $(MAIN_EXE)

clear : 
	$(shell rm -f $(OBJECT_FILES))
	$(shell rm -f $(MAIN_EXE).exe)
	$(shell rm -f depend.d)
