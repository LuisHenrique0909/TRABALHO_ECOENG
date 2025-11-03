#include "Menu_Equipes.h"
#include "Ranking.h"
#include "cadastro_equipes.h"
#include "Files.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ==================== MENU PRINCIPAL DA EQUIPE ====================
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

// ==================== MOSTRAR PONTUAÇÃO DA EQUIPE ====================
void mostrar_pontuacao_da_equipe(User *usuario_logado) {
    if (!usuario_logado) {
        printf("Erro: usuário não logado.\n");
        return;
    }

    // Descobre em qual equipe o participante está
    FILE *feq = abrir_csv("equipes.csv");
    if (!feq) {
        printf("Nenhuma equipe cadastrada.\n");
        return;
    }

    char linha[512], nome_equipe_usuario[100] = "";
    fgets(linha, sizeof(linha), feq); // pula cabeçalho

    while (fgets(linha, sizeof(linha), feq)) {
        int id;
        char nome_eq[100], criador[50], nomes[200];
        if (sscanf(linha, "%d,%99[^,],%49[^,],%199[^\n]", &id, nome_eq, criador, nomes) == 4) {
            if (strstr(nomes, usuario_logado->nome)) {
                strcpy(nome_equipe_usuario, nome_eq);
                break;
            }
        }
    }
    fclose(feq);

    if (strlen(nome_equipe_usuario) == 0) {
        printf("Você ainda não está vinculado a nenhuma equipe.\n");
        return;
    }

    // Busca pontuação da equipe em resultados_robo.csv
    FILE *fres = abrir_csv("resultados_robo.csv");
    if (!fres) {
        printf("Nenhuma pontuação registrada ainda.\n");
        return;
    }

    fgets(linha, sizeof(linha), fres); // pula cabeçalho

    int id;
    char nome_eq[100], tipo[50];
    float tempo;
    int pontos;
    int encontrou = 0;

    printf("\n--- Pontuações da equipe '%s' ---\n", nome_equipe_usuario);
    while (fgets(linha, sizeof(linha), fres)) {
        if (sscanf(linha, "%d,%99[^,],%49[^,],%f,%d", &id, nome_eq, tipo, &tempo, &pontos) == 5) {
            if (strcmp(nome_eq, nome_equipe_usuario) == 0) {
                printf("Desafio: %-10s | Tempo: %-8.2fs | Pontos: %d\n", tipo, tempo, pontos);
                encontrou = 1;
            }
        }
    }

    fclose(fres);

    if (!encontrou)
        printf("Ainda não há pontuação registrada para sua equipe.\n");
}