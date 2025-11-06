#include "Menu_Admin.h"
#include "Files.h"
#include "Pontuacao.h"
#include "Ranking.h"
#include "Cadastro_Equipes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Função auxiliar para pausa padronizada
void pausar_sistema() {
    printf("\nPressione ENTER para continuar...");
    getchar();
}

// ============================================================================
// Função principal do menu do administrador
// ============================================================================
void menu_admin() {
    printf("\n===== MENU ADMINISTRADOR =====\n");
    printf("1 - Listar todos os usuários\n");
    printf("2 - Deletar usuário\n");
    printf("3 - Listar todas as equipes\n");
    printf("4 - Deletar equipe\n");
    printf("5 - Alterar pontuação de equipe\n");
    printf("6 - Visualizar ranking geral\n");
    printf("0 - Sair\n");
    printf("Escolha: ");
    
    int opc;
    scanf("%d", &opc);
    getchar();

    switch (opc) {
        case 1: 
            listar_todos_usuarios(); 
            pausar_sistema();
            break;
        case 2: 
            deletar_usuario(); 
            pausar_sistema();
            break;
        case 3: 
            listar_equipes();
            pausar_sistema();
            break;
        case 4: 
            deletar_equipe(); 
            pausar_sistema();
            break;
        case 5: 
            alterar_pontuacao_equipe(); 
            pausar_sistema();
            break;
        case 6: 
            menu_ranking(); 
            break;
        case 0: 
            printf("Saindo do menu do administrador...\n");
            return;
        default: 
            printf("Opção inválida.\n");
            pausar_sistema();
            break;
    }

    return menu_admin();
}

// ============================================================================
// 1. Listar todos os usuários
// ============================================================================
void listar_todos_usuarios() {
    FILE *f = abrir_csv("users.csv");
    if (!f) {
        printf("Nenhum usuário encontrado.\n");
        return;
    }

    char linha[256];
    fgets(linha, sizeof(linha), f);

    printf("\n=== USUÁRIOS CADASTRADOS ===\n");
    printf("────────────────────────────────────────\n");
    printf("%-5s %-25s %-15s\n", "ID", "Nome", "Cargo");
    printf("────────────────────────────────────────\n");

    int id, cargo;
    char nome[50], senha[50];
    int count = 0;

    while (fgets(linha, sizeof(linha), f)) {
        if (sscanf(linha, "%d,%49[^,],%d,%49[^\n]", &id, nome, &cargo, senha) == 4) {
            printf("%-5d %-25s %-15s\n", id, nome, cargo_pra_texto(cargo));
            count++;
        }
    }

    printf("────────────────────────────────────────\n");
    printf("Total de usuários: %d\n", count);
    fclose(f);
}

// ============================================================================
// 2. Deletar usuário por ID
// ============================================================================
void deletar_usuario() {
    listar_todos_usuarios();
    printf("\nDigite o ID do usuário que deseja deletar: ");
    int id_alvo;
    scanf("%d", &id_alvo);
    getchar();

    FILE *f = abrir_csv("users.csv");
    if (!f) {
        printf("Erro ao abrir arquivo de usuários.\n");
        return;
    }

    FILE *temp = fopen("./dados/temp_users.csv", "w");
    fprintf(temp, "ID,NOME,CARGO,SENHA\n");

    char linha[256];
    int id, cargo;
    char nome[50], senha[50];
    int encontrado = 0;

    fgets(linha, sizeof(linha), f);
    while (fgets(linha, sizeof(linha), f)) {
        sscanf(linha, "%d,%49[^,],%d,%49[^\n]", &id, nome, &cargo, senha);
        if (id == id_alvo) {
            encontrado = 1;
            printf("Usuário '%s' (ID: %d) será removido.\n", nome, id);
            continue;
        }
        fprintf(temp, "%d,%s,%d,%s\n", id, nome, cargo, senha);
    }

    fclose(f);
    fclose(temp);
    remove("./dados/users.csv");
    rename("./dados/temp_users.csv", "./dados/users.csv");

    if (encontrado)
        printf("Usuário ID %d deletado com sucesso.\n", id_alvo);
    else
        printf("Usuário não encontrado.\n");
}

// ============================================================================
// 4. Deletar equipe
// ============================================================================
void deletar_equipe() {
    listar_equipes();
    
    printf("\nDigite o ID da equipe que deseja deletar: ");
    int id_alvo;
    scanf("%d", &id_alvo);
    getchar();

    FILE *f = abrir_csv("equipes.csv");
    if (!f) {
        printf("Erro ao abrir arquivo de equipes.\n");
        return;
    }

    FILE *temp = fopen("./dados/temp_equipes.csv", "w");
    fprintf(temp, "ID_EQUIPE,NOME_EQUIPE,LIDER,PARTICIPANTES\n");

    char linha[512];
    int id;
    char nome[100], lider[50], participantes[500];
    int encontrado = 0;

    fgets(linha, sizeof(linha), f);
    while (fgets(linha, sizeof(linha), f)) {
        sscanf(linha, "%d,%99[^,],%49[^,],%499[^\n]", &id, nome, lider, participantes);
        if (id == id_alvo) {
            encontrado = 1;
            printf("Equipe '%s' (ID: %d) será removida.\n", nome, id);
            continue;
        }
        fprintf(temp, "%d,%s,%s,%s\n", id, nome, lider, participantes);
    }

    fclose(f);
    fclose(temp);
    remove("./dados/equipes.csv");
    rename("./dados/temp_equipes.csv", "./dados/equipes.csv");

    if (encontrado)
        printf("Equipe ID %d deletada com sucesso.\n", id_alvo);
    else
        printf("Equipe não encontrada.\n");
}

// ============================================================================
// 5. Alterar pontuação de equipe
// ============================================================================
void alterar_pontuacao_equipe() {
    listar_equipes();

    printf("\nDigite o nome exato da equipe que deseja alterar a pontuação: ");
    char nome_equipe[100];
    fgets(nome_equipe, sizeof(nome_equipe), stdin);
    nome_equipe[strcspn(nome_equipe, "\n")] = '\0';

    Pontuacao p;
    if (!buscar_pontuacao_por_equipe(nome_equipe, &p)) {
        printf("Equipe '%s' não encontrada no sistema.\n", nome_equipe);
        return;
    }

    printf("Pontuação atual: %d\n", p.pontos);
    printf("Nova pontuação: ");
    int nova_pontuacao;
    scanf("%d", &nova_pontuacao);
    getchar();

    Result r = atualizar_pontuacao(nome_equipe, nova_pontuacao);
    if (r.code == OK)
        printf("Pontuação da equipe '%s' atualizada com sucesso!\n", nome_equipe);
    else
        print_err(&r);
}