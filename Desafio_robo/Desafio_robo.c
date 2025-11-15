#include "Desafio_robo.h"
#include "Pontuacao.h"
#include "Files.h"
#include "Chaveamento.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Inicializa um desafio (só mostra as regras básicas)
Result iniciar_desafio(TipoDesafio tipo) {
    printf("\n===== INICIANDO DESAFIO =====\n");
    if (tipo == SUMO) {
        printf("Robô SUMÔ: confronto direto. Vitória = 100 pontos.\n");
        printf("Formato: Eliminação direta - cada vitória avança para a próxima rodada\n");
    } else {
        printf("Robô SEGUIDOR DE LINHA: menor tempo = maior pontuação.\n");
        printf("Pontuação: 1000 / tempo (em segundos)\n");
    }

    return ok();
}

// Registra um resultado de desafio individual
Result registrar_resultado(Resultado_Desafio *res) {
    if (!res) return erro(ERRO_INVALIDO, "Resultado inválido");

    // ID e nome da equipe
    if (res->id_equipe <= 0) {
        printf("Digite o ID da equipe: ");
        scanf("%d", &res->id_equipe);
        getchar();
    }

    if (strlen(res->nome_equipe) == 0) {
        printf("Digite o nome da equipe: ");
        fgets(res->nome_equipe, sizeof(res->nome_equipe), stdin);
        res->nome_equipe[strcspn(res->nome_equipe, "\n")] = '\0';
    }

    const char *tipo_str = (res->tipo == SUMO) ? "Sumo" : "Seguidor";

    // Leitura e cálculo de pontuação
    if (res->tipo == SEGUIDOR_LINHA) {
        printf("Tempo de execução (segundos): ");
        scanf("%f", &res->tempo_execucao);
        getchar();
        res->pontuacao = (int)(1000 / res->tempo_execucao);
    } else {
        res->tempo_execucao = 0.0f;
        res->pontuacao = 100;
    }

    // Registra persistente na tabela de resultados
    registrar_pontuacao_para_equipe(res->id_equipe, res->nome_equipe, tipo_str, res->tempo_execucao, res->pontuacao);

    printf("Resultado registrado com sucesso!\n");
    return ok();
}

// Gera chaveamento usando o sistema persistente
void gerar_chaveamento(TipoDesafio tipo) {
    Result r = gerar_chaveamento_persistente(tipo);
    if (r.code == OK) {
        printf("Chaveamento gerado e salvo com sucesso!\n");
        exibir_chaveamento(tipo);
    } else {
        return;
    }
}

// Mostra as pontuações registradas para um tipo de desafio
void calcular_pontuacoes(TipoDesafio tipo) {
    FILE *f = abrir_csv("resultados_robo.csv");
    if (!f) {
        printf("Nenhum resultado registrado ainda.\n");
        return;
    }

    char linha[512];
    fgets(linha, sizeof(linha), f);

    int id, pontos;
    float tempo;
    char nome[100], tipo_csv[50];

    printf("\n--- PONTUAÇÕES (%s) ---\n", tipo == SUMO ? "SUMO" : "SEGUIDOR DE LINHA");

    while (fgets(linha, sizeof(linha), f)) {
        sscanf(linha, "%d,%99[^,],%49[^,],%f,%d", &id, nome, tipo_csv, &tempo, &pontos);
        if ((tipo == SUMO && strcmp(tipo_csv, "Sumo") == 0) ||
            (tipo == SEGUIDOR_LINHA && strstr(tipo_csv, "Seguidor"))) {
            printf("ID %d | %-25s | Pontos: %4d | Tempo: %.2fs\n",
                   id, nome, pontos, tempo);
        }
    }

    fclose(f);
    printf("Cálculo de pontuações concluído!\n");
}

// Registra vencedor de um confronto específico
Result registrar_vencedor_desafio(TipoDesafio tipo) {
    Chaveamento *chave = carregar_chaveamento_ativo(tipo);
    if (!chave) {
        return erro(ERRO_LOGICA, "Nenhum chaveamento ativo encontrado. Gere um chaveamento primeiro.");
    }

    printf("\n=== REGISTRAR VENCEDOR - %s ===\n", 
           tipo == SUMO ? "ROBÔ SUMÔ" : "ROBÔ SEGUIDOR DE LINHA");
    
    // Mostrar confrontos pendentes da rodada atual
    printf("Confrontos da Rodada %d (Pendentes):\n", chave->rodada_atual);
    int confrontos_pendentes = 0;
    
    for (int i = 0; i < chave->num_confrontos; i++) {
        Confronto *c = &chave->confrontos[i];
        if (c->rodada == chave->rodada_atual && c->status != FINALIZADO && c->id_equipe2 != -1) {
            printf("[%d] %s vs %s\n", c->id_confronto, c->nome_equipe1, c->nome_equipe2);
            confrontos_pendentes++;
        }
    }

    if (confrontos_pendentes == 0) {
        free(chave);
        return erro(ERRO_LOGICA, "Nenhum confronto pendente nesta rodada.");
    }

    printf("\nID do confronto: ");
    int id_confronto;
    scanf("%d", &id_confronto);
    getchar();

    // Encontrar confronto
    Confronto *confronto = NULL;
    for (int i = 0; i < chave->num_confrontos; i++) {
        if (chave->confrontos[i].id_confronto == id_confronto) {
            confronto = &chave->confrontos[i];
            break;
        }
    }

    if (!confronto || confronto->status == FINALIZADO || confronto->rodada != chave->rodada_atual) {
        free(chave);
        return erro(ERRO_INVALIDO, "Confronto inválido, já finalizado ou não pertence à rodada atual.");
    }

    printf("Vencedor do confronto %d:\n", id_confronto);
    printf("1 - %s\n", confronto->nome_equipe1);
    printf("2 - %s\n", confronto->nome_equipe2);
    printf("Escolha: ");
    
    int opc_vencedor;
    scanf("%d", &opc_vencedor);
    getchar();

    int id_vencedor;
    float tempo = 0.0;

    if (opc_vencedor == 1) {
        id_vencedor = confronto->id_equipe1;
    } else if (opc_vencedor == 2) {
        id_vencedor = confronto->id_equipe2;
    } else {
        free(chave);
        return erro(ERRO_INVALIDO, "Opção inválida.");
    }

    // Para seguidor de linha, capturar tempo
    if (tipo == SEGUIDOR_LINHA) {
        printf("Tempo do vencedor (segundos): ");
        scanf("%f", &tempo);
        getchar();
        
        if (tempo <= 0) {
            free(chave);
            return erro(ERRO_INVALIDO, "Tempo deve ser maior que zero.");
        }
    }

    // Registrar vencedor no arquivo
    Result r = registrar_vencedor_confronto(id_confronto, id_vencedor, tempo);
    
    if (r.code == OK) {
        printf("Vencedor registrado com sucesso!\n");
        
        // Verificar se é a final (todos os confrontos da rodada finalizados)
        int todos_finalizados = 1;
        for (int i = 0; i < chave->num_confrontos; i++) {
            if (chave->confrontos[i].rodada == chave->rodada_atual && 
                chave->confrontos[i].status != FINALIZADO) {
                todos_finalizados = 0;
                break;
            }
        }

        if (todos_finalizados) {
            printf("\n=== FIM DO DESAFIO ===\n");
            printf("Campeão: %s\n", 
                   id_vencedor == confronto->id_equipe1 ? 
                   confronto->nome_equipe1 : confronto->nome_equipe2);
            
            // Registrar pontuação final para o campeão
            Resultado_Desafio res_final;
            res_final.id_equipe = id_vencedor;
            strcpy(res_final.nome_equipe, 
                   id_vencedor == confronto->id_equipe1 ? 
                   confronto->nome_equipe1 : confronto->nome_equipe2);
            res_final.tipo = tipo;
            res_final.tempo_execucao = tempo;
            res_final.pontuacao = (tipo == SUMO) ? 100 : (int)(1000 / tempo);
            
            registrar_resultado(&res_final);
            printf("Pontuação final registrada: %d pontos\n", res_final.pontuacao);
        }
    }

    free(chave);
    return r;
}

// Exibe chaveamento formatado
void exibir_chaveamento_desafio(TipoDesafio tipo) {
    exibir_chaveamento(tipo);
}