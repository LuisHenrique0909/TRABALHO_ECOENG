# ==========================
# Makefile para TRABALHO_ECOENG
# Compatível com Windows (CMD / PowerShell)
# ==========================

# Compilador
CC = gcc

# Opções de compilação
# -Wall: mostra todos os warnings
# -g: adiciona informações de debug
# -I<path>: adiciona diretórios onde o compilador deve procurar .h
CFLAGS = -Wall -g \
	-I. \
	-ICadastro/Cadastro_Equipes \
	-ICadastro/Cadastro_user \
	-IDesafio_robo_Pontuacao \
	-IFuncoes_cvs \
	-IMenus/Menu_Equipes \
	-IUtilidades/FILES \
	-IUtilidades/Result

# Pasta de build intermediária
BUILD_DIR = build

# Pasta de saída do executável
BIN_DIR = bin

# Nome do executável final
TARGET = $(BIN_DIR)\ecoeng.exe

# ==========================
# Subpastas do projeto
# ==========================
DIRS = Cadastro/Cadastro_Equipes \
       Cadastro/Cadastro_user \
       Desafio_robo_Pontuacao \
       Funcoes_cvs \
       Menus/Menu_Equipes \
       Utilidades/FILES \
       Utilidades/Result

# ==========================
# Fontes e objetos
# ==========================
SRC = main.c \
      $(wildcard Cadastro/Cadastro_Equipes/*.c) \
      $(wildcard Cadastro/Cadastro_user/*.c) \
      $(wildcard Desafio_robo_Pontuacao/*.c) \
      $(wildcard Funcoes_cvs/*.c) \
      $(wildcard Menus/Menu_Equipes/*.c) \
      $(wildcard Utilidades/FILES/*.c) \
      $(wildcard Utilidades/Result/*.c)

OBJ = $(patsubst %.c,$(BUILD_DIR)/%.o,$(SRC))

# ==========================
# Alvo padrão
# ==========================
all: prepare $(TARGET)

# ==========================
# Linkagem final
# ==========================
$(TARGET): $(OBJ)
	@if not exist $(BIN_DIR) mkdir $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

# ==========================
# Compilação dos .c em .o
# ==========================
$(BUILD_DIR)/%.o: %.c
	@if not exist $(BUILD_DIR)\$(subst /,\,$(dir $@)) mkdir $(BUILD_DIR)\$(subst /,\,$(dir $@))
	$(CC) $(CFLAGS) -c $< -o $@

# ==========================
# Preparação das pastas
# ==========================
prepare:
	@if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
	@if not exist $(BUILD_DIR)\Cadastro mkdir $(BUILD_DIR)\Cadastro
	@if not exist $(BUILD_DIR)\Cadastro\Cadastro_Equipes mkdir $(BUILD_DIR)\Cadastro\Cadastro_Equipes
	@if not exist $(BUILD_DIR)\Cadastro\Cadastro_user mkdir $(BUILD_DIR)\Cadastro\Cadastro_user
	@if not exist $(BUILD_DIR)\Desafio_robo_Pontuacao mkdir $(BUILD_DIR)\Desafio_robo_Pontuacao
	@if not exist $(BUILD_DIR)\Funcoes_cvs mkdir $(BUILD_DIR)\Funcoes_cvs
	@if not exist $(BUILD_DIR)\Menus mkdir $(BUILD_DIR)\Menus
	@if not exist $(BUILD_DIR)\Menus\Menu_Equipes mkdir $(BUILD_DIR)\Menus\Menu_Equipes
	@if not exist $(BUILD_DIR)\Utilidades mkdir $(BUILD_DIR)\Utilidades
	@if not exist $(BUILD_DIR)\Utilidades\FILES mkdir $(BUILD_DIR)\Utilidades\FILES
	@if not exist $(BUILD_DIR)\Utilidades\Result mkdir $(BUILD_DIR)\Utilidades\Result
	@if not exist $(BIN_DIR) mkdir $(BIN_DIR)

# ==========================
# Limpeza
# ==========================S
clean:
	@if exist $(BUILD_DIR) rd /S /Q $(BUILD_DIR)
	@if exist $(TARGET) del $(TARGET)

# ==========================
# Executar o programa
# ==========================
run: all
	./$(TARGET)
