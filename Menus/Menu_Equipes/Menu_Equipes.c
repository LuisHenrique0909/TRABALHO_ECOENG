#include "Menu_Equipes.h"
#include "Files.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Função auxiliar: mostra pontuação da equipe do usuário
void mostrar_pontuacao_da_equipe(User *usuario_logado) {
    FILE *f = abrir_csv("resultados_robo.csv");
    if (!f) {
        printf("Nenhuma pontuação registrada ainda.\n");
        return;
    }

    char linha[512];
    fgets(linha, sizeof(linha), f); // pula cabeçalho

    char nome_equipe[100];
    float tempo;
    int pontos, id;
    char tipo[50];

    int encontrou = 0;

    while (fgets(linha, sizeof(linha), f)) {
        sscanf(linha, "%d,%99[^,],%49[^,],%f,%d", &id, nome_equipe, tipo, &tempo, &pontos);
        if (strstr(nome_equipe, usuario_logado->nome)) {
            printf("\n--- Pontuação da sua equipe (%s) ---\n", nome_equipe);
            printf("Desafio: %s\nTempo: %.2f s\nPontos: %d\n", tipo, tempo, pontos);
            encontrou = 1;
        }
    }

    fclose(f);

    if (!encontrou)
        printf("\nSua equipe ainda não tem pontuação registrada.\n");
}

// Menu principal da equipe
void menu_equipe(User *usuario_logado) {
    if (!usuario_logado) {
        printf("Erro: usuário não logado.\n");
        return;
    }

    int opc;
    do {
        printf("\n===== MENU EQUIPE =====\n");
        printf("1 - Visualizar minha equipe e integrantes\n");
        printf("2 - Ver pontuação da minha equipe\n");
        printf("3 - Ver ranking geral de equipes\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opc);
        getchar();

        switch (opc) {
            case 1:
                exibir_equipe_do_participante(usuario_logado);
                break;
            case 2:
                mostrar_pontuacao_da_equipe(usuario_logado);
                break;
            case 3:
                menu_ranking();
                break;
            case 0:
                printf("Saindo do menu de equipe...\n");
                break;
            default:
                printf("Opção inválida.\n");
        }
    } while (opc != 0);
}