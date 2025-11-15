#ifndef CHAVEAMENTO_H
#define CHAVEAMENTO_H

#include "Desafio_robo.h"

typedef enum {
    AGUARDANDO,
    EM_ANDAMENTO,
    FINALIZADO
} StatusConfronto;

typedef struct {
    int id_confronto;
    int id_equipe1;
    int id_equipe2;
    int id_vencedor;
    float tempo_vencedor;
    int rodada;
    StatusConfronto status;
    char nome_equipe1[100];
    char nome_equipe2[100];
} Confronto;

typedef struct {
    int id_chaveamento;
    TipoDesafio tipo_desafio;
    Confronto confrontos[32]; // Suporte até 32 equipes
    int num_confrontos;
    int rodada_atual;
    int finalizado;
} Chaveamento;

// Funções principais
Result gerar_chaveamento_persistente(TipoDesafio tipo);
Result registrar_vencedor_confronto(int id_confronto, int id_vencedor, float tempo);
void exibir_chaveamento(TipoDesafio tipo);
Chaveamento* carregar_chaveamento_ativo(TipoDesafio tipo);
Result avancar_rodada(TipoDesafio tipo);  // Nova função para avançar rodada automaticamente

#endif