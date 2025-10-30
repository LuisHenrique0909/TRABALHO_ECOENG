#include <stdio.h>
#include <stdlib.h>
#include "User.h"
#include "Result.h"

void menu() {
    printf("\n====================================\n");
    printf("        SISTEMA ECOENG CADASTRO     \n");
    printf("====================================\n");
    printf("1 - Cadastrar usuário\n");
    printf("2 - Autorizar usuário (Admin)\n");
    printf("0 - Sair\n");
    printf("====================================\n");
    printf("Escolha uma opção: ");
}

int main() {
    int opcao;
    inicializar_admin(); // garante que o admin Luis Henrique existe

    do {
        menu();
        scanf("%d", &opcao);
        getchar(); // limpa o buffer do teclado

        switch (opcao) {
            case 1:
                printf("\n--- CADASTRAR USUÁRIO ---\n");
                singin();
                break;

            case 2: {
                int id_autorizar;
                printf("\n--- AUTORIZAR USUÁRIO ---\n");
                printf("Digite o ID do usuário que deseja autorizar: ");
                scanf("%d", &id_autorizar);
                getchar();

                Result r = autorizar_user(id_autorizar);
                if (r.code == OK)
                    printf("Usuário autorizado com sucesso!\n");
                else
                    print_err(&r);
                break;
            }

            case 0:
                printf("Saindo do sistema...\n");
                break;

            default:
                printf("Opção inválida, tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}
