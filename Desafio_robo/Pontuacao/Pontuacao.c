#include "Pontuacao.h"
#include "Files.h"
#include <stdio.h>
#include <string.h>

// Adiciona ou atualiza pontuação no arquivo CSV de resultados
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