#include "User.h"
#include "Menu_Equipes.h"
#include "Menu_Avaliador.h"
#include "Menu_Admin.h"
#include "Files.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Função auxiliar para pausa padronizada
void pausar_sistema() {
    printf("\nPressione ENTER para continuar...");
    getchar();
}

// ========================== LOGIN E MENU PRINCIPAL ==========================

void menu_principal() {
    // Garante que admin e avaliador padrão existam
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

    switch (opc) {
        case 0:
            printf("Encerrando sistema...\n");
            return;
        
        case 1: {
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
            
        default:
            printf("Opção inválida.\n");
            break;
    }

    pausar_sistema();
    return menu_principal();
}