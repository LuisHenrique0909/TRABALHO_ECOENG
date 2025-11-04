# ==========================
# Makefile para SISTEMA_ECOENG
# Compatível com Windows (CMD / PowerShell)
# ==========================

# Compilador
CC = gcc

# Opções de compilação
CFLAGS = -Wall -g \
	-I. \
	-ICadastro/Cadastro_Equipes \
	-ICadastro/Cadastro_user \
	-IDesafio_robo \
	-IPontuacao \
	-IMenus \
	-IMenus/Menu_Admin \
	-IMenus/Menu_Avaliador \
	-IMenus/Menu_Equipes \
	-IMenus/Menu_Principal \
	-IMenus/Ranking \
	-IUtilidades/FILES \
	-IUtilidades/Result

# Diretórios de build e saída
BUILD_DIR = build
BIN_DIR = bin

# Nome do executável final
TARGET = $(BIN_DIR)\ecoeng.exe

# ==========================
# Estrutura de diretórios
# ==========================
DIRS = Cadastro/Cadastro_Equipes \
       Cadastro/Cadastro_user \
       Desafio_robo \
       Pontuacao \
       Menus/Menu_Admin \
       Menus/Menu_Avaliador \
       Menus/Menu_Equipes \
       Menus/Menu_Principal \
       Menus/Ranking \
       Utilidades/FILES \
       Utilidades/Result

# ==========================
# Fontes e objetos
# ==========================
SRC = main.c \
      $(wildcard Cadastro/Cadastro_Equipes/*.c) \
      $(wildcard Cadastro/Cadastro_user/*.c) \
      $(wildcard Desafio_robo/*.c) \
      $(wildcard Pontuacao/*.c) \
      $(wildcard Menus/Menu_Admin/*.c) \
      $(wildcard Menus/Menu_Avaliador/*.c) \
      $(wildcard Menus/Menu_Equipes/*.c) \
      $(wildcard Menus/Menu_Principal/*.c) \
      $(wildcard Menus/Ranking/*.c) \
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
	@for %%d in ($(DIRS)) do @if not exist $(BUILD_DIR)\%%d mkdir $(BUILD_DIR)\%%d
	@if not exist $(BIN_DIR) mkdir $(BIN_DIR)

# ==========================
# Limpeza
# ==========================
clean:
	@if exist $(BUILD_DIR) rd /S /Q $(BUILD_DIR)
	@if exist $(TARGET) del $(TARGET)

# ==========================
# Executar o programa
# ==========================
run: all
	$(TARGET)