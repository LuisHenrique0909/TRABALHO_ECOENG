#ifndef MENU_EQUIPES_H
#define MENU_EQUIPES_H

#include "User.h"
#include "Cadastro_Equipes.h"
#include "Ranking.h"

// Menu principal para o participante logado
void menu_equipe(User *usuario);

// Mostra a pontuação da equipe logada
void mostrar_pontuacao_da_equipe(User *usuario);

#endif