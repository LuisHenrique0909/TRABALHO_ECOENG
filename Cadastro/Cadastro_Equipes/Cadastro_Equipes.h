#ifndef CADASTRO_EQUIPES_H
#define CADASTRO_EQUIPES_H

#include "Result.h"
#include "User.h"

// Estrutura principal da equipe
typedef struct {
    int id;
    char nome[100];
    char criador[50];
    char participantes[300];
} Equipe;

// Funções principais
Result cadastrar_equipe(User *lider);
void listar_equipes();
void listar_todas_equipes(); // para o admin
void exibir_equipe_do_participante(User *usuario);
void adicionar_participante(User *lider);
void remover_participante(User *lider);
int participante_ja_tem_equipe(const char *nome_participante);
int buscar_equipe_por_nome(const char *nome);
int ultimo_id_equipe();

#endif