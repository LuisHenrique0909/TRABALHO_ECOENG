#include "Cadastro_Equipes.h"  // para cadastrar_equipe e vincular_participante
#include "User.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// MENU DE EQUIPES

void menu_equipes(User *usuario_logado) {
    int opcao;

    do {
        printf("\n===== MENU DE EQUIPES =====\n");
        printf("1. Cadastrar nova equipe\n");
        printf("2. Vincular participante a equipe\n");
        printf("3. Voltar\n");
        printf("Escolha: ");
        if (scanf("%d", &opcao) != 1) {
            int c; while ((c = getchar()) != '\n' && c != EOF) {}
            opcao = 0;
        }
        getchar(); // limpa buffer

        switch (opcao) {
            case 1:
                cadastrar_equipe(usuario_logado);
                break;
            case 2:
                vincular_participante(usuario_logado);
                break;
            case 3:
                printf("Voltando ao menu principal...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 3);
}