    #include "Cadastro_Equipes.h"
    #include "Files.h"
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    // ============================================================================
    // FUNÇÕES INTERNAS (não exportadas no .h)
    // ============================================================================

    // Verifica se uma equipe já existe
    static int equipe_existe(const char *nome_equipe) {
        FILE *f = abrir_csv("equipes.csv");
        if (!f) return 0;

        char linha[512], nome[100];
        fgets(linha, sizeof(linha), f);
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

    // Verifica se participante já está em alguma equipe
    static int participante_em_alguma_equipe(User *participante) {
        if (!participante) return 0;
        
        FILE *f = abrir_csv("equipes.csv");
        if (!f) return 0;

        char linha[512];
        fgets(linha, sizeof(linha), f);
        
        char identificador[60];
        snprintf(identificador, sizeof(identificador), "%s:%d", participante->nome, participante->RA);
        
        while (fgets(linha, sizeof(linha), f)) {
            if (strstr(linha, identificador)) {
                fclose(f);
                return 1;
            }
        }
        fclose(f);
        return 0;
    }

    // Cria identificador único do participante (Nome:RA)
    static void criar_identificador_participante(char *destino, size_t tamanho, User *participante) {
        snprintf(destino, tamanho, "%s:%d", participante->nome, participante->RA);
    }

    // Formata a exibição dos participantes para melhor legibilidade
    static void formatar_exibicao_participantes(const char *participantes_csv) {
        if (!participantes_csv || strlen(participantes_csv) == 0) {
            printf("  Nenhum participante\n");
            return;
        }

        char participantes[500];
        strcpy(participantes, participantes_csv);
        
        char *token = strtok(participantes, ";");
        int count = 1;
        
        while (token) {
            char nome[50];
            int ra;
            if (sscanf(token, "%49[^:]:%d", nome, &ra) == 2) {
                printf("  %d. %s (RA: %d)\n", count, nome, ra);
            }
            count++;
            token = strtok(NULL, ";");
        }
    }

    // Função interna para processar arquivo de equipes (usada por várias funções)
    static Result processar_arquivo_equipes(const char *nome_equipe, User *participante, int operacao) {
        // operacao: 1 = adicionar, 2 = remover
        
        FILE *f = abrir_csv("equipes.csv");
        if (!f) return erro(ERRO_ARQUIVO, "Arquivo de equipes inexistente.");

        char linha[512];
        fgets(linha, sizeof(linha), f);

        FILE *temp = fopen("./dados/temp.csv", "w");
        fprintf(temp, "ID_EQUIPE,NOME_EQUIPE,LIDER,PARTICIPANTES\n");

        int modificado = 0;
        int id;
        char nome[100], lider[100], membros[500];

        char identificador_participante[60];
        if (participante) {
            criar_identificador_participante(identificador_participante, sizeof(identificador_participante), participante);
        }

        while (fgets(linha, sizeof(linha), f)) {
            sscanf(linha, "%d,%99[^,],%99[^,],%499[^\n]", &id, nome, lider, membros);

            if (strcmp(nome, nome_equipe) == 0) {
                if (operacao == 1) {
                    // Adicionar participante
                    strcat(membros, ";");
                    strcat(membros, identificador_participante);
                    modificado = 1;
                } else if (operacao == 2) {
                    // Remover participante
                    char buffer[500] = "";
                    char *token = strtok(membros, ";");
                    int primeiro = 1;
                    
                    while (token) {
                        if (strcmp(token, identificador_participante) != 0) {
                            if (!primeiro) strcat(buffer, ";");
                            strcat(buffer, token);
                            primeiro = 0;
                        } else {
                            modificado = 1;
                        }
                        token = strtok(NULL, ";");
                    }
                    strcpy(membros, buffer);
                }
            }
            fprintf(temp, "%d,%s,%s,%s\n", id, nome, lider, membros);
        }

        fclose(f);
        fclose(temp);

        if (modificado) {
            remove("./dados/equipes.csv");
            rename("./dados/temp.csv", "./dados/equipes.csv");
            return ok();
        } else {
            remove("./dados/temp.csv");
            return erro(ERRO_LOGICA, operacao == 1 ? "Equipe não encontrada." : "Participante não encontrado na equipe.");
        }
    }

    // ============================================================================
    // FUNÇÕES PRINCIPAIS (exportadas no .h)
    // ============================================================================

    // Cadastra uma nova equipe
    Result cadastrar_equipe(User *lider) {
        if (!lider) return erro(ERRO_INVALIDO, "Usuário inválido");

        criar_diretorio_dados();

        // Verifica se o líder já tem equipe
        if (participante_em_alguma_equipe(lider))
            return erro(ERRO_LOGICA, "Você já está em uma equipe.");

        Equipe e;
        e.id_equipe = (int)contar_linhas("equipes.csv");
        strcpy(e.nome_lider, lider->nome);

        printf("\n--- Cadastro de Equipe ---\n");
        printf("Nome da equipe: ");
        fgets(e.nome_equipe, sizeof(e.nome_equipe), stdin);
        e.nome_equipe[strcspn(e.nome_equipe, "\n")] = '\0';

        if (equipe_existe(e.nome_equipe))
            return erro(ERRO_LOGICA, "Já existe uma equipe com esse nome.");

        // Líder é o primeiro integrante
        criar_identificador_participante(e.participantes, sizeof(e.participantes), lider);

        FILE *f = escrever_no_csv("equipes.csv", "ID_EQUIPE,NOME_EQUIPE,LIDER,PARTICIPANTES\n");
        if (!f) return erro(ERRO_ARQUIVO, "Erro ao abrir arquivo de equipes.");

        fprintf(f, "%d,%s,%s,%s\n", e.id_equipe, e.nome_equipe, e.nome_lider, e.participantes);
        fclose(f);

        printf("Equipe '%s' criada com sucesso!\n", e.nome_equipe);
        return ok();
    }

    // Adiciona um participante à equipe
    Result adicionar_participante_equipe(const char *nome_equipe, User *participante) {
        if (!nome_equipe || !participante)
            return erro(ERRO_INVALIDO, "Parâmetros inválidos.");

        if (participante_em_alguma_equipe(participante))
            return erro(ERRO_LOGICA, "Participante já está em uma equipe.");

        return processar_arquivo_equipes(nome_equipe, participante, 1);
    }

    // Remove um participante da equipe
    Result remover_participante_equipe(const char *nome_equipe, User *participante) {
        if (!nome_equipe || !participante)
            return erro(ERRO_INVALIDO, "Parâmetros inválidos.");

        return processar_arquivo_equipes(nome_equipe, participante, 2);
    }

    // Lista todas as equipes
    void listar_equipes() {
        FILE *f = abrir_csv("equipes.csv");
        if (!f) {
            printf("Nenhuma equipe cadastrada.\n");
            return;
        }

        char linha[512];
        fgets(linha, sizeof(linha), f);
        printf("\n=== LISTA DE EQUIPES ===\n");

        int id;
        char nome[100], lider[100], membros[500];
        int count = 0;

        while (fgets(linha, sizeof(linha), f)) {
            sscanf(linha, "%d,%99[^,],%99[^,],%499[^\n]", &id, nome, lider, membros);
            
            printf("\n────────────────────────────────────────\n");
            printf("ID: %d\n", id);
            printf("Nome: %s\n", nome);
            printf("Líder: %s\n", lider);
            formatar_exibicao_participantes(membros);
            count++;
        }

        if (count == 0) {
            printf("\nNenhuma equipe cadastrada.\n");
        } else {
            printf("────────────────────────────────────────\n");
            printf("Total de equipes: %d\n", count);
        }

        fclose(f);
    }

    // Exibe a equipe e integrantes do usuário logado
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
        fgets(linha, sizeof(linha), f);

        int encontrado = 0;
        int id;
        char nome[100], lider[100], membros[500];

        char identificador_usuario[60];
        criar_identificador_participante(identificador_usuario, sizeof(identificador_usuario), usuario);

        while (fgets(linha, sizeof(linha), f)) {
            sscanf(linha, "%d,%99[^,],%99[^,],%499[^\n]", &id, nome, lider, membros);
            if (strstr(membros, identificador_usuario)) {
                printf("\n=== SUA EQUIPE ===\n");
                printf("────────────────────────────────────────\n");
                printf("ID: %d\n", id);
                printf("Nome: %s\n", nome);
                printf("Líder: %s\n", lider);
                formatar_exibicao_participantes(membros);
                printf("────────────────────────────────────────\n");
                encontrado = 1;
                break;
            }
        }

        fclose(f);

        if (!encontrado)
            printf("\nVocê ainda não está em nenhuma equipe.\n");
    }