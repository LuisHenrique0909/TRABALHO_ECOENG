#include "User.h"
#include "Files.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// ================================================================
// Converte o enum Cargo para texto
// ================================================================
const char* cargo_pra_texto(Cargo cargo) {
    switch (cargo) {
        case ADMIN: return "Admin";
        case PARTICIPANTE: return "Participante";
        case AVALIADOR: return "Avaliador";
        default: return "Desconhecido";
    }
}

// ================================================================
// Inicializa usuário admin padrão, se não existir
// ================================================================
void inicializar_admin() {
    FILE *f = abrir_csv("users.csv");
    int existe = 0;

    if (f) {
        char linha[256];
        fgets(linha, sizeof(linha), f); // cabeçalho
        while (fgets(linha, sizeof(linha), f)) {
            if (strstr(linha, ",Admin,0,")) {
                existe = 1;
                break;
            }
        }
        fclose(f);
    }

    if (!existe) {
        FILE *fw = escrever_no_csv("users.csv", "ID,NOME,CARGO,SENHA");
        fprintf(fw, "0,Admin,0,admin123\n");
        fclose(fw);
    }
}

// ================================================================
// Inicializa usuário avaliador padrão, se não existir
// ================================================================
void inicializar_avaliador() {
    FILE *f = abrir_csv("users.csv");
    int existe = 0;

    if (f) {
        char linha[256];
        fgets(linha, sizeof(linha), f); // cabeçalho
        while (fgets(linha, sizeof(linha), f)) {
            if (strstr(linha, ",Avaliador,2,")) {
                existe = 1;
                break;
            }
        }
        fclose(f);
    }

    if (!existe) {
        FILE *fw = escrever_no_csv("users.csv", "ID,NOME,CARGO,SENHA");
        fprintf(fw, "1,Avaliador,2,avaliador123\n");
        fclose(fw);
    }
}

// ================================================================
// Cadastro de novo participante
// ================================================================
void singin() {
    User u;
    inicializar_admin();
    inicializar_avaliador();

    printf("\n--- Cadastro de Participante ---\n");
    printf("Nome: ");
    fgets(u.nome, sizeof(u.nome), stdin);
    limpar_linha(u.nome);

    printf("RA: ");
    scanf("%d", &u.RA);
    getchar();

    printf("Senha: ");
    fgets(u.senha, sizeof(u.senha), stdin);
    limpar_linha(u.senha);

    u.cargo = PARTICIPANTE;

    // Verifica duplicação de RA
    FILE *f = abrir_csv("users.csv");
    if (f) {
        char linha[256];
        int id_existente;
        fgets(linha, sizeof(linha), f); // cabeçalho
        while (fgets(linha, sizeof(linha), f)) {
            sscanf(linha, "%d,", &id_existente);
            if (id_existente == u.RA) {
                fclose(f);
                printf("Já existe um usuário com esse RA.\n");
                return;
            }
        }
        fclose(f);
    }

    // Salva no CSV
    Result r = cadastrar_user(&u);
    if (r.code == OK)
        printf("Participante cadastrado com sucesso!\n");
    else
        print_err(&r);
}

// ================================================================
// Registra usuário no CSV (modo append, persistente)
// ================================================================
Result cadastrar_user(User *u) {
    FILE *f = escrever_no_csv("users.csv", "RA,NOME,CARGO,SENHA");
    if (!f) return erro(ERRO_ARQUIVO, "Erro ao abrir users.csv");

    fprintf(f, "%d,%s,%d,%s\n", u->RA, u->nome, u->cargo, u->senha);
    fclose(f);
    return ok();
}

// ================================================================
// Realiza login do usuário
// ================================================================
User* login_user() {
    FILE *f = abrir_csv("users.csv");
    if (!f) {
        printf("Nenhum usuário encontrado. Cadastre-se primeiro.\n");
        return NULL;
    }

    char nome[50], senha[50];
    int RA_input;
    printf("\n--- LOGIN ---\n");
    printf("Nome: ");
    fgets(nome, sizeof(nome), stdin);
    limpar_linha(nome);
    printf("RA: ");
    scanf("%d", &RA_input);
    getchar();
    printf("Senha: ");
    fgets(senha, sizeof(senha), stdin);
    limpar_linha(senha);

    char linha[256];
    fgets(linha, sizeof(linha), f); // pula cabeçalho

    User *u = NULL;
    int logado = 0;

    while (fgets(linha, sizeof(linha), f)) {
        int RA_csv, cargo_csv;
        char nome_csv[50], senha_csv[50];
        
        // CORREÇÃO: Formato consistente - RA é inteiro no CSV
        if (sscanf(linha, "%d,%49[^,],%d,%49[^\n]", &RA_csv, nome_csv, &cargo_csv, senha_csv) == 4) {
            
            if (RA_csv == RA_input && 
                strcmp(nome_csv, nome) == 0 && 
                strcmp(senha_csv, senha) == 0) {
                
                // Aloca memória apenas quando encontrar usuário válido
                u = malloc(sizeof(User));
                if (u) {
                    u->RA = RA_csv;
                    strcpy(u->nome, nome_csv);
                    u->cargo = (Cargo)cargo_csv;
                    strcpy(u->senha, senha_csv);
                    logado = 1;
                }
                break;
            }
        }
    }
    fclose(f);

    if (!logado) {
        printf("Nome, RA ou senha incorretos.\n");
        // Não faz free(u) aqui porque u só é alocado se logado=1
        return NULL;
    }

    printf("Login realizado com sucesso!\n");
    printf("Bem-vindo, %s! (%s)\n", u->nome, cargo_pra_texto(u->cargo));
    return u;
}