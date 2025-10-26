#include "Cadastro_Equipes.h"
#include "User.h"
#include "Files.h"
#include "Dados.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ==============================
// CADASTRAR NOVA EQUIPE
// ==============================
int cadastrar_equipe(User *usuario_logado) {
    if (!(usuario_logado->cargo == ADMIN)) {
        printf("Acesso negado! Apenas ADMIN podem cadastrar equipes.\n");
        return 0;
    }

    Equipe e;

    FILE *f = escrever_no_csv("equipes.csv", "RA,ID_Equipe,ID_MENTOR,NOME_EQUIPE\n");
    if (!f) {
        printf("Erro ao abrir o arquivo de equipes.\n");
        return 0;
    }

    printf("\nDigite o nome da equipe: ");
    fgets(e.nome, sizeof(e.nome), stdin);
    e.nome[strcspn(e.nome, "\n")] = '\0';

    if (equipe_ja_existe(e.nome)) {
        printf("Erro: já existe uma equipe com o nome '%s'.\n", e.nome);
        fclose(f);
        return 0;
    }

    printf("Digite o ID da Equipe: ");
    scanf("%d", &e.id_Equipe);
    getchar();

    printf("Digite o ID do mentor: ");
    scanf("%d", &e.id_PROFESSOR_RESPONSAVEL);
    getchar();

    // Validação usando identificar_cargo_por_id
    char id_texto[20];
    sprintf(id_texto, "%dP", e.id_PROFESSOR_RESPONSAVEL); // adiciona 'P' para simular ID de professor
    if (identificar_cargo_por_id(id_texto) != PROFESSOR_RESPONSAVEL) {
        printf("Erro: o ID %d não corresponde a um mentor válido.\n", e.id_PROFESSOR_RESPONSAVEL);
        fclose(f);
        return 0;
    }

    int ok = salvar_equipe(&e);
    fclose(f);

    if (ok) {
        printf("Equipe '%s' cadastrada com sucesso! (ID: %d)\n", e.nome, e.id_Equipe);
        return 1;
    }

    printf("Erro ao salvar equipe.\n");
    return 0;
}

// ==============================
// SALVAR EQUIPE NO CSV
// ==============================
int salvar_equipe(Equipe *e) {
    FILE *f = escrever_no_csv("equipes.csv", "ID,ID_HACKATHON,ID_MENTOR,NOME_EQUIPE\n");
    if (!f) return 0;

    int ultimo = ultimo_id("equipes.csv");
    if (ultimo < 0) return 0;

    fprintf(f, "%d,%d,%d,%s\n", ultimo +1, e->id_Equipe, e->id_PROFESSOR_RESPONSAVEL, e->nome);
    fclose(f);
    return 1;
}

// ==============================
// VERIFICAR DUPLICIDADE DE EQUIPE
// ==============================
int equipe_ja_existe(const char *nome) {
    FILE *f = abrir_csv("equipes.csv");
    if (!f) return 0;

    char linha[256];
    int id, id_hack, id_mentor;
    char nome_existente[50];

    while (fgets(linha, sizeof(linha), f)) {
        linha[strcspn(linha, "\n")] = '\0';
        if (sscanf(linha, "%d,%d,%d,%49[^\n]", &id, &id_hack, &id_mentor, nome_existente) == 4) {
            if (strcmp(nome_existente, nome) == 0) {
                fclose(f);
                return 1;
            }
        }
    }

    fclose(f);
    return 0;
}

// ==============================
// PARTICIPANTE JA VINCULADO
// ==============================
int participante_ja_vinculado(int id_user) {
    FILE *f = abrir_csv("user_equipe.csv");
    if (!f) return 0;

    char linha[100];
    int id_equipe, id_part;
    while (fgets(linha, sizeof(linha), f)) {
        if (sscanf(linha, "%d,%d", &id_equipe, &id_part) == 2) {
            if (id_part == id_user) {
                fclose(f);
                return 1;
            }
        }
    }
    fclose(f);
    return 0;
}

// ==============================
// COLETAR DADOS PARA VINCULO
// ==============================
int coletar_vinculo_participante(int *id_equipe, int *id_user) {
    printf("\n===== VINCULAR PARTICIPANTE =====\n");

    printf("Digite o ID da equipe: ");
    if (scanf("%d", id_equipe) != 1) {
        int c; while ((c = getchar()) != '\n' && c != EOF) {}
        return 0;
    }
    getchar();

    printf("Digite o ID do participante: ");
    if (scanf("%d", id_user) != 1) {
        int c; while ((c = getchar()) != '\n' && c != EOF) {}
        return 0;
    }
    getchar();

    // Validação usando identificar_cargo_por_id
    char id_texto[20];
    sprintf(id_texto, "%d", *id_user); // apenas números → PARTICIPANTE
    if (identificar_cargo_por_id(id_texto) != PARTICIPANTE) {
        printf("Erro: o ID %d não pertence a um participante válido.\n", *id_user);
        return 0;
    }

    if (participante_ja_vinculado(*id_user)) {
        printf("Erro: o participante %d já está vinculado a uma equipe.\n", *id_user);
        return 0;
    }

    return 1;
}

// ==============================
// SALVAR VINCULO PARTICIPANTE EQUIPE
// ==============================
int salvar_vinculo_participante(int id_equipe, int id_user) {
    FILE *f = escrever_no_csv("user_equipe.csv", "ID_EQUIPE,ID_PARTICIPANTE\n");
    if (!f) {
        printf("Erro ao abrir user_equipe.csv\n");
        return 0;
    }

    fprintf(f, "%d,%d\n", id_equipe, id_user);
    fclose(f);
    return 1;
}

// ==============================
// CONTROLADOR: VINCULAR PARTICIPANTE
// ==============================
void vincular_participante(User *usuario_logado) {
    if (!(usuario_logado->cargo == ADMIN)) {
        printf("Acesso negado! Apenas ADMIN podem vincular participantes.\n");
        return;
    }

    int id_equipe, id_user;
    if (!coletar_vinculo_participante(&id_equipe, &id_user)) {
        printf("Falha na coleta dos dados do vínculo.\n");
        return;
    }

    if (salvar_vinculo_participante(id_equipe, id_user)) {
        printf("Participante %d vinculado com sucesso à equipe %d!\n", id_user, id_equipe);
    } else {
        printf("Erro ao salvar vínculo no arquivo CSV.\n");
    }
}
