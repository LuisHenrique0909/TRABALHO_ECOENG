#ifndef CADASTRO_EQUIPES_H
#define CADASTRO_EQUIPES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../Cadastro/Cadastro_user/User.h"
#include "../../Utilidades/Result/Result.h"

#define MAX_EQUIPE_NOME 100
#define MAX_PARTICIPANTES 5

typedef struct {
    char nome[MAX_EQUIPE_NOME];
    int ra_criador;
    char nome_criador[50];
} Equipe;

// --- Funções principais ---
Result cadastrar_equipe(User *usuario_logado);
Result vincular_participante(User *usuario_logado);
void listar_equipes(User *usuario_logado);

// --- Funções auxiliares ---
int participante_ja_vinculado(int ra);
int criador_da_equipe(int ra);
Equipe* buscar_equipe_por_ra(int ra);

#endif
