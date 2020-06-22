# Projeto: Parser
# Author: João peterson 
# Data de criação: 24.04.2020
# Ultima data de modificação: 14.06.2020 
# 
# Comandos:
# - Compilar .exe: make build 
# - Limpar arquivos compilados: make clear

# Programa principal
MAIN_EXE = m2tex
VERSION = v1.0

# Compilador :
CC = gcc
CFLAGS = -std=c99 $(HEADERS_PATH)
LFLAGS = $(LIB_PATH) $(LIBS)

# Ferramentas :
TAR_UTILITY = tar
TAR_ARG = -c -v -z -f 
RELEASE_DIR = releases/
TAR_NAME = $(RELEASE_DIR)Math2Tex_Win_x86_64_$(VERSION).tar.gz
PACK_FILES = $(MAIN_EXE).exe *.lua *.bat
LUA_SCRIPTS = $(wildcard scripts/*.lua)
LUA_MOVE = $(subst scripts/,,$(LUA_SCRIPTS))
BAT_SCRIPT = scripts/_RUN_M2TEX_.bat
BAT_MOVE = $(subst scripts/,,$(BAT_SCRIPT))

# Diretórios :
HEADERS_PATH = -Iinclude
LIB_PATH =-L./libs

LIBS = ./libs/libzip.a
LIBS += C:/msys64/mingw64/lib/liblua.a


#procura recursiva com wildcard
rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) \
  $(filter $(subst *,%,$2),$d))

SOURCE_FILES += $(call rwildcard,./modules/,*.c) # lista arquivos .c de forma recursiva em ./modules
SOURCE_FILES += $(wildcard *.c) # lista arquivos .c no diretório principal
OBJECT_FILES := $(SOURCE_FILES:.c=.o) # futuros arquivos .o 
DEPENDENCE_FILES := $(OBJECT_FILES:.o=.d) # dependecias de cada source, .d
DEP_FLAGS = -MMD -MF $(@:.o=.d) # flags para gerar depências

# Regras -----------------------------------------------------------------------------------------------

#Padrão
all: build

#Cria arquivo compactado com executavel e scripts
pack: release
	@echo Packing...
	cp -r $(LUA_SCRIPTS) $(LUA_MOVE)
	cp -r $(BAT_SCRIPT) $(BAT_MOVE)
	$(TAR_UTILITY) $(TAR_ARG) $(TAR_NAME) $(PACK_FILES)
	rm -f $(LUA_MOVE)
	rm -f $(BAT_MOVE)
	@echo Done packing.

#Compila como release
release: CC += -Ofast 
release: $(MAIN_EXE).exe

#Verifica se .exe está atualizado e então compila, nesse caso como debug
build: CC += -g 
build: $(MAIN_EXE).exe

#compilar executável
$(MAIN_EXE).exe : $(OBJECT_FILES) 
	@echo Building...
	$(CC) $^ $(LFLAGS) -o $@
	@echo Done Building.

clear : 
	@echo Cleaning...
	rm -f $(SOURCE_FILES:.c=.o)
	rm -f $(SOURCE_FILES:.c=.d)
	rm -f $(MAIN_EXE).exe
	rm -f $(wildcard *.tex)
	@echo Done cleaning.

#Genérica para compilação dos objetos, compila e gera dependencias numa tacada só, então inclui as dependencias
%.o : %.c
	@echo Compiling..
	$(CC) $(CFLAGS) -c $< $(DEP_FLAGS) -o $@ 
-include $(DEPENDENCE_FILES)