#ifndef PONTUACAO_H
#define PONTUACAO_H

#include "Result.h"

// Definição da estrutura Pontuacao (para busca)
typedef struct {
    char nome_equipe[100];
    int pontos;
} Pontuacao;

// Declarações das funções
void listar_pontuacoes();
int buscar_pontuacao_por_equipe(const char* nome_equipe, Pontuacao* pontuacao);
Result atualizar_pontuacao(const char* nome_equipe, int nova_pontuacao);

// MANTENHA a assinatura original que já está sendo usada
void registrar_pontuacao_para_equipe(int id, const char *nome, const char *tipo, float tempo, int pontos);

#endif