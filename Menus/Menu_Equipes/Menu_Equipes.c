#include "Menu_Equipes.h"
#include "Files.h"
#include "pausar_sistema.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ================================================================
// Menu principal do participante
// ================================================================
void menu_equipe(User *usuario) {
    if (!usuario) {
        printf("Erro: usuário não logado.\n");
        return;
    }

    printf("\n===== MENU EQUIPE =====\n");
    printf("1 - Criar nova equipe\n");
    printf("2 - Visualizar minha equipe e integrantes\n");
    printf("3 - Adicionar participante à minha equipe (somente líder)\n");
    printf("4 - Remover participante da minha equipe (somente líder)\n");
    printf("5 - Ver pontuação da minha equipe\n");
    printf("6 - Ver ranking geral\n");
    printf("0 - Sair\n");
    printf("Escolha: ");
    
    int opc;
    scanf("%d", &opc);
    getchar();

    switch (opc) {
        case 1:
            cadastrar_equipe(usuario);
            pausar_sistema();
            break;
        case 2:
            exibir_equipe_do_participante(usuario);
            pausar_sistema();
            break;
        case 3: {
            char nome_equipe[100], nome_participante[100];
            int ra_participante;
            
            printf("Nome da equipe: ");
            fgets(nome_equipe, sizeof(nome_equipe), stdin);
            nome_equipe[strcspn(nome_equipe, "\n")] = '\0';

            printf("Nome do participante para adicionar: ");
            fgets(nome_participante, sizeof(nome_participante), stdin);
            nome_participante[strcspn(nome_participante, "\n")] = '\0';

            printf("RA do participante: ");
            scanf("%d", &ra_participante);
            getchar();

            // Criar objeto User temporário para o participante
            User participante;
            strcpy(participante.nome, nome_participante);
            participante.RA = ra_participante;

            Result r = adicionar_participante_equipe(nome_equipe, &participante);
            if (r.code == OK)
                printf("Participante adicionado com sucesso!\n");
            else
                print_err(&r);
            pausar_sistema();
            break;
        }
        case 4: {
            char nome_equipe[100], nome_participante[100];
            int ra_participante;
            
            printf("Nome da equipe: ");
            fgets(nome_equipe, sizeof(nome_equipe), stdin);
            nome_equipe[strcspn(nome_equipe, "\n")] = '\0';

            printf("Nome do participante para remover: ");
            fgets(nome_participante, sizeof(nome_participante), stdin);
            nome_participante[strcspn(nome_participante, "\n")] = '\0';

            printf("RA do participante: ");
            scanf("%d", &ra_participante);
            getchar();

            // Criar objeto User temporário para o participante
            User participante;
            strcpy(participante.nome, nome_participante);
            participante.RA = ra_participante;

            Result r = remover_participante_equipe(nome_equipe, &participante);
            if (r.code == OK)
                printf("Participante removido com sucesso!\n");
            else
                print_err(&r);
            pausar_sistema();
            break;
        }
        case 5:
            mostrar_pontuacao_da_equipe(usuario);
            break;
        case 6:
            menu_ranking();
            break;
        case 0:
            printf("Saindo do menu de equipe...\n");
            return;
        default:
            printf("Opção inválida.\n");
            pausar_sistema();
            break;
    }

    return menu_equipe(usuario);
}

// ================================================================
// Mostra a pontuação da equipe logada (dados do CSV de resultados)
// ================================================================
void mostrar_pontuacao_da_equipe(User *usuario) {
    FILE *f_eq = abrir_csv("equipes.csv");
    if (!f_eq) {
        printf("Nenhuma equipe cadastrada.\n");
        pausar_sistema();
        return;
    }

    char linha_eq[512];
    fgets(linha_eq, sizeof(linha_eq), f_eq);

    char nome_equipe[100] = "";
    int id_equipe = -1;

    char identificador_usuario[60];
    snprintf(identificador_usuario, sizeof(identificador_usuario), "%s:%d", usuario->nome, usuario->RA);

    while (fgets(linha_eq, sizeof(linha_eq), f_eq)) {
        int id;
        char nome_eq[100], criador[50], participantes[500];
        if (sscanf(linha_eq, "%d,%99[^,],%49[^,],%499[^\n]", &id, nome_eq, criador, participantes) == 4) {
            if (strstr(participantes, identificador_usuario)) {
                strcpy(nome_equipe, nome_eq);
                id_equipe = id;
                break;
            }
        }
    }
    fclose(f_eq);

    if (id_equipe == -1) {
        printf("\nVocê ainda não faz parte de nenhuma equipe.\n");
        pausar_sistema();
        return;
    }

    FILE *f_res = abrir_csv("resultados_robo.csv");
    if (!f_res) {
        printf("Nenhuma pontuação registrada ainda.\n");
        pausar_sistema();
        return;
    }

    char linha[512];
    fgets(linha, sizeof(linha), f_res);

    int id;
    char nome_csv[100], tipo[50];
    float tempo;
    int pontos;
    int encontrou = 0;
    int total_pontos = 0;

    printf("\n=== PONTUAÇÃO DA SUA EQUIPE (%s) ===\n", nome_equipe);
    printf("────────────────────────────────────────\n");
    
    while (fgets(linha, sizeof(linha), f_res)) {
        if (sscanf(linha, "%d,%99[^,],%49[^,],%f,%d", &id, nome_csv, tipo, &tempo, &pontos) == 5) {
            if (strcmp(nome_csv, nome_equipe) == 0) {
                printf("• Desafio: %-20s | Tempo: %6.2f s | Pontos: %3d\n", tipo, tempo, pontos);
                total_pontos += pontos;
                encontrou = 1;
            }
        }
    }

    if (encontrou) {
        printf("────────────────────────────────────────\n");
        printf("PONTUAÇÃO TOTAL: %d pontos\n", total_pontos);
    } else {
        printf("Esta equipe ainda não possui pontuação registrada.\n");
    }

    fclose(f_res);
    
    pausar_sistema();
}