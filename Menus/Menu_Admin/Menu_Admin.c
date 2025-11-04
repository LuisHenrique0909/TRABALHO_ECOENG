#include "Menu_Admin.h"
#include "Files.h"
#include "Cadastro_Equipes.h"
#include "Ranking.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ================================================================
// Menu do administrador
// ================================================================
void menu_admin() {
    int opc;
    do {
        printf("\n===== MENU ADMINISTRADOR =====\n");
        printf("1 - Listar usuários\n");
        printf("2 - Deletar usuário\n");
        printf("3 - Listar equipes\n");
        printf("4 - Deletar equipe\n");
        printf("5 - Ver ranking geral\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opc);
        getchar();

        switch (opc) {
            case 1: listar_todos_usuarios(); break;
            case 2: deletar_usuario(); break;
            case 3: listar_todas_equipes(); break;
            case 4: deletar_equipe(); break;
            case 5: menu_ranking(); break;
            case 0: printf("Saindo do menu administrador...\n"); break;
            default: printf("Opção inválida.\n");
        }
    } while (opc != 0);
}

// ================================================================
// Lista todos os usuários registrados no sistema
// ================================================================
void listar_todos_usuarios() {
    FILE *f = abrir_csv("users.csv");
    if (!f) {
        printf("Nenhum usuário cadastrado.\n");
        return;
    }

    char linha[256];
    fgets(linha, sizeof(linha), f);
    printf("\n--- LISTA DE USUÁRIOS ---\n");
    printf("%-5s %-25s %-10s\n", "ID", "Nome", "Cargo");

    int id, cargo;
    char nome[50], senha[50];
    while (fgets(linha, sizeof(linha), f)) {
        sscanf(linha, "%d,%49[^,],%d,%49[^\n]", &id, nome, &cargo, senha);
        printf("%-5d %-25s %-10s\n", id, nome,
               cargo == 0 ? "Admin" : cargo == 1 ? "Participante" : "Avaliador");
    }
    fclose(f);
}

// ================================================================
// Deleta usuário pelo ID informado
// ================================================================
void deletar_usuario() {
    listar_todos_usuarios();

    int id_remover;
    printf("\nDigite o ID do usuário a ser deletado: ");
    scanf("%d", &id_remover);
    getchar();

    FILE *f = abrir_csv("users.csv");
    if (!f) return;

    FILE *tmp = fopen("./dados/users_temp.csv", "w");
    fprintf(tmp, "ID,NOME,CARGO,SENHA\n");

    char linha[256];
    fgets(linha, sizeof(linha), f);
    int id, cargo;
    char nome[50], senha[50];

    while (fgets(linha, sizeof(linha), f)) {
        sscanf(linha, "%d,%49[^,],%d,%49[^\n]", &id, nome, &cargo, senha);
        if (id != id_remover)
            fprintf(tmp, "%d,%s,%d,%s\n", id, nome, cargo, senha);
    }

    fclose(f);
    fclose(tmp);

    remove("./dados/users.csv");
    rename("./dados/users_temp.csv", "./dados/users.csv");

    printf("Usuário removido com sucesso!\n");
}

// ================================================================
// Deleta equipe pelo ID informado
// ================================================================
void deletar_equipe() {
    listar_todas_equipes();

    int id_remover;
    printf("\nDigite o ID da equipe a ser deletada: ");
    scanf("%d", &id_remover);
    getchar();

    FILE *f = abrir_csv("equipes.csv");
    if (!f) return;

    FILE *tmp = fopen("./dados/equipes_temp.csv", "w");
    fprintf(tmp, "ID_EQUIPE,NOME_EQUIPE,CRIADOR,PARTICIPANTES\n");

    char linha[512];
    fgets(linha, sizeof(linha), f);
    int id;
    char nome[100], criador[50], participantes[300];

    while (fgets(linha, sizeof(linha), f)) {
        sscanf(linha, "%d,%99[^,],%49[^,],%299[^\n]", &id, nome, criador, participantes);
        if (id != id_remover)
            fprintf(tmp, "%d,%s,%s,%s\n", id, nome, criador, participantes);
    }

    fclose(f);
    fclose(tmp);

    remove("./dados/equipes.csv");
    rename("./dados/equipes_temp.csv", "./dados/equipes.csv");

    printf("Equipe removida com sucesso!\n");
}