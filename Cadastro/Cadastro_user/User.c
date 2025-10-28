#include "User.h"
#include "Dados.h"
#include "Files.h"
#include "Result.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MIN_USERNAME_LEN 4
#define MAX_USERNAME_LEN 50

#define MIN_SENHA_LEN 7
#define MAX_SENHA_LEN 10

// ------------------- Funções de Validação -------------------

int validar_username(const char *username) {
    int len = strlen(username);
    if (len < MIN_USERNAME_LEN || len > MAX_USERNAME_LEN) {
        printf("O username deve ter entre %d e %d caracteres.\n",
               MIN_USERNAME_LEN, MAX_USERNAME_LEN);
        return 0;
    }
    for (int i = 0; i < len; i++) {
        if (!isalnum(username[i]) && username[i] != '_') {
            printf("O username só pode conter letras, números ou '_'.\n");
            return 0;
        }
    }
    return 1;
}

int validar_senha(const char *senha) {
    int len = strlen(senha);
    int tem_letra = 0, tem_numero = 0, tem_especial = 0;
    if (len < MIN_SENHA_LEN || len > MAX_SENHA_LEN) {
        printf("A senha deve ter entre %d e %d caracteres.\n", MIN_SENHA_LEN, MAX_SENHA_LEN);
        return 0;
    }
    for (int i = 0; i < len; i++) {
        if (isalpha(senha[i])) tem_letra = 1;
        else if (isdigit(senha[i])) tem_numero = 1;
        else if (strchr("!@#$%&*_-+=?.", senha[i])) tem_especial = 1;
        else {
            printf("Caractere inválido: '%c'\n", senha[i]);
            return 0;
        }
    }
    if (!tem_letra || !tem_numero || !tem_especial) {
        printf("A senha deve conter letra, número e caractere especial (!@#$%%&*_-+=.?).\n");
        return 0;
    }
    return 1;
}

// ------------------- Funções de Identificação -------------------

Cargo identificar_cargo_por_id(const char *id_texto) {
    int len = strlen(id_texto);
    if (len == 0) return PARTICIPANTE;
    char ultimo = id_texto[len - 1];

    for (int i = 0; i < len - 1; i++) {
        if (!isdigit(id_texto[i])) return PARTICIPANTE;
    }

    if (isdigit(ultimo)) return PARTICIPANTE;
    if (ultimo == 'P' || ultimo == 'p') return PROFESSOR_RESPONSAVEL;
    if (ultimo == 'A' || ultimo == 'a') return AVALIADOR;
    return PARTICIPANTE;
}

const char* cargo_pra_texto(Cargo cargo) {
    switch (cargo) {
        case ADMIN: return "Admin";
        case PARTICIPANTE: return "Participante";
        case PROFESSOR_RESPONSAVEL: return "Professor Responsável";
        case AVALIADOR: return "Avaliador";
        default: return "Desconhecido";
    }
}

Cargo int_pra_cargo(int valor) {
    switch (valor) {
        case 0: return ADMIN;
        case 1: return PARTICIPANTE;
        case 2: return PROFESSOR_RESPONSAVEL;
        case 3: return AVALIADOR;
        default: return PARTICIPANTE;
    }
}

// ------------------- Inicialização do Admin -------------------

void inicializar_admin() {
    FILE *f = abrir_csv("users.csv");
    if (!f) {
        f = escrever_no_csv("users.csv", "ID,NOME,CPF,GMAIL,CARGO,SENHA,AUTORIZADO\n");
        fprintf(f, "0,Luis Henrique,,admin@gmail.com,0,admin123,1\n");
        fclose(f);
        return;
    }

    char linha[256];
    int achou = 0;
    fgets(linha, sizeof(linha), f);
    while (fgets(linha, sizeof(linha), f)) {
        if (strstr(linha, "Luis Henrique")) {
            achou = 1;
            break;
        }
    }
    fclose(f);

    if (!achou) {
        f = fopen("users.csv", "a");
        fprintf(f, "0,Luis Henrique,,admin@gmail.com,0,admin123,1\n");
        fclose(f);
    }
}

// ------------------- Função principal de cadastro -------------------

void singin() {
    User u;
    char id_ou_cpf[30];
    inicializar_admin();

    printf("Digite o ID (RA) ou CPF: ");
    fgets(id_ou_cpf, sizeof(id_ou_cpf), stdin);
    id_ou_cpf[strcspn(id_ou_cpf, "\n")] = '\0';

    u.cargo = identificar_cargo_por_id(id_ou_cpf);
    printf("Cargo identificado: %s\n", cargo_pra_texto(u.cargo));

    if (u.cargo == PARTICIPANTE) {
        strcpy(u.cpf, "");
        u.id = atoi(id_ou_cpf);
    } else {
        strcpy(u.cpf, id_ou_cpf);
        u.id = ultimo_id("users.csv") + 1;
    }

    printf("Gmail: ");
    fgets(u.gmail, sizeof(u.gmail), stdin);
    u.gmail[strcspn(u.gmail, "\n")] = '\0';

    if (u.cargo == PARTICIPANTE) {
        while (1) {
            printf("Nome: ");
            fgets(u.nome, sizeof(u.nome), stdin);
            u.nome[strcspn(u.nome, "\n")] = '\0';
            if (validar_username(u.nome)) break;
        }
    } else {
        printf("Nome completo: ");
        fgets(u.nome, sizeof(u.nome), stdin);
        u.nome[strcspn(u.nome, "\n")] = '\0';
    }

    while (1) {
        printf("Senha: ");
        fgets(u.senha, sizeof(u.senha), stdin);
        u.senha[strcspn(u.senha, "\n")] = '\0';
        if (validar_senha(u.senha)) break;
    }

    u.autorizado = (u.cargo == PARTICIPANTE) ? 1 : 0;

    Result r = cadastrar_user(&u);
    if (r.code == OK) {
        if (u.autorizado)
            printf("Usuário criado e autorizado: %s (%s)\n", u.nome, cargo_pra_texto(u.cargo));
        else
            printf("Cadastro enviado. Aguarde autorização do administrador.\n");
    } else {
        print_err(&r);
    }
}

// ------------------- Função de cadastro -------------------

Result cadastrar_user(User *u) {
    FILE *f = escrever_no_csv("users.csv", "ID,NOME,CPF,GMAIL,CARGO,SENHA,AUTORIZADO\n");
    if (!f) return erro(ERRO_ARQUIVO, "erro ao abrir users.csv");

    if (existe_nome(u->nome)) return erro(ERRO_LOGICA, "Username já está em uso!");

    fprintf(f, "%d,%s,%s,%s,%d,%s,%d\n",
            u->id, u->nome, u->cpf, u->gmail, u->cargo, u->senha, u->autorizado);

    fclose(f);
    return ok();
}

// ------------------- Função de autorização (somente admin) -------------------

Result autorizar_user(int id) {
    char nome[50], senha[50];
    printf("\n=== Login do Administrador ===\n");
    printf("Nome: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = '\0';

    printf("Senha: ");
    fgets(senha, sizeof(senha), stdin);
    senha[strcspn(senha, "\n")] = '\0';

    if (strcmp(nome, "Luis Henrique") != 0 || strcmp(senha, "admin123") != 0) {
        return erro(ERRO_PERMISSAO, "Acesso negado! Somente o admin pode autorizar usuários.");
    }

    FILE *f = abrir_csv("users.csv");
    if (!f) return erro(ERRO_ARQUIVO, "Arquivo users.csv não encontrado.");

    FILE *temp = fopen("temp.csv", "w");
    if (!temp) {
        fclose(f);
        return erro(ERRO_ARQUIVO, "Erro ao criar arquivo temporário.");
    }

    char linha[512];
    fgets(linha, sizeof(linha), f);
    fprintf(temp, "%s", linha); // cabeçalho

    int id_lido, cargo, autorizado;
    char nome_user[50], cpf[20], gmail[100], senha_user[50];
    int alterado = 0;

    while (fgets(linha, sizeof(linha), f)) {
        if (sscanf(linha, "%d,%49[^,],%19[^,],%99[^,],%d,%49[^,],%d",
                   &id_lido, nome_user, cpf, gmail, &cargo, senha_user, &autorizado) == 7) {

            if (id_lido == id) {
                autorizado = 1;
                alterado = 1;
            }
            fprintf(temp, "%d,%s,%s,%s,%d,%s,%d\n",
                    id_lido, nome_user, cpf, gmail, cargo, senha_user, autorizado);
        }
    }

    fclose(f);
    fclose(temp);
    remove("users.csv");
    rename("temp.csv", "users.csv");

    if (!alterado)
        return erro(ERRO_LOGICA, "Usuário não encontrado para autorização.");

    return ok();
}
