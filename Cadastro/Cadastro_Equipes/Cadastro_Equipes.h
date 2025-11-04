#ifndef CADASTRO_EQUIPES_H
#define CADASTRO_EQUIPES_H

#include "Result.h"
#include "User.h"

// Estrutura de equipe
typedef struct {
    int id_equipe;
    char nome_equipe[100];
    char nome_lider[100];
    char participantes[300]; // nomes dos integrantes separados por ';'
} Equipe;

// Funções principais
Result cadastrar_equipe(User *lider);
Result adicionar_participante_equipe(const char *nome_equipe, const char *nome_participante);
Result remover_participante_equipe(const char *nome_equipe, const char *nome_participante);
void listar_equipes();
void exibir_equipe_do_participante(User *usuario);
int equipe_existe(const char *nome_equipe);
int participante_em_alguma_equipe(const char *nome_participante);

#endif