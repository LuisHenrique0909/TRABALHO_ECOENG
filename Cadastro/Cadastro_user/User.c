#include "User.h"
#include "Files.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// ------------------- Inicialização -------------------
void inicializar_admin() {
    FILE *f = abrir_csv("users.csv");
    if (!f) {
        f = escrever_no_csv("users.csv", "ID,NOME,CARGO,SENHA\n");
        fprintf(f, "0,Admin,0,admin123\n");
        fclose(f);
        return;
    }

    char linha[256];
    int existe = 0;
    fgets(linha, sizeof(linha), f);
    while (fgets(linha, sizeof(linha), f)) {
        if (strstr(linha, "Admin")) existe = 1;
    }
    fclose(f);

    if (!existe) {
        f = fopen("./dados/users.csv", "a");
        fprintf(f, "0,Admin,0,admin123\n");
        fclose(f);
    }
}

void inicializar_avaliador() {
    FILE *f = fopen("./dados/users.csv", "a+");
    if (!f) return;
    char linha[256];
    int existe = 0;
    rewind(f);
    while (fgets(linha, sizeof(linha), f)) {
        if (strstr(linha, "Avaliador")) existe = 1;
    }
    if (!existe) {
        fprintf(f, "1,Avaliador,2,avaliador123\n");
    }
    fclose(f);
}

// ------------------- Cadastro de Participante -------------------
void singin() {
    User u;
    inicializar_admin();
    inicializar_avaliador();

    u.cargo = PARTICIPANTE;
    printf("\n--- Cadastro de Participante ---\n");

    printf("Nome completo: ");
    fgets(u.nome, sizeof(u.nome), stdin);
    u.nome[strcspn(u.nome, "\n")] = '\0';

    printf("RA (número): ");
    scanf("%d", &u.id);
    getchar();

    printf("Senha: ");
    fgets(u.senha, sizeof(u.senha), stdin);
    u.senha[strcspn(u.senha, "\n")] = '\0';

    Result r = cadastrar_user(&u);
    if (r.code == OK)
        printf("Participante cadastrado com sucesso!\n");
    else
        print_err(&r);
}

// ------------------- Registro no CSV -------------------
Result cadastrar_user(User *u) {
    FILE *f = escrever_no_csv("users.csv", "ID,NOME,CARGO,SENHA\n");
    if (!f) return erro(ERRO_ARQUIVO, "Erro ao abrir users.csv");

    fprintf(f, "%d,%s,%d,%s\n", u->id, u->nome, u->cargo, u->senha);
    fclose(f);
    return ok();
}