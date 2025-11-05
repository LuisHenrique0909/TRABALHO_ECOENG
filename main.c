/* main.c
 * Ponto de entrada do Sistema ECOENG
 * - Garante diretório ./dados
 * - Inicializa usuários padrão (admin/avaliador)
 * - Inicia o menu principal (login/redirecionamento)
 */

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#include "Menu_Principal.h"
#include "Files.h"
#include "User.h"

int main(void) {
    /* Ajuste de locale para evitar problemas com acentuação no Windows.
       Se preferir, remova ou altere conforme seu ambiente. */
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