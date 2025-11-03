#include "Files.h"

// ------------------ Verificar existência de arquivo ------------------
int arquivo_existe(const char *arquivo) {
    char caminho[256];
    snprintf(caminho, sizeof(caminho), "%s%s", DATA_DIR, arquivo);
    struct stat buffer;
    return (stat(caminho, &buffer) == 0);
}

// ------------------ Abrir CSV ------------------
FILE *abrir_csv(const char *arquivo) {
    char caminho[256];
    snprintf(caminho, sizeof(caminho), "%s%s", DATA_DIR, arquivo);

    FILE *f = fopen(caminho, "r");
    if (!f) {
        printf("Erro ao abrir arquivo %s\n", caminho);
        return NULL;
    }
    return f;
}

// ------------------ Escrever em CSV ------------------
FILE *escrever_no_csv(const char *arquivo, const char *header) {
    char caminho[256];
    snprintf(caminho, sizeof(caminho), "%s%s", DATA_DIR, arquivo);

    int novo_arquivo = !arquivo_existe(arquivo);
    FILE *f = fopen(caminho, "a");
    if (!f) {
        printf("Erro ao abrir arquivo %s para escrita\n", caminho);
        return NULL;
    }

    if (novo_arquivo && header && strlen(header) > 0) {
        fprintf(f, "%s\n", header);
        fflush(f);
    }

    return f;
}

// ------------------ Último ID ------------------
int ultimo_id(const char *nome_arquivo) {
    FILE *f = abrir_csv(nome_arquivo);
    if (!f) return -1;

    char linha[256];
    int ultimo = -1, id;
    fgets(linha, sizeof(linha), f); // pula cabeçalho
    while (fgets(linha, sizeof(linha), f)) {
        if (sscanf(linha, "%d,", &id) == 1)
            ultimo = id;
    }
    fclose(f);
    return ultimo;
}