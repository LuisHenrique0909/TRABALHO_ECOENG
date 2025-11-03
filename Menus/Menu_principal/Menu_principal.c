#include "Menu_Principal.h"
#include "Menu_Avaliador.h"
#include "Menu_Equipes.h"
#include "Files.h"
#include <stdio.h>
#include <stdlib.h>

// =================== MENU PRINCIPAL ===================

void menu_principal() {
    printf("=========================================\n");
    printf("      SISTEMA DE GERENCIAMENTO ECOENG    \n");
    printf("=========================================\n");

    // Inicializa usuários padrão (Admin e Avaliador)
    inicializar_admin();
    inicializar_avaliador();

    printf("\nPor favor, realize o login para continuar.\n");

    // Realiza login
    User *usuario_logado = login_user();
    if (!usuario_logado) {
        printf("\nFalha no login. Encerrando o sistema...\n");
        return;
    }

    printf("\nBem-vindo, %s!\n", usuario_logado->nome);
    printf("Cargo: %s\n", cargo_pra_texto(usuario_logado->cargo));

    // Redireciona conforme o cargo do usuário
    switch (usuario_logado->cargo) {
        case ADMIN:
            printf("\n Menu do ADMIN ainda em desenvolvimento.\n");
            break;
        case AVALIADOR:
            menu_avaliador();
            break;
        case PARTICIPANTE:
            menu_equipe(usuario_logado);
            break;
        default:
            printf("Erro: tipo de usuário desconhecido.\n");
    }

    // Libera memória
    free(usuario_logado);

    printf("\nSessão encerrada. Obrigado por usar o sistema!\n");
}