#ifndef Equipes_H
#define Equipes_H

#include "../Cadastro_user/User.h"

// ==============================
// ESTRUTURA PRINCIPAL
// ==============================
typedef struct {
    int id;
    int id_Equipe;
    int id_PROFESSOR_RESPONSAVEL;
    char nome[50];
} Equipe;

// ==============================
// FUNÇoES DE MENU
// ==============================
void menu_equipes(User *usuario_logado);

// ==============================
// FUNÇoES DE CADASTRO DE EQUIPE
// ==============================
int cadastrar_equipe(User *usuario_logado);
int salvar_equipe( Equipe *e);
int equipe_ja_existe(const char *nome);

// ==============================
// FUNÇÕES DE ViNCULO PARTICIPANTE-EQUIPE
// ==============================
void vincular_participante(User *usuario_logado);
int coletar_vinculo_participante(int *id_equipe, int *id_user);
int salvar_vinculo_participante(int id_equipe, int id_user);
int participante_ja_vinculado(int id_user);

#endif
