#include "Cadastro_Equipes.h"
#include "Files.h"

// ------------------ Gerar novo ID automático ------------------
int gerar_novo_id_equipe() {
    FILE *f = abrir_csv("equipes.csv");
    if (!f) return 1;

    char linha[512];
    int id = 0, id_temp;
    fgets(linha, sizeof(linha), f); // pula cabeçalho

    while (fgets(linha, sizeof(linha), f)) {
        if (sscanf(linha, "%d,", &id_temp) == 1 && id_temp > id)
            id = id_temp;
    }
    fclose(f);
    return id + 1;
}

// ------------------ Verificar se participante já está vinculado ------------------
int participante_ja_vinculado(const char *nome) {
    FILE *f = abrir_csv("equipes.csv");
    if (!f) return 0;

    char linha[512];
    fgets(linha, sizeof(linha), f); // pula cabeçalho

    while (fgets(linha, sizeof(linha), f)) {
        char nomes[200];
        if (sscanf(linha, "%*d,%*[^,],%*[^,],%199[^\n]", nomes) == 1) {
            if (strstr(nomes, nome)) {
                fclose(f);
                return 1;
            }
        }
    }
    fclose(f);
    return 0;
}

// ------------------ Verificar se usuário é criador ------------------
int criador_da_equipe(const char *nome) {
    FILE *f = abrir_csv("equipes.csv");
    if (!f) return 0;

    char linha[512], nome_criador[50];
    fgets(linha, sizeof(linha), f); // pula cabeçalho

    while (fgets(linha, sizeof(linha), f)) {
        if (sscanf(linha, "%*d,%*[^,],%49[^,]", nome_criador) == 1) {
            if (strcmp(nome_criador, nome) == 0) {
                fclose(f);
                return 1;
            }
        }
    }
    fclose(f);
    return 0;
}

// ------------------ Cadastrar nova equipe ------------------
Result cadastrar_equipe(User *usuario_logado) {
    if (!usuario_logado)
        return erro(ERRO_LOGICA, "Usuário não logado.");

    if (usuario_logado->cargo != PARTICIPANTE)
        return erro(ERRO_PERMISSAO, "Somente PARTICIPANTES podem cadastrar equipes.");

    if (participante_ja_vinculado(usuario_logado->nome))
        return erro(ERRO_LOGICA, "Você já faz parte de uma equipe.");

    char nome_eq[100];
    printf("\nNome da equipe: ");
    fgets(nome_eq, sizeof(nome_eq), stdin);
    nome_eq[strcspn(nome_eq, "\n")] = '\0';

    // Verifica duplicidade de nome
    FILE *fcheck = abrir_csv("equipes.csv");
    if (fcheck) {
        char linha[512], nome_existente[100];
        fgets(linha, sizeof(linha), fcheck);
        while (fgets(linha, sizeof(linha), fcheck)) {
            sscanf(linha, "%*d,%99[^,]", nome_existente);
            if (strcmp(nome_existente, nome_eq) == 0) {
                fclose(fcheck);
                return erro(ERRO_LOGICA, "Já existe uma equipe com esse nome.");
            }
        }
        fclose(fcheck);
    }

    int novo_id = gerar_novo_id_equipe();

    FILE *f = escrever_no_csv("equipes.csv", "ID_EQUIPE,NOME_EQUIPE,NOME_CRIADOR,NOMES_PARTICIPANTES\n");
    if (!f) return erro(ERRO_ARQUIVO, "Erro ao abrir equipes.csv");

    fprintf(f, "%d,%s,%s,%s\n", novo_id, nome_eq, usuario_logado->nome, usuario_logado->nome);
    fclose(f);

    printf("\nEquipe '%s' cadastrada com sucesso! (ID: %d)\n", nome_eq, novo_id);
    printf("Você é o líder e está automaticamente adicionado à equipe.\n");
    printf("Apenas você poderá adicionar ou remover membros desta equipe.\n");

    return ok();
}

// ------------------ Adicionar participante ------------------
Result adicionar_participante(User *usuario_logado) {
    if (!criador_da_equipe(usuario_logado->nome))
        return erro(ERRO_PERMISSAO, "Somente o líder da equipe pode adicionar membros.");

    char nome_part[50];
    printf("Digite o nome do participante que deseja adicionar: ");
    fgets(nome_part, sizeof(nome_part), stdin);
    nome_part[strcspn(nome_part, "\n")] = '\0';

    if (participante_ja_vinculado(nome_part))
        return erro(ERRO_LOGICA, "Este participante já faz parte de uma equipe.");

    FILE *f = abrir_csv("equipes.csv");
    if (!f) return erro(ERRO_ARQUIVO, "Erro ao abrir equipes.csv para leitura.");

    FILE *temp = fopen("./dados/temp.csv", "w");
    if (!temp) {
        fclose(f);
        return erro(ERRO_ARQUIVO, "Erro ao criar arquivo temporário.");
    }

    char linha[512];
    fgets(linha, sizeof(linha), f);
    fprintf(temp, "%s", linha); // cabeçalho

    while (fgets(linha, sizeof(linha), f)) {
        int id_eq;
        char nome_eq[100], nome_criador[50], nomes[200];
        sscanf(linha, "%d,%99[^,],%49[^,],%199[^\n]", &id_eq, nome_eq, nome_criador, nomes);

        if (strcmp(nome_criador, usuario_logado->nome) == 0) {
            strcat(nomes, "|");
            strcat(nomes, nome_part);
            fprintf(temp, "%d,%s,%s,%s\n", id_eq, nome_eq, nome_criador, nomes);
        } else {
            fprintf(temp, "%s", linha);
        }
    }

    fclose(f);
    fclose(temp);
    remove("./dados/equipes.csv");
    rename("./dados/temp.csv", "./dados/equipes.csv");

    printf("Participante '%s' adicionado à equipe com sucesso!\n", nome_part);
    return ok();
}

// ------------------ Remover participante ------------------
Result remover_participante(User *usuario_logado) {
    if (!criador_da_equipe(usuario_logado->nome))
        return erro(ERRO_PERMISSAO, "Somente o líder da equipe pode remover membros.");

    char nome_rem[50];
    printf("Digite o nome do participante que deseja remover: ");
    fgets(nome_rem, sizeof(nome_rem), stdin);
    nome_rem[strcspn(nome_rem, "\n")] = '\0';

    FILE *f = abrir_csv("equipes.csv");
    if (!f) return erro(ERRO_ARQUIVO, "Erro ao abrir equipes.csv para leitura.");

    FILE *temp = fopen("./dados/temp.csv", "w");
    if (!temp) {
        fclose(f);
        return erro(ERRO_ARQUIVO, "Erro ao criar arquivo temporário.");
    }

    char linha[512];
    fgets(linha, sizeof(linha), f);
    fprintf(temp, "%s", linha); // cabeçalho

    int encontrado = 0;
    while (fgets(linha, sizeof(linha), f)) {
        int id_eq;
        char nome_eq[100], nome_criador[50], nomes[200];
        sscanf(linha, "%d,%99[^,],%49[^,],%199[^\n]", &id_eq, nome_eq, nome_criador, nomes);

        if (strcmp(nome_criador, usuario_logado->nome) == 0) {
            char novos_nomes[200] = "";
            char *token = strtok(nomes, "|");
            while (token) {
                if (strcmp(token, nome_rem) != 0) {
                    strcat(novos_nomes, token);
                    strcat(novos_nomes, "|");
                } else encontrado = 1;
                token = strtok(NULL, "|");
            }
            if (novos_nomes[strlen(novos_nomes) - 1] == '|')
                novos_nomes[strlen(novos_nomes) - 1] = '\0';
            fprintf(temp, "%d,%s,%s,%s\n", id_eq, nome_eq, nome_criador, novos_nomes);
        } else {
            fprintf(temp, "%s", linha);
        }
    }

    fclose(f);
    fclose(temp);
    remove("./dados/equipes.csv");
    rename("./dados/temp.csv", "./dados/equipes.csv");

    if (encontrado)
        printf("Participante '%s' removido com sucesso!\n", nome_rem);
    else
        printf("Participante '%s' não encontrado na equipe.\n", nome_rem);

    return ok();
}

// ------------------ Exibir equipe do participante ------------------
void exibir_equipe_do_participante(User *usuario_logado) {
    FILE *f = abrir_csv("equipes.csv");
    if (!f) {
        printf("Nenhuma equipe cadastrada.\n");
        return;
    }

    char linha[512];
    fgets(linha, sizeof(linha), f);

    int encontrado = 0;
    while (fgets(linha, sizeof(linha), f)) {
        int id_eq;
        char nome_eq[100], nome_criador[50], nomes_part[200];
        if (sscanf(linha, "%d,%99[^,],%49[^,],%199[^\n]",
                   &id_eq, nome_eq, nome_criador, nomes_part) == 4) {

            if (strstr(nomes_part, usuario_logado->nome)) {
                printf("\n--- EQUIPE: %s (ID %d) ---\n", nome_eq, id_eq);
                printf("Líder: %s\n", nome_criador);
                printf("Integrantes: %s\n", nomes_part);
                encontrado = 1;
                break;
            }
        }
    }

    fclose(f);
    if (!encontrado)
        printf("Você ainda não está vinculado a nenhuma equipe.\n");
}

// ------------------ Listar todas as equipes ------------------
void listar_equipes() {
    FILE *f = abrir_csv("equipes.csv");
    if (!f) {
        printf("Nenhuma equipe cadastrada.\n");
        return;
    }

    char linha[512];
    fgets(linha, sizeof(linha), f);

    printf("\n========= LISTA DE EQUIPES =========\n");
    while (fgets(linha, sizeof(linha), f)) {
        int id_eq;
        char nome_eq[100], nome_criador[50], nomes_part[200];
        if (sscanf(linha, "%d,%99[^,],%49[^,],%199[^\n]",
                   &id_eq, nome_eq, nome_criador, nomes_part) == 4) {
            printf("\nID: %d\nEquipe: %s\nLíder: %s\nIntegrantes: %s\n",
                   id_eq, nome_eq, nome_criador, nomes_part);
        }
    }
    printf("====================================\n");

    fclose(f);
}