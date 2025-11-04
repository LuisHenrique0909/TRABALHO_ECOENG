#include "Cadastro_Equipes.h"
#include "Files.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ================================================================
// Retorna o último ID usado no arquivo de equipes
// ================================================================
int ultimo_id_equipe() {
    FILE *f = abrir_csv("equipes.csv");
    if (!f) return 0;

    char linha[512];
    fgets(linha, sizeof(linha), f);
    int id, ultimo = 0;
    while (fgets(linha, sizeof(linha), f)) {
        sscanf(linha, "%d,", &id);
        if (id > ultimo) ultimo = id;
    }
    fclose(f);
    return ultimo;
}

// ================================================================
// Verifica se participante já está em alguma equipe
// ================================================================
int participante_ja_tem_equipe(const char *nome_participante) {
    FILE *f = abrir_csv("equipes.csv");
    if (!f) return 0;

    char linha[512], integrantes[300];
    fgets(linha, sizeof(linha), f);

    while (fgets(linha, sizeof(linha), f)) {
        if (strstr(linha, nome_participante)) {
            fclose(f);
            return 1;
        }
    }

    fclose(f);
    return 0;
}

// ================================================================
// Cadastra uma nova equipe (somente o líder pode criar)
// ================================================================
Result cadastrar_equipe(User *lider) {
    if (!lider || lider->cargo != PARTICIPANTE)
        return erro(ERRO_PERMISSAO, "Apenas participantes podem criar equipes.");

    if (participante_ja_tem_equipe(lider->nome))
        return erro(ERRO_LOGICA, "Você já está em uma equipe e não pode criar outra.");

    Equipe e;
    e.id = ultimo_id_equipe() + 1;

    printf("\n--- Cadastro de Equipe ---\n");
    printf("Nome da equipe: ");
    fgets(e.nome, sizeof(e.nome), stdin);
    e.nome[strcspn(e.nome, "\n")] = '\0';

    // Verifica duplicidade de nome
    FILE *f_check = abrir_csv("equipes.csv");
    if (f_check) {
        char linha[512], nome_existente[100];
        fgets(linha, sizeof(linha), f_check);
        int id_tmp;
        char criador_tmp[50], participantes_tmp[300];
        while (fgets(linha, sizeof(linha), f_check)) {
            sscanf(linha, "%d,%99[^,],%49[^,],%299[^\n]", &id_tmp, nome_existente, criador_tmp, participantes_tmp);
            if (strcmp(nome_existente, e.nome) == 0) {
                fclose(f_check);
                return erro(ERRO_LOGICA, "Já existe uma equipe com esse nome.");
            }
        }
        fclose(f_check);
    }

    strcpy(e.criador, lider->nome);
    strcpy(e.participantes, lider->nome);

    FILE *f = escrever_no_csv("equipes.csv", "ID_EQUIPE,NOME_EQUIPE,CRIADOR,PARTICIPANTES");
    if (!f) return erro(ERRO_ARQUIVO, "Erro ao abrir equipes.csv");

    fprintf(f, "%d,%s,%s,%s\n", e.id, e.nome, e.criador, e.participantes);
    fclose(f);

    printf("✅ Equipe '%s' cadastrada com sucesso!\n", e.nome);
    return ok();
}

// ================================================================
// Exibe a equipe do participante logado
// ================================================================
void exibir_equipe_do_participante(User *usuario) {
    FILE *f = abrir_csv("equipes.csv");
    if (!f) {
        printf("Nenhuma equipe cadastrada.\n");
        return;
    }

    char linha[512];
    fgets(linha, sizeof(linha), f);

    int id;
    char nome[100], criador[50], participantes[300];
    int achou = 0;

    while (fgets(linha, sizeof(linha), f)) {
        sscanf(linha, "%d,%99[^,],%49[^,],%299[^\n]", &id, nome, criador, participantes);
        if (strstr(participantes, usuario->nome)) {
            printf("\n--- Sua Equipe ---\n");
            printf("ID: %d\nNome: %s\nCriador: %s\nParticipantes: %s\n", id, nome, criador, participantes);
            achou = 1;
            break;
        }
    }

    fclose(f);

    if (!achou)
        printf("Você ainda não está em nenhuma equipe.\n");
}

// ================================================================
// Lista todas as equipes (usado por admin)
// ================================================================
void listar_todas_equipes() {
    FILE *f = abrir_csv("equipes.csv");
    if (!f) {
        printf("Nenhuma equipe cadastrada.\n");
        return;
    }

    char linha[512];
    fgets(linha, sizeof(linha), f);
    printf("\n--- Lista de todas as equipes ---\n");

    while (fgets(linha, sizeof(linha), f)) {
        printf("%s", linha);
    }

    fclose(f);
}

// ================================================================
// Adiciona participante a uma equipe (feito pelo líder)
// ================================================================
void adicionar_participante(User *lider) {
    FILE *f = abrir_csv("equipes.csv");
    if (!f) {
        printf("Nenhuma equipe cadastrada.\n");
        return;
    }

    char nome_participante[50];
    printf("Nome do participante a adicionar: ");
    fgets(nome_participante, sizeof(nome_participante), stdin);
    nome_participante[strcspn(nome_participante, "\n")] = '\0';

    if (participante_ja_tem_equipe(nome_participante)) {
        printf("❌ Este participante já está em outra equipe.\n");
        fclose(f);
        return;
    }

    FILE *tmp = fopen("./dados/equipes_temp.csv", "w");
    fprintf(tmp, "ID_EQUIPE,NOME_EQUIPE,CRIADOR,PARTICIPANTES\n");

    char linha[512];
    fgets(linha, sizeof(linha), f);
    int id;
    char nome[100], criador[50], participantes[300];

    while (fgets(linha, sizeof(linha), f)) {
        sscanf(linha, "%d,%99[^,],%49[^,],%299[^\n]", &id, nome, criador, participantes);
        if (strcmp(criador, lider->nome) == 0) {
            strcat(participantes, ";");
            strcat(participantes, nome_participante);
        }
        fprintf(tmp, "%d,%s,%s,%s\n", id, nome, criador, participantes);
    }

    fclose(f);
    fclose(tmp);

    remove("./dados/equipes.csv");
    rename("./dados/equipes_temp.csv", "./dados/equipes.csv");

    printf("✅ Participante '%s' adicionado com sucesso!\n", nome_participante);
}

// ================================================================
// Remove participante de uma equipe (feito pelo líder)
// ================================================================
void remover_participante(User *lider) {
    FILE *f = abrir_csv("equipes.csv");
    if (!f) {
        printf("Nenhuma equipe cadastrada.\n");
        return;
    }

    char nome_remover[50];
    printf("Nome do participante a remover: ");
    fgets(nome_remover, sizeof(nome_remover), stdin);
    nome_remover[strcspn(nome_remover, "\n")] = '\0';

    FILE *tmp = fopen("./dados/equipes_temp.csv", "w");
    fprintf(tmp, "ID_EQUIPE,NOME_EQUIPE,CRIADOR,PARTICIPANTES\n");

    char linha[512];
    fgets(linha, sizeof(linha), f);
    int id;
    char nome[100], criador[50], participantes[300];
    int alterado = 0;

    while (fgets(linha, sizeof(linha), f)) {
        sscanf(linha, "%d,%99[^,],%49[^,],%299[^\n]", &id, nome, criador, participantes);

        if (strcmp(criador, lider->nome) == 0) {
            char nova_lista[300] = "";
            char *token = strtok(participantes, ";");
            while (token) {
                if (strcmp(token, nome_remover) != 0) {
                    strcat(nova_lista, token);
                    strcat(nova_lista, ";");
                }
                token = strtok(NULL, ";");
            }
            if (nova_lista[strlen(nova_lista) - 1] == ';')
                nova_lista[strlen(nova_lista) - 1] = '\0';
            fprintf(tmp, "%d,%s,%s,%s\n", id, nome, criador, nova_lista);
            alterado = 1;
        } else {
            fprintf(tmp, "%d,%s,%s,%s\n", id, nome, criador, participantes);
        }
    }

    fclose(f);
    fclose(tmp);

    remove("./dados/equipes.csv");
    rename("./dados/equipes_temp.csv", "./dados/equipes.csv");

    if (alterado)
        printf("✅ Participante removido com sucesso!\n");
    else
        printf("Nenhuma modificação feita.\n");
}