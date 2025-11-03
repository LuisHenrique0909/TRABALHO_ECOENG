#ifndef MENU_EQUIPES_H
#define MENU_EQUIPES_H

#include "User.h"
#include "Cadastro_Equipes.h"
#include "Ranking.h"
#include "Files.h"

// Abre o menu principal da equipe (após login/cadastro)
void menu_equipe(User *usuario_logado);

// Mostra a pontuação da equipe do usuário logado
void mostrar_pontuacao_da_equipe(User *usuario_logado);

#endif