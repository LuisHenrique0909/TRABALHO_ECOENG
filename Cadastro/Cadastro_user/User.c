#include "User.h"
#include "Dados.h"
#include "Files.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MIN_USERNAME_LEN 4
#define MAX_USERNAME_LEN 50

#define MIN_SENHA_LEN 7
#define MAX_SENHA_LEN 10


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
        printf("A senha deve conter pelo menos uma letra, um numero e um caractere especial (!@#$%%&*_-+=.?).\n");
        return 0;
    }

    return 1;
}

int existe_nome(const char *username){
    FILE *f = abrir_csv("users.csv");
    char linha[256];

    if (!f){
        return -1;
    }
    
    int id_lido;
    char nome[50];

    fgets(linha, sizeof(linha), f);

    while (fgets(linha, sizeof(linha), f)) {
        if (sscanf(linha, "%d,%49[^,],", &id_lido, nome) == 2) {
            if (strcmp(username, nome) == 0) {
                fclose(f);
                return 1;
            }
        }
    }

    fclose(f);
    return 0;
}

/*
 identifica o cargo com base no ID digitado
   Regras:
   - Apenas números → PARTICIPANTE
   - Números + 'P' → PROFESSOR_RESPONSAVEL
   - Números + 'A' → AVALIADOR
   */
Cargo identificar_cargo_por_id(const char *id_texto) {
    int len = strlen(id_texto);

    if (len == 0) {
        printf("ID inválido.\n");
        return PARTICIPANTE;
    }

    char ultimo = id_texto[len - 1];

    // Verifica se todos os anteriores são dígitos
    for (int i = 0; i < len - 1; i++) {
        if (!isdigit(id_texto[i])) {
            printf("ID inválido: deve conter apenas números antes da letra final.\n");
            return PARTICIPANTE;
        }
    }

    // Apenas números → Participante
    if (isdigit(ultimo)) {
        return PARTICIPANTE;
    }

    // Números + P → Professor Responsável
    if (ultimo == 'P' || ultimo == 'p') {
        return PROFESSOR_RESPONSAVEL;
    }

    // Números + A → Avaliador
    if (ultimo == 'A' || ultimo == 'a') {
        return AVALIADOR;
    }

    printf("ID inválido: letra final desconhecida (%c)\n", ultimo);
    return PARTICIPANTE;
}

const char* cargo_pra_texto(Cargo cargo) {
    switch (cargo) {
        case PARTICIPANTE:
            return "Participante";
        case PROFESSOR_RESPONSAVEL:
            return "Professor Responsável";
        case AVALIADOR:
            return "Avaliador";
        default:
            return "Cargo Desconhecido";
    }
}

Cargo int_pra_cargo(int valor) {
    switch (valor) {
        case 0:
            return PARTICIPANTE;
        case 1:
            return PROFESSOR_RESPONSAVEL;
        case 2:
            return AVALIADOR;
        default:
            return PARTICIPANTE; // valor padrão
    }
}


/* ------------------------------------------------------------------
   Função principal de cadastro
------------------------------------------------------------------ */
void singin() {
    User e;
    char id_texto[20];

    // Leitura do ID e identificação automática do cargo
    printf("Digite o ID: ");
    fgets(id_texto, sizeof(id_texto), stdin);
    id_texto[strcspn(id_texto, "\n")] = '\0';

    e.cargo = identificar_cargo_por_id(id_texto);
    printf("Cargo identificado: %s\n\n", cargo_pra_texto(e.cargo));

    // Leitura e validação do username
    while (1) {
        printf("Username: ");
        fgets(e.nome, sizeof(e.nome), stdin);
        e.nome[strcspn(e.nome, "\n")] = '\0';

        if (validar_username(e.nome)) break;
        printf("Tente novamente.\n\n");
    }

    // Leitura e validação da senha
    while(1){
        printf("Senha: ");
        fgets(e.senha, sizeof(e.senha), stdin);
        e.senha[strcspn(e.senha, "\n")] = '\0';

        if (validar_senha(e.senha)) break;
        printf("Tente novamente.\n\n");
    }

    Result r = cadastrar_user(&e);

    if (r.code == OK) {
        printf("Usuário criado: %s (%s)\n", e.nome, cargo_pra_texto(e.cargo));
    } else {
        print_err(&r);
    }
}


Result cadastrar_user(User *u){
    FILE *f = escrever_no_csv("users.csv", "ID,NOME,CARGO,SENHA\n");

    if (f == NULL) return erro(ERRO_ARQUIVO, "erro ao abrir o arquivo users.csv");

    if (existe_nome((u->nome))) {
        return erro(ERRO_LOGICA, "Username ja esta em uso!");
    }

    fprintf(f,"%d,%s,%d,%s\n",ultimo_id("users.csv")+1, u->nome, u->cargo,u->senha);

    fclose(f);
    return ok();
}

User* procura_user(int id) {
    FILE *f = abrir_csv("users.csv");
    char linha[256];
    
    static User user;
    int id_lido;
    char nome[50], senha[50], cargo_str[20];

    fgets(linha, sizeof(linha), f);

    while (fgets(linha, sizeof(linha), f)) {
        if (sscanf(linha, "%d,%49[^,],%19[^,],%49[^\n]", &id_lido, nome, cargo_str, senha) == 4) {
            if (id_lido == id) {
                user.id = id_lido;
                strcpy(user.nome, nome);
                user.cargo = identificar_cargo_por_id(cargo_str);
                strcpy(user.senha, senha);
                fclose(f);
                return &user;
            }
        }
    }

    fclose(f);
    return NULL;
}


User* lista_users_por_cargo(Cargo cargo, int *quantidade) {
    FILE *f = abrir_csv("users.csv");
    if (!f) return NULL;

    char linha[256];
    *quantidade = 0;
    User *lista = NULL;

    fgets(linha, sizeof(linha), f);

    while (fgets(linha, sizeof(linha), f)) {
        int id_lido, cargo_lido;
        char nome[50], senha[50];

        if (sscanf(linha, "%d,%49[^,],%d,%49[^\n]", &id_lido, nome, &cargo_lido, senha) == 4) {
            if (int_pra_cargo(cargo_lido) == cargo) {
                User *tmp = realloc(lista, sizeof(User) * (*quantidade + 1));
                if (!tmp) {
                    free(lista);
                    fclose(f);
                    return NULL;
                }
                lista = tmp;
                lista[*quantidade].id = id_lido;
                strcpy(lista[*quantidade].nome, nome);
                lista[*quantidade].cargo = int_pra_cargo(cargo_lido);
                strcpy(lista[*quantidade].senha, senha);
                (*quantidade)++;
            }
        }
    }

    fclose(f);

    if (*quantidade == 0) {
        free(lista);
        return NULL;
    }

    return lista;
}
