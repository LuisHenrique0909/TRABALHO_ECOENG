#ifndef USER_H
#define USER_H

#include "Result.h"
#include <stdio.h>

typedef enum {
    ADMIN = 0,
    PARTICIPANTE = 1,
    AVALIADOR = 2
} Cargo;

typedef struct {
    int id;
    char nome[50];
    Cargo cargo;
    char senha[50];
} User;

// Cadastro e login
void singin();
Result cadastrar_user(User *u);
User* login_user();

// Usuários padrões
void inicializar_admin();
void inicializar_avaliador();

#endif