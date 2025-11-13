#include "User.h"
#include "Menu_Equipes.h"
#include "Menu_Avaliador.h"
#include "Menu_Admin.h"
#include "Files.h"
#include "pausar_sistema.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ========================== LOGIN E MENU PRINCIPAL ==========================

void menu_principal() {
    // Garante que admin e avaliador padrão existam
    inicializar_admin();
    inicializar_avaliador();

    printf("\n===== SISTEMA DE GERENCIAMENTO DO EVENTO ECOENG =====\n");
    printf("1 - Fazer login\n");
    printf("2 - Cadastrar novo usuário\n");
    printf("3 - Local do evento\n");
    printf("0 - Sair\n");
    printf("Escolha: ");

    int opc;
    scanf("%d", &opc);
    getchar();

    switch (opc) {
        case 0:
            printf("Encerrando sistema...\n");
            return;
        
        case 1: {
            User *usuario = login_user();
            if (!usuario) {
                printf("\nUsuário não encontrado.\n");
                printf("Deseja se cadastrar como PARTICIPANTE? (sim/não): ");
                char resp[3];
                scanf("%s", &resp[0]);
                getchar();

                if (resp[0] == 's' || resp[0] == 'S') {
                    singin(); // Cadastro automático
                    printf("\nCadastro realizado! Faça login novamente.\n");
                } else {
                    printf("Voltando ao menu principal...\n");
                }
            } else {
                printf("\nBem-vindo(a), %s! Cargo: %s\n", 
                       usuario->nome, cargo_pra_texto(usuario->cargo));
                
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
            }
            break;
        }
        
        case 2:
            singin();
            printf("\nCadastro realizado com sucesso!\n");
            break;

        case 3:
         printf("\n=== DATA E LOCAL DO EVENTO ===\n");
            printf("O evento ECOENG será realizado no bloco E\n");
            printf("Data: 18 e 19 de Novembro");
            printf("\n=======================================\n");
            break;
            
        default:
            printf("Opção inválida.\n");
            break;
    }

    pausar_sistema();
    return menu_principal();
}