#include "Pontuacao.h"
#include "Files.h"
#include <stdio.h>
#include <string.h>

// MANTENHA esta função EXATAMENTE como estava
void registrar_pontuacao_para_equipe(int id, const char *nome, const char *tipo, float tempo, int pontos) {
    FILE *f = fopen("./dados/resultados_robo.csv", "a");
    if (!f) {
        printf("Erro ao registrar pontuação.\n");
        return;
    }

    // Se arquivo vazio, escreve cabeçalho
    long pos = ftell(f);
    if (pos == 0) fprintf(f, "ID_EQUIPE,NOME_EQUIPE,DESAFIO,TEMPO,PONTOS\n");

    fprintf(f, "%d,%s,%s,%.2f,%d\n", id, nome, tipo, tempo, pontos);
    fclose(f);
}

// Adicione apenas as NOVAS funções que estavam faltando
void listar_pontuacoes() {
    FILE *f = fopen("./dados/resultados_robo.csv", "r");
    if (!f) {
        printf("Nenhuma pontuação registrada.\n");
        return;
    }

    char linha[256];
    printf("\n--- PONTUAÇÕES REGISTRADAS ---\n");
    printf("%-5s %-25s %-15s %-10s %-10s\n", "ID", "Equipe", "Desafio", "Tempo", "Pontos");
    
    fgets(linha, sizeof(linha), f); // Pula cabeçalho
    
    int id, pontos;
    char nome[100], desafio[50];
    float tempo;
    
    while (fgets(linha, sizeof(linha), f)) {
        if (sscanf(linha, "%d,%99[^,],%49[^,],%f,%d", &id, nome, desafio, &tempo, &pontos) == 5) {
            printf("%-5d %-25s %-15s %-10.2f %-10d\n", id, nome, desafio, tempo, pontos);
        }
    }
    
    fclose(f);
}

// Busca pontuação por nome da equipe
int buscar_pontuacao_por_equipe(const char* nome_equipe, Pontuacao* pontuacao) {
    FILE *f = fopen("./dados/resultados_robo.csv", "r");
    if (!f) {
        return 0;
    }

    char linha[256];
    int id, pontos;
    char nome[100], desafio[50];
    float tempo;
    int encontrado = 0;
    
    fgets(linha, sizeof(linha), f); // Pula cabeçalho
    
    while (fgets(linha, sizeof(linha), f)) {
        if (sscanf(linha, "%d,%99[^,],%49[^,],%f,%d", &id, nome, desafio, &tempo, &pontos) == 5) {
            if (strcmp(nome, nome_equipe) == 0) {
                strcpy(pontuacao->nome_equipe, nome);
                pontuacao->pontos = pontos;
                encontrado = 1;
                break;
            }
        }
    }
    
    fclose(f);
    return encontrado;
}

// Atualiza pontuação de uma equipe
Result atualizar_pontuacao(const char* nome_equipe, int nova_pontuacao) {
    FILE *f = fopen("./dados/resultados_robo.csv", "r");
    if (!f) {
        return erro(ERRO_ARQUIVO, "Não foi possível abrir o arquivo de pontuações");
    }

    FILE *temp = fopen("./dados/temp_resultados.csv", "w");
    fprintf(temp, "ID_EQUIPE,NOME_EQUIPE,DESAFIO,TEMPO,PONTOS\n");

    char linha[256];
    int id, pontos;
    char nome[100], desafio[50];
    float tempo;
    int encontrado = 0;
    
    fgets(linha, sizeof(linha), f); // Pula cabeçalho
    
    while (fgets(linha, sizeof(linha), f)) {
        if (sscanf(linha, "%d,%99[^,],%49[^,],%f,%d", &id, nome, desafio, &tempo, &pontos) == 5) {
            if (strcmp(nome, nome_equipe) == 0) {
                fprintf(temp, "%d,%s,%s,%.2f,%d\n", id, nome, desafio, tempo, nova_pontuacao);
                encontrado = 1;
            } else {
                fprintf(temp, "%d,%s,%s,%.2f,%d\n", id, nome, desafio, tempo, pontos);
            }
        }
    }
    
    fclose(f);
    fclose(temp);
    
    if (encontrado) {
        remove("./dados/resultados_robo.csv");
        rename("./dados/temp_resultados.csv", "./dados/resultados_robo.csv");
        return ok();
    } else {
        remove("./dados/temp_resultados.csv");
        return erro(ERRO_INVALIDO, "Equipe não encontrada");
    }
}