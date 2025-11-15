#include "Chaveamento.h"
#include "Result.h"
#include "Files.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Implementação das funções de chaveamento...
Result gerar_chaveamento_persistente(TipoDesafio tipo) {
    FILE *f_equipes = abrir_csv("equipes.csv");
    if (!f_equipes) {
        return erro(ERRO_ARQUIVO, "Nenhuma equipe cadastrada.");
    }

    // Verificar se já existe chaveamento ativo para este tipo
    FILE *f_chave = abrir_csv("chaveamento.csv");
    if (f_chave) {
        char linha[512];
        fgets(linha, sizeof(linha), f_chave);
        while (fgets(linha, sizeof(linha), f_chave)) {
            int id_chave, tipo_existente, finalizado;
            if (sscanf(linha, "%d,%d,%d", &id_chave, &tipo_existente, &finalizado) == 3) {
                if (tipo_existente == tipo && !finalizado) {
                    fclose(f_chave);
                    fclose(f_equipes);
                    return erro(ERRO_LOGICA, "Já existe um chaveamento ativo para este tipo de desafio.");
                }
            }
        }
        fclose(f_chave);
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
            
            // Verificar se a equipe é do tipo correto
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

    // Embaralhar equipes para sorteio
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

    // Gerar novo ID de chaveamento
    int novo_id = 1;
    f_chave = abrir_csv("chaveamento.csv");
    if (f_chave) {
        char linha[512];
        fgets(linha, sizeof(linha), f_chave);
        while (fgets(linha, sizeof(linha), f_chave)) {
            int id_existente;
            sscanf(linha, "%d,", &id_existente);
            if (id_existente >= novo_id) novo_id = id_existente + 1;
        }
        fclose(f_chave);
    }

    // Salvar chaveamento
    FILE *f_chave_salvar = escrever_no_csv("chaveamento.csv", 
                                         "ID_CHAVEAMENTO,TIPO_DESAFIO,FINALIZADO\n");
    if (!f_chave_salvar) {
        return erro(ERRO_ARQUIVO, "Erro ao salvar chaveamento.");
    }
    fprintf(f_chave_salvar, "%d,%d,0\n", novo_id, tipo);
    fclose(f_chave_salvar);

    // Salvar confrontos
    FILE *f_confrontos = escrever_no_csv("confrontos.csv", 
                                       "ID_CONFRONTO,ID_CHAVEAMENTO,ID_EQUIPE1,ID_EQUIPE2,"
                                       "NOME_EQUIPE1,NOME_EQUIPE2,ID_VENCEDOR,TEMPO_VENCEDOR,RODADA,STATUS\n");
    if (!f_confrontos) {
        return erro(ERRO_ARQUIVO, "Erro ao salvar confrontos.");
    }

    int id_confronto = 1;
    int rodada = 1;
    
    // Primeira rodada - todos os confrontos iniciais
    for (int i = 0; i < num_equipes; i += 2) {
        if (i + 1 < num_equipes) {
            fprintf(f_confrontos, "%d,%d,%d,%d,%s,%s,-1,0.0,%d,%d\n",
                    id_confronto++, novo_id, equipes[i], equipes[i+1],
                    nomes_equipes[i], nomes_equipes[i+1], rodada, AGUARDANDO);
        } else {
            // Equipe recebe bye (avança automaticamente)
            fprintf(f_confrontos, "%d,%d,%d,-1,%s,BYE,%d,0.0,%d,%d\n",
                    id_confronto++, novo_id, equipes[i],
                    nomes_equipes[i], equipes[i], rodada, FINALIZADO);
        }
    }

    fclose(f_confrontos);
    
    printf("Chaveamento gerado com sucesso! ID: %d\n", novo_id);
    printf("Número de equipes no chaveamento: %d\n", num_equipes);
    
    return ok();
}

Result registrar_vencedor_confronto(int id_confronto, int id_vencedor, float tempo) {
    // Implementação para registrar vencedor...
    printf("Registrando vencedor do confronto %d: equipe %d com tempo %.2f\n", 
           id_confronto, id_vencedor, tempo);
    return ok();
}

void exibir_chaveamento(TipoDesafio tipo) {
    // Implementação para exibir chaveamento...
    printf("Exibindo chaveamento para o tipo: %d\n", tipo);
}

Chaveamento* carregar_chaveamento_ativo(TipoDesafio tipo) {
    // Implementação para carregar chaveamento...
    printf("Carregando chaveamento ativo para tipo: %d\n", tipo);
    return NULL;
}