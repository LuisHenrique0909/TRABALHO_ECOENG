#include "User.h"
#include "Files.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// ========================= VERIFICAÇÕES BÁSICAS =========================

// Permite letras, espaços e acentuação comum em nomes
int verificar_nome(const char *nome) {
    const char *permitidos = "áéíóúàãõâêôçÁÉÍÓÚÀÃÕÂÊÔÇ´~ ";
    for (int i = 0; nome[i] != '\0'; i++) {
        unsigned char c = nome[i];
        if (!isalpha(c) && !strchr(permitidos, c)) {
            return 0;
        }
    }
    return 1;
}

int verificar_ra(const char *ra) {
    for (int i = 0; ra[i] != '\0'; i++) {
        if (!isdigit(ra[i])) return 0;
    }
    return 1;
}

int nome_vazio(const char *nome) {
    for (int i = 0; nome[i] != '\0'; i++) {
        if (!isspace(nome[i])) return 0;
    }
    return 1;
}

// ========================= UTILITÁRIOS =========================

const char* cargo_pra_texto(Cargo cargo) {
    switch (cargo) {
        case ADMIN: return "Admin";
        case PARTICIPANTE: return "Participante";
        case AVALIADOR: return "Avaliador";
        default: return "Desconhecido";
    }
}

// ========================= USUÁRIOS PADRÃO =========================

void inicializar_admin() {
    FILE *f = abrir_csv("users.csv");
    int existe = 0;

    if (f) {
        char linha[256];
        fgets(linha, sizeof(linha), f);
        while (fgets(linha, sizeof(linha), f)) {
            if (strstr(linha, "0,Admin,0,")) {
                existe = 1;
                break;
            }
        }
        fclose(f);
    }

    if (!existe) {
        FILE *fw = escrever_no_csv("users.csv", "RA,NOME,CARGO,SENHA");
        fprintf(fw, "0,Admin,0,admin123\n");
        fclose(fw);
    }
}

void inicializar_avaliador() {
    FILE *f = abrir_csv("users.csv");
    int existe = 0;

    if (f) {
        char linha[256];
        fgets(linha, sizeof(linha), f);
        while (fgets(linha, sizeof(linha), f)) {
            if (strstr(linha, "1,Avaliador,2,")) {
                existe = 1;
                break;
            }
        }
        fclose(f);
    }

    if (!existe) {
        FILE *fw = escrever_no_csv("users.csv", "RA,NOME,CARGO,SENHA");
        fprintf(fw, "1,Avaliador,2,avaliador123\n");
        fclose(fw);
    }
}

// ========================= CADASTRO =========================

void singin() {
    User u;
    inicializar_admin();
    inicializar_avaliador();

    printf("\n--- Cadastro de Participante ---\n");

    do {
        printf("Nome: ");
        fgets(u.nome, sizeof(u.nome), stdin);
        limpar_linha(u.nome);

        if (nome_vazio(u.nome)) {
            printf("Erro: o nome não pode estar vazio.\n");
        } else if (!verificar_nome(u.nome)) {
            printf("Erro: o nome contém caracteres inválidos.\n");
        }
    } while (nome_vazio(u.nome) || !verificar_nome(u.nome));

    char ra_str[20];
    do {
        printf("RA: ");
        fgets(ra_str, sizeof(ra_str), stdin);
        limpar_linha(ra_str);

        if (!verificar_ra(ra_str)) {
            printf("Erro: o RA deve conter apenas números.\n");
        }
    } while (!verificar_ra(ra_str));
    u.RA = atoi(ra_str);

    printf("Senha: ");
    fgets(u.senha, sizeof(u.senha), stdin);
    limpar_linha(u.senha);

    u.cargo = PARTICIPANTE;

    FILE *f = abrir_csv("users.csv");
    if (f) {
        char linha[256];
        int RA_csv, cargo_temp;
        char nome_temp[50], senha_temp[50];

        fgets(linha, sizeof(linha), f);
        while (fgets(linha, sizeof(linha), f)) {
            if (sscanf(linha, "%d,%49[^,],%d,%49[^\n]", &RA_csv, nome_temp, &cargo_temp, senha_temp) == 4) {
                if (RA_csv == u.RA) {
                    fclose(f);
                    printf("Já existe um usuário com esse RA.\n");
                    return;
                }
            }
        }
        fclose(f);
    }

    Result r = cadastrar_user(&u);
    if (r.code == OK)
        printf("Participante cadastrado com sucesso!\n");
    else
        print_err(&r);
}

// ========================= SALVAR NO CSV =========================

Result cadastrar_user(User *u) {
    FILE *f = fopen("./dados/users.csv", "a");
    if (!f) {
        f = fopen("./dados/users.csv", "w");
        if (!f) return erro(ERRO_ARQUIVO, "Erro ao criar users.csv");
        fprintf(f, "RA,NOME,CARGO,SENHA\n");
    } else {
        fseek(f, 0, SEEK_END);
        long pos = ftell(f);
        if (pos == 0) fprintf(f, "RA,NOME,CARGO,SENHA\n");
        fseek(f, 0, SEEK_END);
    }

    fprintf(f, "%d,%s,%d,%s\n", u->RA, u->nome, u->cargo, u->senha);
    fclose(f);
    return ok();
}

// ========================= LOGIN COM 3 TENTATIVAS =========================

User* login_user() {
    FILE *f = abrir_csv("users.csv");
    if (!f) {
        printf("Nenhum usuário encontrado. Cadastre-se primeiro.\n");
        return NULL;
    }

    int tentativas = 0;
    while (tentativas < 3) {
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

        rewind(f);
        char linha[256];
        fgets(linha, sizeof(linha), f);

        User *u = NULL;
        int logado = 0;

        while (fgets(linha, sizeof(linha), f)) {
            int RA_csv, cargo_csv;
            char nome_csv[50], senha_csv[50];

            if (sscanf(linha, "%d,%49[^,],%d,%49[^\n]", &RA_csv, nome_csv, &cargo_csv, senha_csv) == 4) {
                if (RA_csv == RA_input &&
                    strcmp(nome_csv, nome) == 0 &&
                    strcmp(senha_csv, senha) == 0) {
                    
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

        if (logado) {
            fclose(f);
            printf("Login realizado com sucesso!\n");
            printf("Bem-vindo, %s! (%s)\n", u->nome, cargo_pra_texto(u->cargo));
            return u;
        }

        tentativas++;
        printf("Nome, RA ou senha incorretos. (%d/3)\n", tentativas);

        if (tentativas == 3) {
            char resp;
            printf("Você errou 3 vezes. Deseja voltar ao menu principal? (s/n): ");
            scanf(" %c", &resp);
            getchar();
            if (resp == 's' || resp == 'S') {
                fclose(f);
                return NULL;
            } else {
                tentativas = 0; // reinicia tentativas
            }
        }
    }

    fclose(f);
    return NULL;
}