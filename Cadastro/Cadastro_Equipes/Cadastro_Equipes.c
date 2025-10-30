#include "Cadastro_Equipes.h"

// --------------------------------------------------
// Verifica se o participante já está em alguma equipe
// --------------------------------------------------
int participante_ja_vinculado(int ra) {
    FILE *f = abrir_csv("equipes.csv");
    if (!f) return 0;

    char linha[256];
    fgets(linha, sizeof(linha), f); // cabeçalho

    int ra_lido, ra_criador;
    char nome_equipe[100], nome_criador[50];
    while (fgets(linha, sizeof(linha), f)) {
        if (sscanf(linha, "%99[^,],%d,%49[^,],%d",
                   nome_equipe, &ra_criador, nome_criador, &ra_lido) >= 2) {
            if (ra == ra_criador || ra == ra_lido) {
                fclose(f);
                return 1; // Já está em uma equipe
            }
        }
    }
    fclose(f);
    return 0;
}

// --------------------------------------------------
// Retorna 1 se o RA informado for criador de uma equipe
// --------------------------------------------------
int criador_da_equipe(int ra) {
    FILE *f = abrir_csv("equipes.csv");
    if (!f) return 0;

    char linha[256];
    fgets(linha, sizeof(linha), f);

    char nome[100], nome_criador[50];
    int ra_criador;
    while (fgets(linha, sizeof(linha), f)) {
        if (sscanf(linha, "%99[^,],%d,%49[^\n]", nome, &ra_criador, nome_criador) == 3) {
            if (ra_criador == ra) {
                fclose(f);
                return 1;
            }
        }
    }

    fclose(f);
    return 0;
}

// --------------------------------------------------
// Busca uma equipe pelo RA do criador
// --------------------------------------------------
Equipe* buscar_equipe_por_ra(int ra) {
    FILE *f = abrir_csv("equipes.csv");
    if (!f) return NULL;

    static Equipe e;
    char linha[256];
    fgets(linha, sizeof(linha), f);

    while (fgets(linha, sizeof(linha), f)) {
        if (sscanf(linha, "%99[^,],%d,%49[^\n]", e.nome, &e.ra_criador, e.nome_criador) == 3) {
            if (e.ra_criador == ra) {
                fclose(f);
                return &e;
            }
        }
    }

    fclose(f);
    return NULL;
}

// --------------------------------------------------
// Cadastrar nova equipe
// --------------------------------------------------
Result cadastrar_equipe(User *usuario_logado) {
    if (!usuario_logado) return erro(ERRO_LOGICA, "Usuário não está logado.");

    if (usuario_logado->cargo != PARTICIPANTE)
        return erro(ERRO_PERMISSAO, "Somente PARTICIPANTES podem cadastrar equipes.");

    if (participante_ja_vinculado(usuario_logado->id))
        return erro(ERRO_LOGICA, "Você já faz parte de uma equipe e não pode criar outra.");

    char nome_equipe[MAX_EQUIPE_NOME];
    printf("\n--- CADASTRO DE EQUIPE ---\n");
    printf("Digite o nome da equipe: ");
    fgets(nome_equipe, sizeof(nome_equipe), stdin);
    nome_equipe[strcspn(nome_equipe, "\n")] = '\0';

    FILE *f = escrever_no_csv("equipes.csv", "NOME_EQUIPE,RA_CRIADOR,NOME_CRIADOR\n");
    if (!f) return erro(ERRO_ARQUIVO, "Erro ao abrir equipes.csv");

    fprintf(f, "%s,%d,%s\n", nome_equipe, usuario_logado->id, usuario_logado->nome);
    fclose(f);

    printf("\nEquipe '%s' cadastrada com sucesso!\n", nome_equipe);
    printf("Você é o criador da equipe.\n");
    printf("Apenas você poderá adicionar ou remover participantes.\n");

    return ok();
}

// --------------------------------------------------
// Vincular participante a uma equipe (apenas criador)
// --------------------------------------------------
Result vincular_participante(User *usuario_logado) {
    if (!usuario_logado) return erro(ERRO_LOGICA, "Usuário não está logado.");

    if (usuario_logado->cargo != PARTICIPANTE)
        return erro(ERRO_PERMISSAO, "Somente PARTICIPANTES podem vincular outros participantes.");

    if (!criador_da_equipe(usuario_logado->id)) {
        printf("\nVocê não é criador de nenhuma equipe.\n");
        printf("Você só pode visualizar a equipe da qual participa.\n\n");
        listar_equipes(usuario_logado);
        return ok();
    }

    Equipe *eq = buscar_equipe_por_ra(usuario_logado->id);
    if (!eq) return erro(ERRO_LOGICA, "Erro ao localizar sua equipe.");

    int ra_novo;
    printf("\n--- ADICIONAR PARTICIPANTE ---\n");
    printf("Equipe: %s\n", eq->nome);
    printf("Digite o RA do participante que deseja adicionar: ");
    scanf("%d", &ra_novo);
    getchar();

    if (participante_ja_vinculado(ra_novo))
        return erro(ERRO_LOGICA, "Esse participante já está vinculado a uma equipe.");

    FILE *f = fopen("equipes.csv", "a");
    if (!f) return erro(ERRO_ARQUIVO, "Erro ao abrir equipes.csv para vincular participante.");

    fprintf(f, "%s,%d,%s,%d\n", eq->nome, eq->ra_criador, eq->nome_criador, ra_novo);
    fclose(f);

    printf("Participante RA %d adicionado à equipe '%s'.\n", ra_novo, eq->nome);
    return ok();
}

// --------------------------------------------------
// Listar equipes do participante
// --------------------------------------------------
void listar_equipes(User *usuario_logado) {
    if (!usuario_logado) {
        printf("Usuário não logado.\n");
        return;
    }

    FILE *f = abrir_csv("equipes.csv");
    if (!f) {
        printf("Nenhuma equipe cadastrada.\n");
        return;
    }

    char linha[256];
    fgets(linha, sizeof(linha), f);

    int encontrou = 0;
    char nome[100], nome_criador[50];
    int ra_criador, ra_vinculado;

    printf("\n--- SUAS EQUIPES ---\n");

    while (fgets(linha, sizeof(linha), f)) {
        int campos = sscanf(linha, "%99[^,],%d,%49[^,],%d", nome, &ra_criador, nome_criador, &ra_vinculado);

        if (campos >= 3 && (ra_criador == usuario_logado->id || ra_vinculado == usuario_logado->id)) {
            printf("Equipe: %s | Criador: %s (RA: %d)\n", nome, nome_criador, ra_criador);
            encontrou = 1;
        }
    }

    if (!encontrou)
        printf("Você não está vinculado a nenhuma equipe.\n");

    fclose(f);
}
