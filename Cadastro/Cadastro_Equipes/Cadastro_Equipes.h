#ifndef CADASTRO_EQUIPES_H
#define CADASTRO_EQUIPES_H

#include "Result.h"
#include "User.h"

// Estrutura de equipe
typedef struct {
    int id_equipe;
    char nome_equipe[100];
    char nome_lider[100];
    char participantes[500]; // Formato: "Nome1:RA1;Nome2:RA2;..."
} Equipe;

// Funções principais
Result cadastrar_equipe(User *lider);
Result adicionar_participante_equipe(const char *nome_equipe, User *participante);
Result remover_participante_equipe(const char *nome_equipe, User *participante);
void listar_equipes();
void exibir_equipe_do_participante(User *usuario);

#endif