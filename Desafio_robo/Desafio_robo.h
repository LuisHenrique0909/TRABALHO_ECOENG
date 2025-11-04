#ifndef DESAFIO_ROBO_H
#define DESAFIO_ROBO_H

#include "Result.h"

typedef enum {
    SUMO,
    SEGUIDOR_LINHA
} TipoDesafio;

typedef struct {
    int id_equipe;
    char nome_equipe[100];
    TipoDesafio tipo;
    float tempo_execucao;
    int pontuacao;
} Resultado_Desafio;

// Inicializa um desafio (Sumô ou Seguidor)
Result iniciar_desafio(TipoDesafio tipo);

// Registra resultados e atualiza pontuação da equipe
Result registrar_resultado(Resultado_Desafio *res);

// Gera chaveamento com base nas equipes cadastradas
void gerar_chaveamento(TipoDesafio tipo);

// Exibe a pontuação das equipes em determinado desafio
void calcular_pontuacoes(TipoDesafio tipo);

#endif