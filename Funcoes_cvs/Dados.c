#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define DATA_DIR "./dados/"

int arquivo_existe(const char* arquivo) {
    char caminho[256];
    snprintf(caminho, sizeof(caminho), "%s%s", DATA_DIR, arquivo);
    struct stat buffer;
    return (stat(caminho, &buffer) == 0);
}

FILE* abrir_csv(const char* arquivo) {
    char caminho[256];
    snprintf(caminho, sizeof(caminho), "%s%s", DATA_DIR, arquivo);

    FILE* f = fopen(caminho, "r");
    if (!f) {
        printf("Erro ao abrir arquivo %s\n", caminho);
        return NULL;
    }

    return f;
}

FILE* escrever_no_csv(const char* arquivo, const char* header) {
    char caminho[256];
    snprintf(caminho, sizeof(caminho), "%s%s", DATA_DIR, arquivo);

    int novo_arquivo = !arquivo_existe(arquivo);

    FILE* f = fopen(caminho, "a");
    if (!f) {
        printf("Erro ao abrir arquivo %s para escrita\n", caminho);
        return NULL;
    }

    // Se é novo e cabeçalho foi fornecido, escreve
    if (novo_arquivo && header != NULL && strlen(header) > 0) {
        fprintf(f, "%s\n", header);
        fflush(f);
    }

    return f;
}