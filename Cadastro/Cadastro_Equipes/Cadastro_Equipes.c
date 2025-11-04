#include "Cadastro_Equipes.h"
#include "Files.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ============================================================================
// Verifica se uma equipe já existe
// ============================================================================
int equipe_existe(const char *nome_equipe) {
    FILE *f = abrir_csv("equipes.csv");
    if (!f) return 0;

    char linha[512], nome[100];
    fgets(linha, sizeof(linha), f); // pula cabeçalho
    while (fgets(linha, sizeof(linha), f)) {
        sscanf(linha, "%*d,%99[^,],", nome);
        if (strcmp(nome, nome_equipe) == 0) {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

// ============================================================================
// Verifica se participante já está em alguma equipe
// ============================================================================
int participante_em_alguma_equipe(const char *nome_participante) {
    FILE *f = abrir_csv("equipes.csv");
    if (!f) return 0;

    char linha[512];
    fgets(linha, sizeof(linha), f); // pula cabeçalho
    while (fgets(linha, sizeof(linha), f)) {
        if (strstr(linha, nome_participante)) {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

// ============================================================================
// Cadastra uma nova equipe
// ============================================================================
Result cadastrar_equipe(User *lider) {
    if (!lider) return erro(ERRO_INVALIDO, "Usuário inválido");

    criar_diretorio_dados();

    // Verifica se o líder já tem equipe
    if (participante_em_alguma_equipe(lider->nome))
        return erro(ERRO_LOGICA, "Você já está em uma equipe.");

    Equipe e;
    e.id_equipe = (int)contar_linhas("equipes.csv"); // gera ID automaticamente
    strcpy(e.nome_lider, lider->nome);

    printf("\n--- Cadastro de Equipe ---\n");
    printf("Nome da equipe: ");
    fgets(e.nome_equipe, sizeof(e.nome_equipe), stdin);
    e.nome_equipe[strcspn(e.nome_equipe, "\n")] = '\0';

    if (equipe_existe(e.nome_equipe))
        return erro(ERRO_LOGICA, "Já existe uma equipe com esse nome.");

    strcpy(e.participantes, lider->nome); // líder é o primeiro integrante

    FILE *f = escrever_no_csv("equipes.csv", "ID_EQUIPE,NOME_EQUIPE,LIDER,PARTICIPANTES\n");
    if (!f) return erro(ERRO_ARQUIVO, "Erro ao abrir arquivo de equipes.");

    fprintf(f, "%d,%s,%s,%s\n", e.id_equipe, e.nome_equipe, e.nome_lider, e.participantes);
    fclose(f);

    return ok();
}

// ============================================================================
// Adiciona um participante à equipe
// ============================================================================
Result adicionar_participante_equipe(const char *nome_equipe, const char *nome_participante) {
    if (!nome_equipe || !nome_participante)
        return erro(ERRO_INVALIDO, "Parâmetros inválidos.");

    if (participante_em_alguma_equipe(nome_participante))
        return erro(ERRO_LOGICA, "Participante já está em uma equipe.");

    FILE *f = abrir_csv("equipes.csv");
    if (!f) return erro(ERRO_ARQUIVO, "Arquivo de equipes inexistente.");

    char linha[512];
    fgets(linha, sizeof(linha), f); // pula cabeçalho

    FILE *temp = fopen("./dados/temp.csv", "w");
    fprintf(temp, "ID_EQUIPE,NOME_EQUIPE,LIDER,PARTICIPANTES\n");

    int modificado = 0;
    int id;
    char nome[100], lider[100], membros[300];

    while (fgets(linha, sizeof(linha), f)) {
        sscanf(linha, "%d,%99[^,],%99[^,],%299[^\n]", &id, nome, lider, membros);

        if (strcmp(nome, nome_equipe) == 0) {
            strcat(membros, ";");
            strcat(membros, nome_participante);
            modificado = 1;
        }
        fprintf(temp, "%d,%s,%s,%s\n", id, nome, lider, membros);
    }

    fclose(f);
    fclose(temp);

    remove("./dados/equipes.csv");
    rename("./dados/temp.csv", "./dados/equipes.csv");

    return modificado ? ok() : erro(ERRO_LOGICA, "Equipe não encontrada.");
}

// ============================================================================
// Remove um participante da equipe
// ============================================================================
Result remover_participante_equipe(const char *nome_equipe, const char *nome_participante) {
    if (!nome_equipe || !nome_participante)
        return erro(ERRO_INVALIDO, "Parâmetros inválidos.");

    FILE *f = abrir_csv("equipes.csv");
    if (!f) return erro(ERRO_ARQUIVO, "Arquivo de equipes inexistente.");

    char linha[512];
    fgets(linha, sizeof(linha), f); // pula cabeçalho

    FILE *temp = fopen("./dados/temp.csv", "w");
    fprintf(temp, "ID_EQUIPE,NOME_EQUIPE,LIDER,PARTICIPANTES\n");

    int id;
    char nome[100], lider[100], membros[300];
    int alterado = 0;

    while (fgets(linha, sizeof(linha), f)) {
        sscanf(linha, "%d,%99[^,],%99[^,],%299[^\n]", &id, nome, lider, membros);

        if (strcmp(nome, nome_equipe) == 0) {
            char *ptr = strstr(membros, nome_participante);
            if (ptr) {
                char buffer[300] = "";
                char *token = strtok(membros, ";");
                while (token) {
                    if (strcmp(token, nome_participante) != 0) {
                        strcat(buffer, token);
                        strcat(buffer, ";");
                    }
                    token = strtok(NULL, ";");
                }
                strcpy(membros, buffer);
                alterado = 1;
            }
        }

        fprintf(temp, "%d,%s,%s,%s\n", id, nome, lider, membros);
    }

    fclose(f);
    fclose(temp);
    remove("./dados/equipes.csv");
    rename("./dados/temp.csv", "./dados/equipes.csv");

    return alterado ? ok() : erro(ERRO_LOGICA, "Participante não encontrado.");
}

// ============================================================================
// Lista todas as equipes
// ============================================================================
void listar_equipes() {
    FILE *f = abrir_csv("equipes.csv");
    if (!f) {
        printf("Nenhuma equipe cadastrada.\n");
        return;
    }

    char linha[512];
    fgets(linha, sizeof(linha), f);
    printf("\n--- LISTA DE EQUIPES ---\n");

    int id;
    char nome[100], lider[100], membros[300];

    while (fgets(linha, sizeof(linha), f)) {
        sscanf(linha, "%d,%99[^,],%99[^,],%299[^\n]", &id, nome, lider, membros);
        printf("ID: %d | Nome: %s | Líder: %s | Membros: %s\n", id, nome, lider, membros);
    }

    fclose(f);
}

// ============================================================================
// Exibe a equipe e integrantes do usuário logado
// ============================================================================
void exibir_equipe_do_participante(User *usuario) {
    if (!usuario) {
        printf("Erro: usuário inválido.\n");
        return;
    }

    FILE *f = abrir_csv("equipes.csv");
    if (!f) {
        printf("Nenhuma equipe cadastrada.\n");
        return;
    }

    char linha[512];
    fgets(linha, sizeof(linha), f); // pula cabeçalho

    int encontrado = 0;
    int id;
    char nome[100], lider[100], membros[300];

    while (fgets(linha, sizeof(linha), f)) {
        sscanf(linha, "%d,%99[^,],%99[^,],%299[^\n]", &id, nome, lider, membros);
        if (strstr(membros, usuario->nome)) {
            printf("\n--- SUA EQUIPE ---\n");
            printf("ID: %d\nNome: %s\nLíder: %s\nIntegrantes: %s\n", id, nome, lider, membros);
            encontrado = 1;
            break;
        }
    }

    fclose(f);

    if (!encontrado)
        printf("\nVocê ainda não está em nenhuma equipe.\n");
}