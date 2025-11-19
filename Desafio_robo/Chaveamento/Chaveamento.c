#include "Chaveamento.h"
#include "Result.h"
#include "Files.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Função auxiliar para verificar equipes
int existem_equipes_para_desafio(TipoDesafio tipo) {
    FILE *f_equipes = fopen("./dados/equipes.csv", "r");
    if (!f_equipes) return 0;
    
    char linha[512];
    fgets(linha, sizeof(linha), f_equipes); // Pular cabeçalho
    
    int count = 0;
    const char *tipo_busca = (tipo == SUMO) ? "Sumo" : "Seguidor";
    
    while (fgets(linha, sizeof(linha), f_equipes)) {
        int id;
        char nome[100], tipo_equipe[50], lider[100], participantes[500];
        
        if (sscanf(linha, "%d,%99[^,],%49[^,],%99[^,],%499[^\n]", 
                   &id, nome, tipo_equipe, lider, participantes) == 5) {
            if (strcmp(tipo_equipe, tipo_busca) == 0) {
                count++;
                if (count >= 2) {
                    fclose(f_equipes);
                    return 1;
                }
            }
        }
    }
    
    fclose(f_equipes);
    return 0;
}

Result gerar_chaveamento_persistente(TipoDesafio tipo) {
    // Verificar equipes primeiro
    if (!existem_equipes_para_desafio(tipo)) {
        return erro(ERRO_LOGICA, "Número insuficiente de equipes para gerar chaveamento (mínimo 2).");
    }

    FILE *f_equipes = fopen("./dados/equipes.csv", "r");
    if (!f_equipes) {
        return erro(ERRO_ARQUIVO, "Nenhuma equipe cadastrada.");
    }

    // Coletar equipes do tipo específico
    char linha[512];
    fgets(linha, sizeof(linha), f_equipes);
    
    int equipes[50];
    char nomes_equipes[50][100];
    int num_equipes = 0;
    
    while (fgets(linha, sizeof(linha), f_equipes) && num_equipes < 50) {
        int id;
        char nome[100], tipo_equipe[50], lider[100], participantes[500];
        if (sscanf(linha, "%d,%99[^,],%49[^,],%99[^,],%499[^\n]", 
                   &id, nome, tipo_equipe, lider, participantes) == 5) {
            
            if ((tipo == SUMO && strcmp(tipo_equipe, "Sumo") == 0) ||
                (tipo == SEGUIDOR_LINHA && strcmp(tipo_equipe, "Seguidor") == 0)) {
                equipes[num_equipes] = id;
                strcpy(nomes_equipes[num_equipes], nome);
                num_equipes++;
            }
        }
    }
    fclose(f_equipes);

    if (num_equipes < 2) {
        return erro(ERRO_LOGICA, "Número insuficiente de equipes para gerar chaveamento (mínimo 2).");
    }

    // Embaralhar equipes
    srand(time(NULL));
    for (int i = num_equipes - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp_id = equipes[i];
        equipes[i] = equipes[j];
        equipes[j] = temp_id;
        
        char temp_nome[100];
        strcpy(temp_nome, nomes_equipes[i]);
        strcpy(nomes_equipes[i], nomes_equipes[j]);
        strcpy(nomes_equipes[j], temp_nome);
    }

    // Gerar novo ID para chaveamento
    int novo_id = 1;
    FILE *f_chave_existente = fopen("./dados/chaveamento.csv", "r");
    if (f_chave_existente) {
        char linha[512];
        fgets(linha, sizeof(linha), f_chave_existente);
        while (fgets(linha, sizeof(linha), f_chave_existente)) {
            int id_existente;
            sscanf(linha, "%d,", &id_existente);
            if (id_existente >= novo_id) novo_id = id_existente + 1;
        }
        fclose(f_chave_existente);
    }

    // Salvar chaveamento
    FILE *f_chave_salvar = fopen("./dados/chaveamento.csv", "a");
    if (!f_chave_salvar) {
        return erro(ERRO_ARQUIVO, "Erro ao salvar chaveamento.");
    }
    if (ftell(f_chave_salvar) == 0) {
        fprintf(f_chave_salvar, "ID_CHAVEAMENTO,TIPO_DESAFIO,FINALIZADO\n");
    }
    fprintf(f_chave_salvar, "%d,%d,0\n", novo_id, tipo);
    fclose(f_chave_salvar);

    // Salvar confrontos
    FILE *f_confrontos = fopen("./dados/confrontos.csv", "a");
    if (!f_confrontos) {
        return erro(ERRO_ARQUIVO, "Erro ao salvar confrontos.");
    }
    if (ftell(f_confrontos) == 0) {
        fprintf(f_confrontos, "ID_CONFRONTO,ID_CHAVEAMENTO,ID_EQUIPE1,ID_EQUIPE2,"
                "NOME_EQUIPE1,NOME_EQUIPE2,ID_VENCEDOR,TEMPO_VENCEDOR,RODADA,STATUS\n");
    }

    int id_confronto = 1;
    int rodada = 1;
    
    // Encontrar o maior ID de confronto existente
    FILE *f_conf_existente = fopen("./dados/confrontos.csv", "r");
    if (f_conf_existente) {
        char linha[512];
        fgets(linha, sizeof(linha), f_conf_existente);
        while (fgets(linha, sizeof(linha), f_conf_existente)) {
            int id_conf;
            sscanf(linha, "%d,", &id_conf);
            if (id_conf >= id_confronto) id_confronto = id_conf + 1;
        }
        fclose(f_conf_existente);
    }
    
    // Primeira rodada
    for (int i = 0; i < num_equipes; i += 2) {
        if (i + 1 < num_equipes) {
            // CORREÇÃO: Ordem correta dos parâmetros
            fprintf(f_confrontos, "%d,%d,%d,%d,%s,%s,-1,0.0,%d,%d\n",
                    id_confronto,           // ID_CONFRONTO
                    novo_id,                // ID_CHAVEAMENTO  
                    equipes[i],             // ID_EQUIPE1
                    equipes[i+1],           // ID_EQUIPE2
                    nomes_equipes[i],       // NOME_EQUIPE1
                    nomes_equipes[i+1],     // NOME_EQUIPE2
                    rodada,                 // RODADA - CORREÇÃO: estava no lugar errado
                    AGUARDANDO);            // STATUS - CORREÇÃO: estava no lugar errado
        } else {
            // CORREÇÃO: Ordem correta dos parâmetros para BYE
            fprintf(f_confrontos, "%d,%d,%d,-1,%s,BYE,%d,0.0,%d,%d\n",
                    id_confronto,           // ID_CONFRONTO
                    novo_id,                // ID_CHAVEAMENTO
                    equipes[i],             // ID_EQUIPE1
                    nomes_equipes[i],       // NOME_EQUIPE1
                    equipes[i],             // ID_VENCEDOR (para BYE, a própria equipe vence)
                    rodada,                 // RODADA
                    FINALIZADO);            // STATUS
        }
        id_confronto++;
    }

    fclose(f_confrontos);
    
    return ok();
}

// APENAS UMA DEFINIÇÃO da função exibir_chaveamento - REMOVA A SEGUNDA!
void exibir_chaveamento(TipoDesafio tipo) {
    FILE *f_chave = fopen("./dados/chaveamento.csv", "r");
    if (!f_chave) {
        printf("Nenhum chaveamento encontrado.\n");
        return;
    }
    
    // Encontrar chaveamento ativo do tipo especificado
    char linha[512];
    fgets(linha, sizeof(linha), f_chave);
    
    int id_chaveamento = -1;
    while (fgets(linha, sizeof(linha), f_chave)) {
        int id_chave, tipo_chave, finalizado;
        if (sscanf(linha, "%d,%d,%d", &id_chave, &tipo_chave, &finalizado) == 3) {
            if (tipo_chave == tipo && !finalizado) {
                id_chaveamento = id_chave;
                break;
            }
        }
    }
    fclose(f_chave);
    
    if (id_chaveamento == -1) {
        printf("Nenhum chaveamento ativo encontrado para este tipo de desafio.\n");
        return;
    }
    
    // Carregar e exibir confrontos
    printf("\n=== CHAVEAMENTO - %s ===\n", 
           tipo == SUMO ? "ROBÔ SUMÔ" : "ROBÔ SEGUIDOR DE LINHA");
    printf("ID do Chaveamento: %d\n\n", id_chaveamento);
    
    FILE *f_confrontos = fopen("./dados/confrontos.csv", "r");
    if (!f_confrontos) {
        printf("Erro ao carregar confrontos.\n");
        return;
    }
    
    fgets(linha, sizeof(linha), f_confrontos);
    
    int rodada_atual = 1;
    int encontrou_confrontos = 0;
    
    // Primeiro, coletar todos os confrontos para organizar por rodada
    typedef struct {
        int id_confronto;
        int id_equipe1;
        int id_equipe2;
        int id_vencedor;
        float tempo;
        int rodada;
        int status;
        char nome_equipe1[100];
        char nome_equipe2[100];
    } ConfrontoInfo;
    
    ConfrontoInfo confrontos[50];
    int num_confrontos = 0;
    
    while (fgets(linha, sizeof(linha), f_confrontos) && num_confrontos < 50) {
        // Limpar a linha de quebras
        linha[strcspn(linha, "\n")] = 0;
        linha[strcspn(linha, "\r")] = 0;
        
        ConfrontoInfo *c = &confrontos[num_confrontos];
        
        // Parsing mais robusto
        if (sscanf(linha, "%d,%d,%d,%d,%99[^,],%99[^,],%d,%f,%d,%d",
                   &c->id_confronto, &c->id_equipe1, &c->id_equipe2, 
                   c->nome_equipe1, c->nome_equipe2, &c->id_vencedor, 
                   &c->tempo, &c->rodada, &c->status) >= 6) {
            
            // Verificar se pertence ao chaveamento correto
            // O segundo campo é o ID do chaveamento
            int id_chave_confronto;
            char *token = strtok(linha, ",");
            if (token) {
                id_chave_confronto = atoi(token); // ID_CONFRONTO
                token = strtok(NULL, ","); // ID_CHAVEAMENTO
                if (token) {
                    id_chave_confronto = atoi(token);
                    if (id_chave_confronto == id_chaveamento) {
                        num_confrontos++;
                        encontrou_confrontos = 1;
                    }
                }
            }
        }
    }
    fclose(f_confrontos);
    
    if (!encontrou_confrontos) {
        printf("Nenhum confronto encontrado para este chaveamento.\n");
        return;
    }
    
    // Agora exibir organizado por rodada
    int rodada_exibida = 1;
    printf("--- RODADA %d ---\n", rodada_exibida);
    
    for (int i = 0; i < num_confrontos; i++) {
        ConfrontoInfo *c = &confrontos[i];
        
        // Só mostrar confrontos da primeira rodada inicialmente
        if (c->rodada == 1) {
            printf("Confronto %d: %s vs %s", c->id_confronto, c->nome_equipe1, c->nome_equipe2);
            
            if (c->status == FINALIZADO && c->id_vencedor != -1) {
                printf(" → Vencedor: %s", 
                       c->id_vencedor == c->id_equipe1 ? c->nome_equipe1 : c->nome_equipe2);
                if (tipo == SEGUIDOR_LINHA && c->tempo > 0) {
                    printf(" (Tempo: %.2fs)", c->tempo);
                }
            } else if (c->id_equipe2 == -1) {
                printf(" → BYE (avança automaticamente)");
            } else {
                printf(" → Aguardando resultado");
            }
            printf("\n");
        }
    }
    
    printf("\n");
}

// Implementação completa de carregar_chaveamento_ativo
Chaveamento* carregar_chaveamento_ativo(TipoDesafio tipo) {
    FILE *f_chave = fopen("./dados/chaveamento.csv", "r");
    if (!f_chave) return NULL;

    // Primeiro, encontrar o ID do chaveamento ativo
    int id_chaveamento_ativo = -1;
    char linha[512];
    fgets(linha, sizeof(linha), f_chave); // Cabeçalho
    
    while (fgets(linha, sizeof(linha), f_chave)) {
        int id_chave, tipo_chave, finalizado;
        if (sscanf(linha, "%d,%d,%d", &id_chave, &tipo_chave, &finalizado) == 3) {
            if (tipo_chave == tipo && !finalizado) {
                id_chaveamento_ativo = id_chave;
                break;
            }
        }
    }
    fclose(f_chave);

    if (id_chaveamento_ativo == -1) return NULL;

    // Agora criar e preencher a estrutura Chaveamento
    Chaveamento *chave = malloc(sizeof(Chaveamento));
    if (!chave) return NULL;

    chave->id_chaveamento = id_chaveamento_ativo;
    chave->tipo_desafio = tipo;
    chave->finalizado = 0;
    chave->num_confrontos = 0;
    chave->rodada_atual = 1; // Sempre começa na rodada 1

    // Carregar confrontos
    FILE *f_confrontos = fopen("./dados/confrontos.csv", "r");
    if (f_confrontos) {
        fgets(linha, sizeof(linha), f_confrontos); // Cabeçalho
        
        while (fgets(linha, sizeof(linha), f_confrontos) && chave->num_confrontos < 32) {
            Confronto *c = &chave->confrontos[chave->num_confrontos];
            
            // Parsing manual mais seguro
            char *token = strtok(linha, ",");
            if (!token) continue;
            c->id_confronto = atoi(token);
            
            token = strtok(NULL, ",");
            if (!token) continue;
            int id_chave_confronto = atoi(token);
            
            // Só processar se pertencer ao chaveamento ativo
            if (id_chave_confronto == id_chaveamento_ativo) {
                token = strtok(NULL, ",");
                if (token) c->id_equipe1 = atoi(token);
                
                token = strtok(NULL, ",");
                if (token) c->id_equipe2 = atoi(token);
                
                token = strtok(NULL, ",");
                if (token) strncpy(c->nome_equipe1, token, sizeof(c->nome_equipe1)-1);
                
                token = strtok(NULL, ",");
                if (token) strncpy(c->nome_equipe2, token, sizeof(c->nome_equipe2)-1);
                
                token = strtok(NULL, ",");
                if (token) c->id_vencedor = atoi(token);
                
                token = strtok(NULL, ",");
                if (token) c->tempo_vencedor = atof(token);
                
                token = strtok(NULL, ",");
                if (token) c->rodada = atoi(token);
                
                token = strtok(NULL, ",");
                if (token) c->status = atoi(token);
                
                chave->num_confrontos++;
            }
        }
        fclose(f_confrontos);
    }

    return chave;
}

// Implementação completa de registrar_vencedor_confronto
Result registrar_vencedor_confronto(int id_confronto, int id_vencedor, float tempo) {
    FILE *f_confrontos = fopen("./dados/confrontos.csv", "r");
    if (!f_confrontos) {
        return erro(ERRO_ARQUIVO, "Arquivo de confrontos não encontrado.");
    }

    // Ler todos os confrontos para memória
    char **linhas = NULL;
    int num_linhas = 0;
    char linha[512];
    char buffer[512];
    
    // Ler cabeçalho
    fgets(linha, sizeof(linha), f_confrontos);
    linhas = malloc(sizeof(char*) * (num_linhas + 1));
    linhas[num_linhas] = malloc(strlen(linha) + 1);
    strcpy(linhas[num_linhas], linha);
    num_linhas++;

    int encontrado = 0;
    
    while (fgets(linha, sizeof(linha), f_confrontos)) {
        int id_conf, id_chave, id_eq1, id_eq2, id_venc, rodada, status;
        float tempo_venc;
        char nome_eq1[100], nome_eq2[100];
        
        if (sscanf(linha, "%d,%d,%d,%d,%99[^,],%99[^,],%d,%f,%d,%d",
                   &id_conf, &id_chave, &id_eq1, &id_eq2, 
                   nome_eq1, nome_eq2, &id_venc, &tempo_venc, &rodada, &status) == 10) {
            
            if (id_conf == id_confronto) {
                encontrado = 1;
                // Atualizar linha com vencedor
                snprintf(buffer, sizeof(buffer), "%d,%d,%d,%d,%s,%s,%d,%.2f,%d,%d\n",
                        id_confronto, id_chave, id_eq1, id_eq2, 
                        nome_eq1, nome_eq2, id_vencedor, tempo, rodada, FINALIZADO);
                
                linhas = realloc(linhas, sizeof(char*) * (num_linhas + 1));
                linhas[num_linhas] = malloc(strlen(buffer) + 1);
                strcpy(linhas[num_linhas], buffer);
            } else {
                // Manter linha original
                linhas = realloc(linhas, sizeof(char*) * (num_linhas + 1));
                linhas[num_linhas] = malloc(strlen(linha) + 1);
                strcpy(linhas[num_linhas], linha);
            }
        } else {
            // Manter linha original se não conseguir parsear
            linhas = realloc(linhas, sizeof(char*) * (num_linhas + 1));
            linhas[num_linhas] = malloc(strlen(linha) + 1);
            strcpy(linhas[num_linhas], linha);
        }
        num_linhas++;
    }
    fclose(f_confrontos);

    if (!encontrado) {
        // Liberar memória
        for (int i = 0; i < num_linhas; i++) free(linhas[i]);
        free(linhas);
        return erro(ERRO_INVALIDO, "Confronto não encontrado.");
    }

    // Reescrever arquivo
    FILE *f_out = fopen("./dados/confrontos.csv", "w");
    if (!f_out) {
        for (int i = 0; i < num_linhas; i++) free(linhas[i]);
        free(linhas);
        return erro(ERRO_ARQUIVO, "Erro ao salvar confrontos atualizados.");
    }

    for (int i = 0; i < num_linhas; i++) {
        fprintf(f_out, "%s", linhas[i]);
        free(linhas[i]);
    }
    free(linhas);
    fclose(f_out);

    return ok();
}