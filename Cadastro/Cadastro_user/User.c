#include "User.h"
#include "Dados.h"
#include "Result.h"
#include "Files.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MIN_USERNAME_LEN 4
#define MAX_USERNAME_LEN 50
#define MIN_SENHA_LEN 7
#define MAX_SENHA_LEN 10

// ------------------- Validação -------------------

int validar_username(const char *username) {
    int len = strlen(username);
    if (len < MIN_USERNAME_LEN || len > MAX_USERNAME_LEN) {
        printf("O nome deve ter entre %d e %d caracteres.\n", MIN_USERNAME_LEN, MAX_USERNAME_LEN);
        return 0;
    }
    for (int i = 0; i < len; i++) {
        if (!isalpha(username[i]) && username[i] != ' ') {
            printf("O nome deve conter apenas letras e espaços.\n");
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
        printf("A senha deve conter pelo menos uma letra, um número e um caractere especial (!@#$%%&*_-+=.?).\n");
        return 0;
    }

    return 1;
}

// ------------------- Utilitários -------------------

const char* cargo_pra_texto(Cargo cargo) {
    switch (cargo) {
        case ADMIN: return "Admin";
        case PARTICIPANTE: return "Participante";
        case PROFESSOR_RESPONSAVEL: return "Professor Responsável";
        case AVALIADOR: return "Avaliador";
        default: return "Desconhecido";
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

// ------------------- Cadastro -------------------

void singin() {
    User u;
    int opcao;
    inicializar_admin();

    printf("\nSelecione o cargo:\n");
    printf("1 - Participante\n");
    printf("2 - Professor Responsável\n");
    printf("3 - Avaliador\n> ");
    scanf("%d", &opcao);
    getchar(); // limpa o \n

    switch (opcao) {
        case 1: u.cargo = PARTICIPANTE; break;
        case 2: u.cargo = PROFESSOR_RESPONSAVEL; break;
        case 3: u.cargo = AVALIADOR; break;
        default:
            printf("Opção inválida. Cadastro cancelado.\n");
            return;
    }

    printf("Nome completo: ");
    fgets(u.nome, sizeof(u.nome), stdin);
    u.nome[strcspn(u.nome, "\n")] = '\0';

    if (u.cargo == PARTICIPANTE) {
        printf("Digite o RA (somente números): ");
        scanf("%d", &u.id);
        getchar();

        printf("Gmail: ");
        fgets(u.gmail, sizeof(u.gmail), stdin);
        u.gmail[strcspn(u.gmail, "\n")] = '\0';
        strcpy(u.cpf, ""); // não usa CPF
    } 
    else {
        printf("Digite o CPF (somente números): ");
        fgets(u.cpf, sizeof(u.cpf), stdin);
        u.cpf[strcspn(u.cpf, "\n")] = '\0';
        strcpy(u.gmail, ""); // não usa Gmail
        u.id = ultimo_id("users.csv") + 1;
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

// ------------------- Gravação -------------------

Result cadastrar_user(User *u) {
    FILE *f = escrever_no_csv("users.csv", "ID,NOME,CPF,GMAIL,CARGO,SENHA,AUTORIZADO\n");
    if (!f) return erro(ERRO_ARQUIVO, "Erro ao abrir users.csv");

    fprintf(f, "%d,%s,%s,%s,%d,%s,%d\n",
            u->id, u->nome, u->cpf, u->gmail, u->cargo, u->senha, u->autorizado);

    fclose(f);
    return ok();
}

// ------------------- Autorização -------------------

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
