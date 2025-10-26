#ifndef User_H
#define User_H

#include "Utilidades/Rsult/Result.h"
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
    Cargo cargo;     // enum definido→ PARTICIPANTE, PROFESSOR_RESPONSAVEL, AVALIADOR
    char senha[50];
} User;

// Validação de username e senha
int validar_username(const char *username);
int validar_senha(const char *senha);

// Verificação de existência no arquivo CSV
int existe_nome(const char *username);

// Cadastro e autenticação
void singin();
Result cadastrar_user(User *u);

// Busca e listagem
User* procura_user(int id);
User* lista_users_por_cargo(Cargo cargo, int *quantidade);

// Identificação de cargo por ID (nova função adicionada)
Cargo identificar_cargo_por_id(const char *id_texto);

// Funções auxiliares para manipulação de cargos
const char* cargo_pra_texto(Cargo cargo);
Cargo int_pra_cargo(int valor);

#endif
