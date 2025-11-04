#include "Desafio_robo.h"
#include "Pontuacao.h"
#include "Files.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ------------------------------------------------------------
// Inicializa um desafio (só mostra as regras básicas)
// ------------------------------------------------------------
Result iniciar_desafio(TipoDesafio tipo) {
    printf("\n===== INICIANDO DESAFIO =====\n");
    if (tipo == SUMO)
        printf("Robô SUMÔ: confronto direto. Vitória = 100 pontos.\n");
    else
        printf("Robô SEGUIDOR DE LINHA: menor tempo = maior pontuação.\n");

    return ok();
}

// ------------------------------------------------------------
// Registra um resultado de desafio
// ------------------------------------------------------------
Result registrar_resultado(Resultado_Desafio *res) {
    if (!res) return erro(ERRO_INVALIDO, "Resultado inválido");

    // ID e nome da equipe
    if (res->id_equipe <= 0) {
        printf("Digite o ID da equipe: ");
        scanf("%d", &res->id_equipe);
        getchar();
    }

    if (strlen(res->nome_equipe) == 0) {
        printf("Digite o nome da equipe: ");
        fgets(res->nome_equipe, sizeof(res->nome_equipe), stdin);
        res->nome_equipe[strcspn(res->nome_equipe, "\n")] = '\0';
    }

    const char *tipo_str = (res->tipo == SUMO) ? "Sumo" : "Seguidor";

    // Leitura e cálculo de pontuação
    if (res->tipo == SEGUIDOR_LINHA) {
        printf("Tempo de execução (segundos): ");
        scanf("%f", &res->tempo_execucao);
        getchar();
        res->pontuacao = (int)(1000 / res->tempo_execucao);
    } else {
        res->tempo_execucao = 0.0f;
        res->pontuacao = 100;
    }

    // Registra persistente na tabela de resultados
    registrar_pontuacao_para_equipe(res->id_equipe, res->nome_equipe, tipo_str, res->tempo_execucao, res->pontuacao);

    printf("Resultado registrado com sucesso!\n");
    return ok();
}

// ------------------------------------------------------------
// Mostra as equipes que participarão (chaveamento)
// ------------------------------------------------------------
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

    printf("\n--- CHAVEAMENTO: %s ---\n", tipo == SUMO ? "SUMO" : "SEGUIDOR DE LINHA");

    while (fgets(linha, sizeof(linha), f)) {
        sscanf(linha, "%d,%99[^,],%49[^,],%199[^\n]", &id, nome, criador, membros);
        printf("ID: %-3d | Equipe: %-20s | Líder: %-15s | Integrantes: %s\n",
               id, nome, criador, membros);
    }

    fclose(f);
    printf("Chaveamento gerado com sucesso!\n");
}

// ------------------------------------------------------------
// Mostra as pontuações registradas para um tipo de desafio
// ------------------------------------------------------------
void calcular_pontuacoes(TipoDesafio tipo) {
    FILE *f = abrir_csv("resultados_robo.csv");
    if (!f) {
        printf("Nenhum resultado registrado ainda.\n");
        return;
    }

    char linha[512];
    fgets(linha, sizeof(linha), f);

    int id, pontos;
    float tempo;
    char nome[100], tipo_csv[50];

    printf("\n--- PONTUAÇÕES (%s) ---\n", tipo == SUMO ? "SUMO" : "SEGUIDOR DE LINHA");

    while (fgets(linha, sizeof(linha), f)) {
        sscanf(linha, "%d,%99[^,],%49[^,],%f,%d", &id, nome, tipo_csv, &tempo, &pontos);
        if ((tipo == SUMO && strcmp(tipo_csv, "Sumo") == 0) ||
            (tipo == SEGUIDOR_LINHA && strstr(tipo_csv, "Seguidor"))) {
            printf("ID %d | %-25s | Pontos: %4d | Tempo: %.2fs\n",
                   id, nome, pontos, tempo);
        }
    }

    fclose(f);
    printf("Cálculo de pontuações concluído!\n");
}