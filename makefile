# Compilador e flags
CC = gcc
AR = ar
CFLAGS = -Wall -I. -ICadastro -ICadastro/Cadastro_Equipes -ICadastro/Cadastro_user -IResult -IDesafio_robo -IDesafio_robo/Pontuacao -IMenus -IMenus/Menu_Equipes -IMenus/Menu_Avaliadores -IMenus/Menu_professores -IMenus/Menu_principal -IFuncoes_cvs -IUtilidades -IUtilidades/FILES -g -std=c11

# Diretórios
OBJ_DIR = obj
BIN_DIR = bin
LIB_DIR = lib

# Executável principal
TARGET = $(BIN_DIR)/SistemaEcoEng.exe

# Fontes
SRC_Cadastro = $(wildcard Cadastro/*.c Cadastro/Cadastro_Equipes/*.c Cadastro/Cadastro_user/*.c)
SRC_Desafio_robo = $(wildcard Desafio_robo/Pontuacao/*.c)
SRC_Menus = $(wildcard Menus/Menu_Equipes/*.c Menus/Menu_Avaliadores/*.c Menus/Menu_professores/*.c Menus/Menu_principal/*.c)
SRC_Core = $(wildcard Funcoes_cvs/*.c Utilidades/FILES/*.c Result/*.c)
SRC_MAIN = main.c

# Objetos
OBJ_Cadastro = $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRC_Cadastro))
OBJ_Desafio_robo = $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRC_Desafio_robo))
OBJ_Menus = $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRC_Menus))
OBJ_Core = $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRC_Core))
OBJ_MAIN = $(OBJ_DIR)/main.o

# Bibliotecas estáticas
LIB_Cadastro = $(LIB_DIR)/libCadastro.a
LIB_Desafio_robo = $(LIB_DIR)/libDesafio_robo.a
LIB_Menus = $(LIB_DIR)/libMenus.a
LIB_Core = $(LIB_DIR)/libCore.a

# Dependências
DEPENDS = $(OBJ_Cadastro:.o=.d) $(OBJ_Desafio_robo:.o=.d) $(OBJ_Menus:.o=.d) $(OBJ_Core:.o=.d) $(OBJ_MAIN:.o=.d)

# --- Regras principais ---
all: $(TARGET)

# Executável principal
$(TARGET): $(LIB_Cadastro) $(LIB_Desafio_robo) $(LIB_Menus) $(LIB_Core) $(OBJ_MAIN)
	@if not exist "$(BIN_DIR)" mkdir "$(BIN_DIR)"
	$(CC) $(CFLAGS) "$(OBJ_MAIN)" "$(LIB_Cadastro)" "$(LIB_Desafio_robo)" "$(LIB_Menus)" "$(LIB_Core)" -o "$(TARGET)"

# Criar bibliotecas estáticas
$(LIB_Cadastro): $(OBJ_Cadastro)
	@if not exist "$(LIB_DIR)" mkdir "$(LIB_DIR)"
	$(AR) rcs "$(LIB_Cadastro)" $(OBJ_Cadastro)

$(LIB_Desafio_robo): $(OBJ_Desafio_robo)
	@if not exist "$(LIB_DIR)" mkdir "$(LIB_DIR)"
	$(AR) rcs "$(LIB_Desafio_robo)" $(OBJ_Desafio_robo)

$(LIB_Menus): $(OBJ_Menus)
	@if not exist "$(LIB_DIR)" mkdir "$(LIB_DIR)"
	$(AR) rcs "$(LIB_Menus)" $(OBJ_Menus)

$(LIB_Core): $(OBJ_Core)
	@if not exist "$(LIB_DIR)" mkdir "$(LIB_DIR)"
	$(AR) rcs "$(LIB_Core)" $(OBJ_Core)

# Compilar objetos e gerar dependências
$(OBJ_DIR)/%.o: %.c
	@if not exist "$(dir $@)" mkdir "$(dir $@)"
	$(CC) $(CFLAGS) -MMD -MP -c "$<" -o "$@"

# Incluir dependências
-include $(DEPENDS)

# Rodar executáveis
run: $(TARGET)
	"$(TARGET)"

# Limpeza
clean:
	del /q "$(OBJ_DIR)\**\*.o" 2>nul
	del /q "$(OBJ_DIR)\**\*.d" 2>nul
	del /q "$(LIB_DIR)\*.a" 2>nul
	del /q "$(BIN_DIR)\*.exe" 2>nul

.PHONY: all clean run
