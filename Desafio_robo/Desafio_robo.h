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

// Funções principais
Result iniciar_desafio(TipoDesafio tipo);
Result registrar_resultado(Resultado_Desafio *res);
void gerar_chaveamento(TipoDesafio tipo);
void calcular_pontuacoes(TipoDesafio tipo);

// Funções para gerenciamento de desafios
Result registrar_vencedor_desafio(TipoDesafio tipo);
void exibir_chaveamento_desafio(TipoDesafio tipo);

#endif