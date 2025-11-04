#include "Menu_Avaliador.h"
#include "Desafio_robo.h"
#include "Cadastro_Equipes.h"
#include "Ranking.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void menu_avaliador() {
    int opc;
    do {
        printf("\n===== MENU AVALIADOR =====\n");
        printf("1 - Iniciar Desafio Robô Sumô\n");
        printf("2 - Iniciar Desafio Robô Seguidor de Linha\n");
        printf("3 - Listar Equipes Cadastradas\n");
        printf("4 - Visualizar Ranking Geral\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opc);
        getchar();

        switch (opc) {
            case 1: {
                iniciar_desafio(SUMO);
                Resultado_Desafio res = {0};
                res.tipo = SUMO;
                registrar_resultado(&res);

                printf("\nDeseja visualizar o ranking atualizado? (s/n): ");
                char resp;
                scanf(" %c", &resp);
                getchar();
                if (resp == 's' || resp == 'S')
                    mostrar_ranking(SUMO);
                break;
            }
            case 2: {
                iniciar_desafio(SEGUIDOR_LINHA);
                Resultado_Desafio res = {0};
                res.tipo = SEGUIDOR_LINHA;
                registrar_resultado(&res);

                printf("\nDeseja visualizar o ranking atualizado? (s/n): ");
                char resp;
                scanf(" %c", &resp);
                getchar();
                if (resp == 's' || resp == 'S')
                    mostrar_ranking(SEGUIDOR_LINHA);
                break;
            }
            case 3:
                listar_equipes();
                break;
            case 4:
                menu_ranking();
                break;
            case 0:
                printf("Saindo do menu do avaliador...\n");
                break;
            default:
                printf("Opção inválida.\n");
        }
    } while (opc != 0);
}