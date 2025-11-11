#include "Files.h"
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>

// Cria a pasta ./dados caso não exista
void criar_diretorio_dados() {
#ifdef _WIN32
    struct _stat st = {0};
    if (_stat(DATA_DIR, &st) == -1) {
        system("mkdir dados");
    }
#else
    struct stat st = {0};
    if (stat(DATA_DIR, &st) == -1) {
        system("mkdir -p dados");
    }
#endif
}

// Verifica se um arquivo existe dentro de ./dados/
int arquivo_existe(const char *arquivo) {
    char caminho[256];
    snprintf(caminho, sizeof(caminho), "%s%s", DATA_DIR, arquivo);
    FILE *f = fopen(caminho, "r");
    if (!f) return 0;
    fclose(f);
    return 1;
}

// Abre um CSV existente em modo leitura
FILE* abrir_csv(const char *arquivo) {
    char caminho[256];
    snprintf(caminho, sizeof(caminho), "%s%s", DATA_DIR, arquivo);
    FILE *f = fopen(caminho, "r");
    return f;
}

// Abre um CSV para escrita, adicionando cabeçalho se o arquivo não existir
// Usa modo append ("a"), garantindo persistência e não sobrescrita
FILE* escrever_no_csv(const char *arquivo, const char *cabecalho) {
    char caminho[256];
    snprintf(caminho, sizeof(caminho), "%s%s", DATA_DIR, arquivo);

    int existe = arquivo_existe(arquivo);
    
    // CORREÇÃO: Usar modo write se arquivo não existe, append se existe
    FILE *f = fopen(caminho, existe ? "a" : "w");
    if (!f) return NULL;

    if (!existe && cabecalho != NULL) {
        fprintf(f, "%s\n", cabecalho);
    }

    return f;
}

// Conta o número de linhas (útil para gerar IDs automáticos)
long contar_linhas(const char *arquivo) {
    FILE *f = abrir_csv(arquivo);
    if (!f) return 0;

    long count = 0;
    char linha[256];
    
    // CORREÇÃO: Contar linhas usando fgets para maior confiabilidade
    while (fgets(linha, sizeof(linha), f)) {
        count++;
    }

    fclose(f);
    return count;
}

// Limpa completamente um arquivo CSV e reescreve o cabeçalho
Result limpar_arquivo(const char *arquivo) {
    char caminho[256];
    snprintf(caminho, sizeof(caminho), "%s%s", DATA_DIR, arquivo);
    FILE *f = fopen(caminho, "w");
    if (!f) return erro(ERRO_ARQUIVO, "Falha ao limpar o arquivo CSV.");
    
    // CORREÇÃO: Escrever cabeçalho padrão para equipes
    if (strstr(arquivo, "equipes") != NULL) {
        fprintf(f, "ID_EQUIPE,NOME_EQUIPE,LIDER,PARTICIPANTES\n");
    }
    
    fclose(f);
    return ok();
}

#include <string.h>

// Remove o caractere '\n' final de uma string lida por fgets
void limpar_linha(char *str) {
    if (!str) return;
    str[strcspn(str, "\n")] = '\0';
}