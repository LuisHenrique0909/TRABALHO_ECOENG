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
	-IDesafio_robo \
	-IDesafio_robo/Pontuacao \
	-IMenus/Menu_Equipes \
	-IMenus/Menu_Principal \
	-IMenus/Menu_Ranking \
	-IMenus/Menu_Avaliador \
	-IMenus/Menu_admin \
	-IUtilidades/FILES \
	-IUtilidades/Result \
	-IUtilidades/pausar_sistema \
	-IDesafio_robo/Chaveamento \

# Pasta de build intermediária
BUILD_DIR = build

# Pasta de saída do executável
BIN_DIR = bin

# Nome do executável final
TARGET = $(BIN_DIR)\ecoeng.exe

# ==========================
# Fontes e objetos
# ==========================
SRC = main.c \
      $(wildcard Cadastro/Cadastro_Equipes/*.c) \
      $(wildcard Cadastro/Cadastro_user/*.c) \
      $(wildcard Desafio_robo/*.c) \
	  $(wildcard Desafio_robo/Pontuacao/*.c) \
      $(wildcard Menus/Menu_admin/*.c) \
      $(wildcard Menus/Menu_Equipes/*.c) \
      $(wildcard Menus/Menu_Principal/*.c) \
      $(wildcard Menus/Menu_Ranking/*.c) \
      $(wildcard Menus/Menu_Avaliador/*.c) \
      $(wildcard Utilidades/FILES/*.c) \
      $(wildcard Utilidades/Result/*.c) \
	  $(wildcard Utilidades/pausar_sistema/*.c) \
	  $(wildcard Desafio_robo/Chaveamento/*.c) \

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
	@mkdir "$(BUILD_DIR)\Cadastro\Cadastro_Equipes" 2>nul || echo.
	@mkdir "$(BUILD_DIR)\Cadastro\Cadastro_user" 2>nul || echo.
	@mkdir "$(BUILD_DIR)\Desafio_robo\Pontuacao" 2>nul || echo.
	@mkdir "$(BUILD_DIR)\Menus\Menu_admin" 2>nul || echo.
	@mkdir "$(BUILD_DIR)\Menus\Menu_Equipes" 2>nul || echo.
	@mkdir "$(BUILD_DIR)\Menus\Menu_Principal" 2>nul || echo.
	@mkdir "$(BUILD_DIR)\Menus\Menu_Ranking" 2>nul || echo.
	@mkdir "$(BUILD_DIR)\Menus\Menu_Avaliador" 2>nul || echo.
	@mkdir "$(BUILD_DIR)\Utilidades\FILES" 2>nul || echo.
	@mkdir "$(BUILD_DIR)\Utilidades\Result" 2>nul || echo.
	@mkdir "$(BUILD_DIR)\Utilidades\pausar_sistema" 2>nul || echo.
	@mkdir "$(BUILD_DIR)\Desafio_robo\Chaveamento" 2>nul || echo.
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
	./$(TARGET)
