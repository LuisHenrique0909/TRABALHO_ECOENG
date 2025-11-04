#include "Files.h"

// ================================================================
// Cria o diretório "./dados" caso ele não exista
// ================================================================
void verificar_diretorio_dados() {
    struct stat st = {0};
    if (stat(DATA_DIR, &st) == -1) {
#ifdef _WIN32
        mkdir(DATA_DIR);
#else
        mkdir(DATA_DIR, 0700);
#endif
    }
}

// ================================================================
// Verifica se o arquivo existe dentro de /dados/
// ================================================================
int arquivo_existe(const char *arquivo) {
    char caminho[256];
    snprintf(caminho, sizeof(caminho), "%s%s", DATA_DIR, arquivo);
    struct stat buffer;
    return (stat(caminho, &buffer) == 0);
}

// ================================================================
// Abre arquivo CSV apenas para leitura
// ================================================================
FILE *abrir_csv(const char *arquivo) {
    verificar_diretorio_dados(); // Garante que ./dados/ existe

    char caminho[256];
    snprintf(caminho, sizeof(caminho), "%s%s", DATA_DIR, arquivo);

    FILE *f = fopen(caminho, "r");
    if (!f) {
        return NULL; // Não exibe erro, apenas retorna NULL
    }
    return f;
}

// ================================================================
// Abre CSV para escrita em modo append ("a"), criando se não existir
// Se for um novo arquivo e tiver header, escreve automaticamente o cabeçalho
// ================================================================
FILE *escrever_no_csv(const char *arquivo, const char *header) {
    verificar_diretorio_dados(); // Garante que ./dados/ existe

    char caminho[256];
    snprintf(caminho, sizeof(caminho), "%s%s", DATA_DIR, arquivo);

    int novo_arquivo = !arquivo_existe(arquivo);
    FILE *f = fopen(caminho, "a");

    if (!f) {
        printf("❌ Erro ao abrir arquivo: %s\n", caminho);
        return NULL;
    }

    // Se for novo arquivo, adiciona o cabeçalho
    if (novo_arquivo && header && strlen(header) > 0) {
        fprintf(f, "%s\n", header);
        fflush(f);
    }

    return f;
}

// ================================================================
// Remove '\n' do final de uma string (usada em fgets)
// ================================================================
void limpar_linha(char *linha) {
    if (!linha) return;
    linha[strcspn(linha, "\n")] = '\0';
}