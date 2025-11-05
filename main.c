#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#include "Menu_principal.h"
#include "Files.h"
#include "User.h"

int main() {
    // CONFIGURAÇÃO UTF-8
    setlocale(LC_ALL, "pt_BR.UTF-8");
    
    printf("==============================================\n");
    printf("   SISTEMA DE GERENCIAMENTO DO EVENTO ECOENG  \n");
    printf("==============================================\n\n");

    criar_diretorio_dados();
    inicializar_admin();
    inicializar_avaliador();
    menu_principal();

    printf("\n==============================================\n");
    printf("Sistema encerrado. Até mais!\n");
    printf("==============================================\n");
    return 0;
}