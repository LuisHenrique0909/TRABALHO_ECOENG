#include "User.h"
#include "Menu_Equipes.h"
#include "Menu_Avaliador.h"
#include "Menu_Admin.h"
#include "Files.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ========================== LOGIN E MENU PRINCIPAL ==========================

void menu_principal() {
    inicializar_admin();
    inicializar_avaliador();

    printf("\n===== SISTEMA DE GERENCIAMENTO DO EVENTO ECOENG =====\n");
    printf("1 - Fazer login\n");
    printf("2 - Cadastrar novo usuário\n");
    printf("0 - Sair\n");
    printf("Escolha: ");

    int opc;
    scanf("%d", &opc);
    getchar();

    if (opc == 0) {
        printf("Encerrando sistema...\n");
        return;
    }
    if (opc == 1) {
        User *usuario = login_user();
        if (!usuario) {
            printf("\nUsuário não encontrado.\n");
            printf("Deseja se cadastrar como PARTICIPANTE? (s/n): ");
            char resp;
            scanf(" %c", &resp);
            getchar();

            if (resp == 's' || resp == 'S') {
                singin(); // Cadastro automático
                printf("\nCadastro realizado! Faça login novamente.\n");
            } else {
                printf("Voltando ao menu principal...\n");
            }

            return menu_principal();
        }

        printf("\nBem-vindo(a), %s! Cargo: %s\n", usuario->nome, cargo_pra_texto(usuario->cargo));
        
        // CORREÇÃO: Usar o mesmo usuário já obtido
        // Redirecionamento conforme cargo
        switch (usuario->cargo) {
            case ADMIN:
                menu_admin();
                break;
            case AVALIADOR:
                menu_avaliador();
                break;
            case PARTICIPANTE:
                menu_equipe(usuario);
                break;
            default:
                printf("Cargo inválido. Encerrando sessão.\n");
                break;
        }

        free(usuario);
        return menu_principal(); // Volta ao menu principal após sair do submenu
    } 
    else if (opc == 2) {
        // Cadastro
        singin();
        printf("\nCadastro realizado!\n");
        return menu_principal();
    } else {
        printf("Opção inválida. Tente novamente.\n");
        return menu_principal();
    }
}