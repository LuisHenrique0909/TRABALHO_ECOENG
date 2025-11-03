#ifndef PONTUACAO_H
#define PONTUACAO_H

#include <stdio.h>
#include "Result.h"

typedef struct {
    int id_equipe;
    char nome_equipe[100];
    char tipo_desafio[50]; // "Sumo" ou "Seguidor"
    float tempo;
    int pontos;
} Pontuacao;

// Registrar pontuação (modo rápido, administrativo)
Result registrar_pontuacao_para_equipe(int id_equipe, const char *nome_equipe, const char *tipo, float tempo);

#endif