#ifndef MENU_ADMIN_H
#define MENU_ADMIN_H

#include "User.h"

// Menu principal do administrador
void menu_admin();

// Funções auxiliares específicas do admin
int listar_usuarios();
void deletar_usuario();
void deletar_equipe();
void alterar_pontuacao_equipe();

#endif