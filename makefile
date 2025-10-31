# ==========================
# Makefile para TRABALHO_ECOENG
# Windows (CMD / PowerShell)
# ==========================

# Compilador
CC = gcc

# Opções de compilação
CFLAGS = -Wall -Wextra -g \
	-I. \
	-ICadastro/Cadastro_Equipes \
	-ICadastro/Cadastro_user \
	-IDesafio_robo_Pontuacao \
	-IFuncoes_cvs \
	-IMenus/Menu_Equipes \
	-IUtilidades/FILES \
	-IUtilidades/Result

# Pastas
BUILD_DIR = build
BIN_DIR   = bin
TARGET    = $(BIN_DIR)\ecoeng.exe

# Fontes
SRC = main.c \
      $(wildcard Cadastro/Cadastro_Equipes/*.c) \
      $(wildcard Cadastro/Cadastro_user/*.c) \
      $(wildcard Desafio_robo_Pontuacao/*.c) \
      $(wildcard Funcoes_cvs/*.c) \
      $(wildcard Menus/Menu_Equipes/*.c) \
      $(wildcard Utilidades/FILES/*.c) \
      $(wildcard Utilidades/Result/*.c)

# Objetos
OBJ = $(patsubst %.c,$(BUILD_DIR)/%.o,$(SRC))

# ==========================
# Alvo padrão
# ==========================
all: $(TARGET)

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
