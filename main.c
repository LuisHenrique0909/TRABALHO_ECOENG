
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#include "Menu_principal.h"
#include "Files.h"
#include "User.h"

int main() {

#ifdef _WIN32
    setlocale(LC_ALL, "Portuguese_Brazil.1252");
#else
    setlocale(LC_ALL, "");
#endif

    printf("==============================================\n");
    printf("   SISTEMA DE GERENCIAMENTO DO EVENTO ECOENG  \n");
    printf("==============================================\n\n");

    /* Garante que a pasta de dados exista (./dados/) */
    criar_diretorio_dados();

    /* Inicializa contas padrão se necessário */
    inicializar_admin();
    inicializar_avaliador();

    /* Inicia loop principal do sistema (login / redirecionamento) */
    menu_principal();

    printf("\n==============================================\n");
    printf("Sistema encerrado. Até mais.\n");
    printf("==============================================\n");
    return 0;
}