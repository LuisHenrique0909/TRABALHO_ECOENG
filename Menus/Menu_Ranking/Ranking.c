#include "Ranking.h"
#include "Files.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura interna para manipulação de ranking
typedef struct {
    int id;
    char nome_equipe[100];
    char tipo[50];
    float tempo;
    int pontos;
} RegistroRanking;

// ------------------------------------------------------------
// Função de comparação para qsort (ordem decrescente de pontos)
// ------------------------------------------------------------
int comparar_ranking(const void *a, const void *b) {
    const RegistroRanking *ra = a;
    const RegistroRanking *rb = b;
    if (rb->pontos != ra->pontos)
        return rb->pontos - ra->pontos;
    return (ra->tempo > rb->tempo) - (ra->tempo < rb->tempo);
}

// ------------------------------------------------------------
// Lê todos os resultados do CSV
// ------------------------------------------------------------
int carregar_resultados(RegistroRanking *lista, int max) {
    FILE *f = abrir_csv("resultados_robo.csv");
    if (!f) return 0;

    char linha[512];
    fgets(linha, sizeof(linha), f); // cabeçalho
    int count = 0;

    while (fgets(linha, sizeof(linha), f) && count < max) {
        if (sscanf(linha, "%d,%99[^,],%49[^,],%f,%d",
                   &lista[count].id,
                   lista[count].nome_equipe,
                   lista[count].tipo,
                   &lista[count].tempo,
                   &lista[count].pontos) == 5) {
            count++;
        }
    }

    fclose(f);
    return count;
}

// ------------------------------------------------------------
// Mostra o ranking filtrando por tipo de desafio
// ------------------------------------------------------------
void mostrar_ranking(TipoDesafio tipo) {
    RegistroRanking lista[200];
    int total = carregar_resultados(lista, 200);

    if (total == 0) {
        printf("Nenhum resultado registrado.\n");
        return;
    }

    const char *tipo_str = (tipo == SUMO) ? "Sumo" : "Seguidor";

    RegistroRanking filtrado[200];
    int qtd = 0;

    for (int i = 0; i < total; i++) {
        if (strcmp(lista[i].tipo, tipo_str) == 0 ||
            (tipo == SEGUIDOR_LINHA && strstr(lista[i].tipo, "Seguidor"))) {
            filtrado[qtd++] = lista[i];
        }
    }

    if (qtd == 0) {
        printf("Nenhum resultado para o desafio %s.\n", tipo_str);
        return;
    }

    qsort(filtrado, qtd, sizeof(RegistroRanking), comparar_ranking);

    printf("\n===== RANKING (%s) =====\n", tipo_str);
    printf("%-5s %-25s %-10s %-10s %-10s\n",
           "Pos", "Equipe", "Tipo", "Pontos", "Tempo(s)");
    printf("-------------------------------------------------------------\n");

    for (int i = 0; i < qtd; i++) {
        printf("%-5d %-25s %-10s %-10d %-10.2f\n",
               i + 1,
               filtrado[i].nome_equipe,
               filtrado[i].tipo,
               filtrado[i].pontos,
               filtrado[i].tempo);
    }

    printf("-------------------------------------------------------------\n");
    printf("Total de equipes ranqueadas: %d\n", qtd);
}

// ------------------------------------------------------------
// Menu de ranking interativo (usado por admin, avaliador e equipe)
// ------------------------------------------------------------
void menu_ranking() {
    int opc;
    do {
        printf("\n===== MENU RANKING =====\n");
        printf("1 - Ver Ranking Robô Sumô\n");
        printf("2 - Ver Ranking Robô Seguidor de Linha\n");
        printf("0 - Voltar\n");
        printf("Escolha: ");
        scanf("%d", &opc);
        getchar();

        switch (opc) {
            case 1:
                mostrar_ranking(SUMO);
                break;
            case 2:
                mostrar_ranking(SEGUIDOR_LINHA);
                break;
            case 0:
                printf("Voltando ao menu anterior...\n");
                break;
            default:
                printf("Opção inválida.\n");
        }
    } while (opc != 0);
}