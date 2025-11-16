#include "Menu_Avaliador.h"
#include "Desafio_robo.h"
#include "Cadastro_Equipes.h"
#include "Ranking.h"
#include "pausar_sistema.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>


// Implementação do desafio Sumô usando funções reais
void executar_desafio_sumo(void) {
    printf("\n=== EXECUTANDO DESAFIO ROBÔ SUMÔ ===\n");
    
    // 1. Gerar chaveamento
    printf("1. Gerando chaveamento...\n");
    gerar_chaveamento(SUMO);
    pausar_sistema();
    
    // 2. Executar rodadas
    int rodada = 1;
    int desafio_ativo = 1;
    
    while (desafio_ativo) {
        printf("\n--- RODADA %d ---\n", rodada);
        
        // Mostrar chaveamento atual
        exibir_chaveamento(SUMO);
        
        // Registrar resultados dos confrontos
        printf("\nRegistrar resultados da rodada %d:\n", rodada);
        Result r = registrar_vencedor_desafio(SUMO);
        
        if (r.code != OK) {
            printf("Erro ao registrar vencedor: %s\n", r.msg);
            printf("Deseja continuar? (s/n): ");
            char resp;
            scanf(" %c", &resp);
            getchar();
            if (resp != 's' && resp != 'S') {
                break;
            }
        } else {
            printf("Vencedor registrado com sucesso!\n");
        }
        
        // Verificar se o desafio terminou (implementação básica)
        printf("Deseja continuar para próxima rodada? (s/n): ");
        char continuar;
        scanf(" %c", &continuar);
        getchar();
        
        if (continuar != 's' && continuar != 'S') {
            desafio_ativo = 0;
            printf("\n=== DESAFIO INTERROMPIDO ===\n");
        }
        
        rodada++;
        pausar_sistema();
    }
    
    printf("\n=== DESAFIO SUMÔ CONCLUÍDO ===\n");
    printf("Ranking atualizado disponível no menu de rankings.\n");
}

// Implementação do desafio Seguidor usando funções reais
void executar_desafio_seguidor(void) {
    printf("\n=== EXECUTANDO DESAFIO ROBÔ SEGUIDOR DE LINHA ===\n");
    
    // 1. Preparar competição
    printf("1. Preparando competição...\n");
    gerar_chaveamento(SEGUIDOR_LINHA);
    pausar_sistema();
    
    // 2. Registrar tempos das equipes
    printf("\n2. Registrar tempos das equipes:\n");
    
    int continuar_registro = 1;
    while (continuar_registro) {
        Resultado_Desafio res = {0};
        res.tipo = SEGUIDOR_LINHA;
        
        printf("\n--- Novo Registro ---\n");
        
        // Coletar dados da equipe
        printf("ID da equipe: ");
        scanf("%d", &res.id_equipe);
        getchar();
        
        printf("Nome da equipe: ");
        fgets(res.nome_equipe, sizeof(res.nome_equipe), stdin);
        res.nome_equipe[strcspn(res.nome_equipe, "\n")] = '\0';
        
        printf("Tempo de execução (segundos): ");
        scanf("%f", &res.tempo_execucao);
        getchar();
        
        // Registrar resultado
        Result r = registrar_resultado(&res);
        if (r.code == OK) {
            printf("Tempo registrado: %.2fs para %s\n", res.tempo_execucao, res.nome_equipe);
            printf("  Pontuação calculada: %d pontos\n", res.pontuacao);
        } else {
            printf("Erro: %s\n", r.msg);
        }
        
        // Perguntar se quer registrar outro
        printf("\nRegistrar outro tempo? (s/n): ");
        char resp;
        scanf(" %c", &resp);
        getchar();
        continuar_registro = (resp == 's' || resp == 'S');
    }
    
    printf("\n=== DESAFIO SEGUIDOR DE LINHA CONCLUÍDO ===\n");
    printf("Ranking atualizado disponível no menu de rankings.\n");
}

// Menu principal corrigido
void menu_avaliador() {
    printf("\n===== MENU AVALIADOR =====\n");
    printf("1 - Executar Desafio Robô Sumô\n");
    printf("2 - Executar Desafio Robô Seguidor de Linha\n");
    printf("3 - Listar Equipes Cadastradas\n");
    printf("4 - Visualizar Ranking\n");
    printf("5 - Ver Chaveamentos Ativos\n");
    printf("0 - Sair\n");
    printf("Escolha: ");
    
    int opc;
    scanf("%d", &opc);
    getchar();

    switch (opc) {
        case 1: {
            // Verificar se existem equipes de Sumô - USAR FUNÇÃO DO CHAVEAMENTO
            if (!existem_equipes_para_desafio(SUMO)) {
                printf("\nNenhuma equipe de Sumô cadastrada ou número insuficiente.\n");
                printf("   É necessário pelo menos 2 equipes do tipo Sumô.\n");
                printf("   Cadastre equipes primeiro no menu de equipes.\n");
                pausar_sistema();
                break;
            }
            
            executar_desafio_sumo();
            pausar_sistema();
            break;
        }
        
        case 2: {
            // Verificar se existem equipes de Seguidor - USAR FUNÇÃO DO CHAVEAMENTO
            if (!existem_equipes_para_desafio(SEGUIDOR_LINHA)) {
                printf("\nNenhuma equipe de Seguidor de Linha cadastrada.\n");
                printf("   Cadastre equipes primeiro no menu de equipes.\n");
                pausar_sistema();
                break;
            }
            
            executar_desafio_seguidor();
            pausar_sistema();
            break;
        }
        
        case 3:
            listar_equipes();
            pausar_sistema();
            break;
            
        case 4:
            menu_ranking();
            break;
            
        case 5:
            printf("\n--- CHAVEAMENTOS ATIVOS ---\n");
            printf("Sumô:\n");
            exibir_chaveamento(SUMO);
            printf("\nSeguidor de Linha:\n");
            exibir_chaveamento(SEGUIDOR_LINHA);
            pausar_sistema();
            break;
            
        case 0:
            printf("Saindo do menu do avaliador...\n");
            return;
            
        default:
            printf("Opção inválida.\n");
            pausar_sistema();
            break;
    }

    return menu_avaliador();
}