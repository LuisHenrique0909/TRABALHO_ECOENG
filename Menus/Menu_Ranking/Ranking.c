#include "Ranking.h"
#include "Files.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char nome_equipe[100];
    int pontos;
    float tempo;
} Ranking;

int comparar_pontos(const void *a, const void *b) {
    const Ranking *ra = a, *rb = b;
    return rb->pontos - ra->pontos; // decrescente
}

void mostrar_ranking(TipoDesafio tipo) {
    FILE *f = abrir_csv("resultados_robo.csv");
    if (!f) {
        printf("Nenhum resultado registrado.\n");
        return;
    }

    char tipo_str[20];
    strcpy(tipo_str, tipo == SUMO ? "Sumo" : "Seguidor");

    char linha[512];
    fgets(linha, sizeof(linha), f); // pula cabeçalho

    Ranking lista[100];
    int count = 0;

    int id, pontos;
    float tempo;
    char nome[100], tipo_csv[50];

    while (fgets(linha, sizeof(linha), f)) {
        if (sscanf(linha, "%d,%99[^,],%49[^,],%f,%d", &id, nome, tipo_csv, &tempo, &pontos) == 5) {
            // Verifica se o tipo no CSV corresponde ao desafio atual
            if (strcmp(tipo_csv, tipo_str) == 0 ||
                (tipo == SEGUIDOR_LINHA && strstr(tipo_csv, "Seguidor"))) {

                strncpy(lista[count].nome_equipe, nome, sizeof(lista[count].nome_equipe));
                lista[count].pontos = pontos;
                lista[count].tempo = tempo;
                count++;
            }
        }
    }

    fclose(f);

    if (count == 0) {
        printf("Nenhum resultado para este desafio.\n");
        return;
    }

    // Ordena e mostra
    qsort(lista, count, sizeof(Ranking), comparar_pontos);

    printf("\n--- RANKING: %s ---\n", tipo_str);
    printf("%-5s %-30s %-10s %-10s\n", "Pos", "Equipe", "Pontos", "Tempo");
    for (int i = 0; i < count; i++)
        printf("%-5d %-30s %-10d %-10.2f\n", i + 1, lista[i].nome_equipe, lista[i].pontos, lista[i].tempo);

    printf("\nTotal de equipes ranqueadas: %d\n", count);
}

void menu_ranking() {
    int opc;
    do {
        printf("\n===== MENU RANKING =====\n");
        printf("1 - Ranking Robô Sumô\n");
        printf("2 - Ranking Robô Seguidor de Linha\n");
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