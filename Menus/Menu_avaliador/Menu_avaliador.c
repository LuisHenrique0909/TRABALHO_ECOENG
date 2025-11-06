#include "Menu_Avaliador.h"
#include "Desafio_robo.h"
#include "Cadastro_Equipes.h"
#include "Ranking.h"
#include "pausar_sistema.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>


void menu_avaliador() {
    printf("\n===== MENU AVALIADOR =====\n");
    printf("1 - Iniciar Desafio Robô Sumô\n");
    printf("2 - Iniciar Desafio Robô Seguidor de Linha\n");
    printf("3 - Listar Equipes Cadastradas\n");
    printf("4 - Visualizar Ranking Geral\n");
    printf("0 - Sair\n");
    printf("Escolha: ");
    
    int opc;
    scanf("%d", &opc);
    getchar();

    switch (opc) {
        case 1: {
            printf("\n=== INICIANDO DESAFIO ROBÔ SUMÔ ===\n");
            iniciar_desafio(SUMO);
            Resultado_Desafio res = {0};
            res.tipo = SUMO;
            registrar_resultado(&res);

            printf("\nDeseja visualizar o ranking? (s/n): ");
            char resp;
            scanf(" %c", &resp);
            getchar();
            if (resp == 's' || resp == 'S') {
                printf("\n1 - Ranking Sumô\n2 - Ranking Geral\nEscolha: ");
                int opc_rank;
                scanf("%d", &opc_rank);
                getchar();
                if (opc_rank == 1) mostrar_ranking(SUMO);
                else if (opc_rank == 2) mostrar_ranking_geral();
            }
            pausar_sistema();
            break;
        }
        case 2: {
            printf("\n=== INICIANDO DESAFIO ROBÔ SEGUIDOR DE LINHA ===\n");
            iniciar_desafio(SEGUIDOR_LINHA);
            Resultado_Desafio res = {0};
            res.tipo = SEGUIDOR_LINHA;
            registrar_resultado(&res);

            printf("\nDeseja visualizar o ranking? (s/n): ");
            char resp;
            scanf(" %c", &resp);
            getchar();
            if (resp == 's' || resp == 'S') {
                printf("\n1 - Ranking Seguidor de Linha\n2 - Ranking Geral\nEscolha: ");
                int opc_rank;
                scanf("%d", &opc_rank);
                getchar();
                if (opc_rank == 1) mostrar_ranking(SEGUIDOR_LINHA);
                else if (opc_rank == 2) mostrar_ranking_geral();
            }
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