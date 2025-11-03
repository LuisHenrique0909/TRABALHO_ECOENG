#ifndef CADASTRO_EQUIPES_H
#define CADASTRO_EQUIPES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "User.h"
#include "Result.h"

typedef struct {
    int id_equipe;
    char nome[100];
    char nome_criador[50];
    char nomes_participantes[200];
} Equipe;

Result cadastrar_equipe(User *usuario_logado);
Result adicionar_participante(User *usuario_logado);
Result remover_participante(User *usuario_logado);
void exibir_equipe_do_participante(User *usuario_logado);
void listar_equipes();

#endif