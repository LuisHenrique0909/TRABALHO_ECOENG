#ifndef MENU_ADMIN_H
#define MENU_ADMIN_H

#include "User.h"
#include "Cadastro_Equipes.h"
#include "Pontuacao.h"
#include "Ranking.h"

// ========================== FUNÇÕES PRINCIPAIS ==========================

// Menu principal do administrador
void menu_admin();

// ========================== FUNÇÕES AUXILIARES ==========================

// Gerencia usuários (listar e deletar interativamente)
void gerenciar_usuarios();

// Gerencia equipes (listar e deletar interativamente)
void gerenciar_equipes();

// Permite alterar a pontuação de uma equipe manualmente
void alterar_pontuacao_equipe();

#endif