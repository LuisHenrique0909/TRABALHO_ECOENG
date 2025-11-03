#include "Desafio_robo.h"
#include "Pontuacao.h"
#include "Files.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Result iniciar_desafio(TipoDesafio tipo) {
    printf("\n===== INICIANDO DESAFIO =====\n");
    if (tipo == SUMO)
        printf("Robô SUMÔ: confronto 1x1, vitória = 100 pontos.\n");
    else
        printf("Robô SEGUIDOR: melhor tempo = mais pontos.\n");
    return ok();
}

Result registrar_resultado(Resultado_Desafio *res) {
    if (!res) return erro(ERRO_INVALIDO, "Resultado inválido.");

    if (res->id_equipe <= 0) {
        printf("ID da equipe: ");
        scanf("%d", &res->id_equipe);
        getchar();
    }

    if (strlen(res->nome_equipe) == 0) {
        printf("Nome da equipe: ");
        fgets(res->nome_equipe, sizeof(res->nome_equipe), stdin);
        res->nome_equipe[strcspn(res->nome_equipe, "\n")] = '\0';
    }

    const char *tipo_str = (res->tipo == SUMO) ? "Sumo" : "Seguidor";

    if (res->tipo == SEGUIDOR_LINHA) {
        printf("Tempo de execução (segundos): ");
        scanf("%f", &res->tempo_execucao);
        getchar();
        res->pontuacao = (int)(1000 / res->tempo_execucao);
    } else {
        res->tempo_execucao = 0;
        res->pontuacao = 100;
    }

    // Centraliza o registro de pontuação
    registrar_pontuacao_para_equipe(res->id_equipe, res->nome_equipe, tipo_str, res->tempo_execucao);

    printf("Resultado registrado com sucesso!\n");
    return ok();
}

void gerar_chaveamento(TipoDesafio tipo) {
    FILE *f = abrir_csv("equipes.csv");
    if (!f) {
        printf("Nenhuma equipe cadastrada.\n");
        return;
    }

    char linha[512];
    fgets(linha, sizeof(linha), f);

    int id;
    char nome[100], criador[50], membros[200];
    printf("\n--- CHAVEAMENTO: %s ---\n", tipo == SUMO ? "SUMO" : "SEGUIDOR");

    while (fgets(linha, sizeof(linha), f)) {
        sscanf(linha, "%d,%99[^,],%49[^,],%199[^\n]", &id, nome, criador, membros);
        printf("ID %d | %s | Líder: %s | Integrantes: %s\n", id, nome, criador, membros);
    }

    fclose(f);
    printf("Chaveamento gerado!\n");
}

void calcular_pontuacoes(TipoDesafio tipo) {
    FILE *f = abrir_csv("resultados_robo.csv");
    if (!f) {
        printf("Nenhum resultado encontrado.\n");
        return;
    }

    char linha[512];
    fgets(linha, sizeof(linha), f);

    char tipo_str[20];
    strcpy(tipo_str, tipo == SUMO ? "Sumo" : "Seguidor");

    int id, pontos;
    float tempo;
    char nome[100], tipo_csv[50];

    printf("\n--- RESULTADOS (%s) ---\n", tipo_str);
    while (fgets(linha, sizeof(linha), f)) {
        sscanf(linha, "%d,%99[^,],%49[^,],%f,%d", &id, nome, tipo_csv, &tempo, &pontos);
        if (strcmp(tipo_csv, tipo_str) == 0)
            printf("Equipe: %-25s | Pontos: %3d | Tempo: %.2fs\n", nome, pontos, tempo);
    }

    fclose(f);
}