#ifndef User_H
#define User_H

#include "Result.h"
#include <stdio.h>

typedef enum {
    ADMIN = 0,
    PARTICIPANTE = 1,
    PROFESSOR_RESPONSAVEL = 2,
    AVALIADOR = 3,
} Cargo;

// Estrutura principal de Usuário
typedef struct {
    int id;
    char nome[50];
    char cpf[20];
    char gmail[100];
    Cargo cargo;
    char senha[50];
    int autorizado; // 0 = aguardando aprovação, 1 = autorizado
} User;

// Validação
int validar_username(const char *username);
int validar_senha(const char *senha);

// Cadastro e autenticação
void singin();
Result cadastrar_user(User *u);

// Autorização (somente admin)
Result autorizar_user(int id);

// Funções auxiliares
const char* cargo_pra_texto(Cargo cargo);

// Inicialização do admin
void inicializar_admin();

#endif
