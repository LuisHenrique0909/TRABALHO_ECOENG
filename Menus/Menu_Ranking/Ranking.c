#include "Ranking.h"
#include "Files.h"
#include "pausar_sistema.h"
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
        printf("\nNenhum resultado registrado.\n");
        return;
    }

    const char *tipo_str = (tipo == SUMO) ? "Sumo" : "Seguidor de Linha";
    const char *tipo_csv = (tipo == SUMO) ? "Sumo" : "Seguidor";

    RegistroRanking filtrado[200];
    int qtd = 0;

    // Filtrar por tipo de desafio
    for (int i = 0; i < total; i++) {
        if (strstr(lista[i].tipo, tipo_csv) != NULL) {
            filtrado[qtd++] = lista[i];
        }
    }

    if (qtd == 0) {
        printf("\nNenhum resultado para o desafio %s.\n", tipo_str);
        return;
    }

    // Ordenar por pontos (decrescente) e tempo (crescente)
    qsort(filtrado, qtd, sizeof(RegistroRanking), comparar_ranking);

    printf("\n=== RANKING - ROBÔ %s ===\n", tipo_str);
    printf("─────────────────────────────────────────────────────────────────\n");
    printf("%-5s %-25s %-12s %-10s %-10s\n",
           "POS", "EQUIPE", "DESAFIO", "PONTOS", "TEMPO(s)");
    printf("─────────────────────────────────────────────────────────────────\n");

    int posicao = 1;
    for (int i = 0; i < qtd; i++) {
        // Destacar o primeiro lugar
        if (posicao == 1) {
            printf("🏆 ");
        } else {
            printf("   ");
        }
        
        printf("%-3d %-25s %-12s %-10d %-10.2f\n",
               posicao,
               filtrado[i].nome_equipe,
               filtrado[i].tipo,
               filtrado[i].pontos,
               filtrado[i].tempo);
        posicao++;
    }

    printf("─────────────────────────────────────────────────────────────────\n");
    printf("Total de equipes ranqueadas: %d\n", qtd);
    
    // Mostrar estatísticas
    if (qtd > 0) {
        int max_pontos = filtrado[0].pontos;
        float melhor_tempo = filtrado[0].tempo;
        printf("Maior pontuação: %d pontos\n", max_pontos);
        printf("Melhor tempo: %.2f segundos\n", melhor_tempo);
    }
}

// ------------------------------------------------------------
// Mostra ranking geral combinando todos os desafios
// ------------------------------------------------------------
void mostrar_ranking_geral() {
    RegistroRanking lista[200];
    int total = carregar_resultados(lista, 200);

    if (total == 0) {
        printf("\nNenhum resultado registrado.\n");
        return;
    }

    // Agrupar por equipe somando pontos
    typedef struct {
        char nome_equipe[100];
        int total_pontos;
        float melhor_tempo;
        int participacoes;
    } EquipeRanking;

    EquipeRanking equipes[100];
    int num_equipes = 0;

    for (int i = 0; i < total; i++) {
        int encontrada = 0;
        for (int j = 0; j < num_equipes; j++) {
            if (strcmp(equipes[j].nome_equipe, lista[i].nome_equipe) == 0) {
                equipes[j].total_pontos += lista[i].pontos;
                if (lista[i].tempo < equipes[j].melhor_tempo || equipes[j].melhor_tempo == 0) {
                    equipes[j].melhor_tempo = lista[i].tempo;
                }
                equipes[j].participacoes++;
                encontrada = 1;
                break;
            }
        }
        
        if (!encontrada && num_equipes < 100) {
            strcpy(equipes[num_equipes].nome_equipe, lista[i].nome_equipe);
            equipes[num_equipes].total_pontos = lista[i].pontos;
            equipes[num_equipes].melhor_tempo = lista[i].tempo;
            equipes[num_equipes].participacoes = 1;
            num_equipes++;
        }
    }

    // Ordenar equipes por pontuação total
    for (int i = 0; i < num_equipes - 1; i++) {
        for (int j = i + 1; j < num_equipes; j++) {
            if (equipes[j].total_pontos > equipes[i].total_pontos) {
                EquipeRanking temp = equipes[i];
                equipes[i] = equipes[j];
                equipes[j] = temp;
            }
        }
    }

    printf("\n=== RANKING GERAL - TODOS OS DESAFIOS ===\n");
    printf("─────────────────────────────────────────────────────────────────\n");
    printf("%-5s %-25s %-10s %-12s %-10s\n",
           "POS", "EQUIPE", "PONTOS", "MELHOR TEMPO", "DESAFIOS");
    printf("─────────────────────────────────────────────────────────────────\n");

    for (int i = 0; i < num_equipes; i++) {
        // Destacar os três primeiros lugares
        if (i == 0) printf("🥇 ");
        else if (i == 1) printf("🥈 ");
        else if (i == 2) printf("🥉 ");
        else printf("   ");
        
        printf("%-3d %-25s %-10d %-12.2f %-10d\n",
               i + 1,
               equipes[i].nome_equipe,
               equipes[i].total_pontos,
               equipes[i].melhor_tempo,
               equipes[i].participacoes);
    }

    printf("─────────────────────────────────────────────────────────────────\n");
    printf("Total de equipes no ranking: %d\n", num_equipes);
    
    if (num_equipes > 0) {
        printf("Maior pontuação total: %d pontos\n", equipes[0].total_pontos);
        printf("Equipe com mais participações: %s (%d desafios)\n", 
               equipes[0].nome_equipe, equipes[0].participacoes);
    }
}

// ------------------------------------------------------------
// Menu de ranking interativo (usado por admin, avaliador e equipe)
// ------------------------------------------------------------
void menu_ranking() {
    printf("\n===== MENU RANKING =====\n");
    printf("1 - Ver Ranking Robô Sumô\n");
    printf("2 - Ver Ranking Robô Seguidor de Linha\n");
    printf("3 - Ver Ranking Geral (Todos os Desafios)\n");
    printf("0 - Voltar\n");
    printf("Escolha: ");
    
    int opc;
    scanf("%d", &opc);
    getchar();

    switch (opc) {
        case 1:
            mostrar_ranking(SUMO);
            pausar_sistema();
            break;
        case 2:
            mostrar_ranking(SEGUIDOR_LINHA);
            pausar_sistema();
            break;
        case 3:
            mostrar_ranking_geral();
            pausar_sistema();
            break;
        case 0:
            printf("Voltando ao menu anterior...\n");
            return;
        default:
            printf("Opção inválida.\n");
            pausar_sistema();
            break;
    }

    return menu_ranking();
}