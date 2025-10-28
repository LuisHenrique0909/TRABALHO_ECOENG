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
    Cargo cargo;      // enum definido → ADMIN, PARTICIPANTE, PROFESSOR_RESPONSAVEL, AVALIADOR
    char senha[50];
    int autorizado;   // 0 = aguardando aprovação, 1 = autorizado
} User;

// Validação de username e senha
int validar_username(const char *username);
int validar_senha(const char *senha);

// Verificação de existência no arquivo CSV
int existe_nome(const char *username);

// Cadastro e autenticação
void singin();
Result cadastrar_user(User *u);

// Autorização (somente admin)
Result autorizar_user(int id);

// Busca e listagem
User* procura_user(int id);
User* lista_users_por_cargo(Cargo cargo, int *quantidade);

// Identificação de cargo por ID (ou CPF)
Cargo identificar_cargo_por_id(const char *id_texto);

// Funções auxiliares
const char* cargo_pra_texto(Cargo cargo);
Cargo int_pra_cargo(int valor);

// Inicialização automática do admin
void inicializar_admin();

#endif
