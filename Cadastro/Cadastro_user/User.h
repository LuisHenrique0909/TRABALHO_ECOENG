#ifndef USER_H
#define USER_H

#include "Result.h"
#include "files.h"
#include <stdio.h>

// Enum de cargos
typedef enum {
    ADMIN = 0,
    PARTICIPANTE = 1,
    AVALIADOR = 2
} Cargo;

// Estrutura principal do usuário
typedef struct {
    int RA;
    char nome[50];
    Cargo cargo;
    char senha[50];
} User;

// Funções principais
void singin();                   // Cadastro de participante
Result cadastrar_user(User *u);  // Salva usuário no CSV
User* login_user();              // Realiza login e retorna struct alocada

// Usuários padrão
void inicializar_admin();
void inicializar_avaliador();

// Utilitários
const char* cargo_pra_texto(Cargo cargo);

#endif