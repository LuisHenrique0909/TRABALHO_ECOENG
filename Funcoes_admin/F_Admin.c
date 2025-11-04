#include "Files.h"
#include "Ranking.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ========================= AUXILIARES =========================

// Gerencia usuários: lista e permite deletar por ID
void gerenciar_usuarios() {
    FILE *f = abrir_csv("users.csv");
    if (!f) {
        printf("Nenhum usuário encontrado.\n");
        return;
    }

    char linha[256];
    fgets(linha, sizeof(linha), f); // pula cabeçalho

    printf("\n--- USUÁRIOS CADASTRADOS ---\n");
    int id, cargo;
    char nome[50], senha[50];
    int count = 0;

    while (fgets(linha, sizeof(linha), f)) {
        if (sscanf(linha, "%d,%49[^,],%d,%49[^\n]", &id, nome, &cargo, senha) == 4) {
            printf("ID: %-3d | %-20s | %-12s\n", id, nome, cargo_pra_texto(cargo));
            count++;
        }
    }
    fclose(f);

    if (count == 0) {
        printf("Nenhum usuário para exibir.\n");
        return;
    }

    printf("\nDigite o ID do usuário que deseja deletar (ou 0 para cancelar): ");
    int id_del;
    scanf("%d", &id_del);
    getchar();

    if (id_del == 0) {
        printf("Operação cancelada.\n");
        return;
    }

    FILE *orig = abrir_csv("users.csv");
    FILE *temp = fopen("temp.csv", "w");
    if (!orig || !temp) {
        printf("Erro ao manipular arquivos.\n");
        if (orig) fclose(orig);
        if (temp) fclose(temp);
        return;
    }

    fgets(linha, sizeof(linha), orig);
    fprintf(temp, "%s", linha);

    int id_csv;
    int encontrado = 0;
    while (fgets(linha, sizeof(linha), orig)) {
        sscanf(linha, "%d,", &id_csv);
        if (id_csv != id_del)
            fprintf(temp, "%s", linha);
        else
            encontrado = 1;
    }

    fclose(orig);
    fclose(temp);
    remove("users.csv");
    rename("temp.csv", "users.csv");

    if (encontrado)
        printf("Usuário com ID %d removido com sucesso!\n", id_del);
    else
        printf("Usuário com ID %d não encontrado.\n", id_del);
}

// Gerencia equipes: lista e permite deletar por ID
void gerenciar_equipes() {
    FILE *f = abrir_csv("equipes.csv");
    if (!f) {
        printf("Nenhuma equipe cadastrada.\n");
        return;
    }

    char linha[512];
    fgets(linha, sizeof(linha), f);

    printf("\n--- EQUIPES CADASTRADAS ---\n");
    int id;
    char nome_eq[100], nome_criador[50], nomes_part[200];
    int count = 0;

    while (fgets(linha, sizeof(linha), f)) {
        if (sscanf(linha, "%d,%99[^,],%49[^,],%199[^\n]", &id, nome_eq, nome_criador, nomes_part) == 4) {
            printf("ID: %-3d | Equipe: %-20s | Líder: %-15s\n", id, nome_eq, nome_criador);
            count++;
        }
    }
    fclose(f);

    if (count == 0) {
        printf("Nenhuma equipe encontrada.\n");
        return;
    }

    printf("\nDigite o ID da equipe que deseja deletar (ou 0 para cancelar): ");
    int id_del;
    scanf("%d", &id_del);
    getchar();

    if (id_del == 0) {
        printf("Operação cancelada.\n");
        return;
    }

    FILE *orig = abrir_csv("equipes.csv");
    FILE *temp = fopen("temp.csv", "w");
    if (!orig || !temp) {
        printf("Erro ao manipular arquivos.\n");
        if (orig) fclose(orig);
        if (temp) fclose(temp);
        return;
    }

    fgets(linha, sizeof(linha), orig);
    fprintf(temp, "%s", linha);

    int id_csv, encontrado = 0;
    while (fgets(linha, sizeof(linha), orig)) {
        sscanf(linha, "%d,", &id_csv);
        if (id_csv != id_del)
            fprintf(temp, "%s", linha);
        else
            encontrado = 1;
    }

    fclose(orig);
    fclose(temp);
    remove("equipes.csv");
    rename("temp.csv", "equipes.csv");

    if (encontrado)
        printf("Equipe com ID %d removida com sucesso!\n", id_del);
    else
        printf("Equipe com ID %d não encontrada.\n", id_del);
}

// Alterar pontuação de equipe
void alterar_pontuacao_equipe() {
    FILE *f = abrir_csv("resultados_robo.csv");
    if (!f) {
        printf("Nenhum resultado registrado.\n");
        return;
    }

    char linha[512];
    fgets(linha, sizeof(linha), f);
    printf("\n--- PONTUAÇÕES ATUAIS ---\n");

    int id, pontos;
    float tempo;
    char nome_eq[100], tipo[50];

    while (fgets(linha, sizeof(linha), f)) {
        if (sscanf(linha, "%d,%99[^,],%49[^,],%f,%d", &id, nome_eq, tipo, &tempo, &pontos) == 5)
            printf("ID: %-3d | Equipe: %-20s | %s | Pontos: %-5d | Tempo: %.2fs\n", id, nome_eq, tipo, pontos, tempo);
    }
    fclose(f);

    printf("\nDigite o ID da equipe para alterar (ou 0 para cancelar): ");
    int id_alt;
    scanf("%d", &id_alt);
    getchar();

    if (id_alt == 0) {
        printf("Operação cancelada.\n");
        return;
    }

    FILE *orig = abrir_csv("resultados_robo.csv");
    FILE *temp = fopen("temp.csv", "w");
    if (!orig || !temp) {
        printf("Erro ao manipular arquivos.\n");
        if (orig) fclose(orig);
        if (temp) fclose(temp);
        return;
    }

    float novo_tempo;
    int nova_pont;
    printf("Novo tempo (0 = manter): ");
    scanf("%f", &novo_tempo);
    printf("Nova pontuação: ");
    scanf("%d", &nova_pont);
    getchar();

    fgets(linha, sizeof(linha), orig);
    fprintf(temp, "%s", linha);

    int id_csv;
    int pontos_csv;
    float tempo_csv;
    int encontrado = 0;

    while (fgets(linha, sizeof(linha), orig)) {
        sscanf(linha, "%d,%99[^,],%49[^,],%f,%d", &id_csv, nome_eq, tipo, &tempo_csv, &pontos_csv);
        if (id_csv == id_alt) {
            if (novo_tempo != 0) tempo_csv = novo_tempo;
            pontos_csv = nova_pont;
            encontrado = 1;
        }
        fprintf(temp, "%d,%s,%s,%.2f,%d\n", id_csv, nome_eq, tipo, tempo_csv, pontos_csv);
    }

    fclose(orig);
    fclose(temp);
    remove("resultados_robo.csv");
    rename("temp.csv", "resultados_robo.csv");

    if (encontrado)
        printf("Pontuação atualizada com sucesso!\n");
    else
        printf("Equipe não encontrada.\n");
}

// ========================= MENU ADMIN =========================

void menu_admin() {
    int opc;
    do {
        printf("\n===== MENU ADMINISTRADOR =====\n");
        printf("1 - Gerenciar usuários\n");
        printf("2 - Gerenciar equipes\n");
        printf("3 - Alterar pontuação de equipe\n");
        printf("4 - Visualizar ranking geral\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opc);
        getchar();

        switch (opc) {
            case 1: gerenciar_usuarios(); break;
            case 2: gerenciar_equipes(); break;
            case 3: alterar_pontuacao_equipe(); break;
            case 4: menu_ranking(); break;
            case 0: printf("Saindo do menu administrador...\n"); break;
            default: printf("Opção inválida.\n");
        }
    } while (opc != 0);
}
