#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#include "Funcoes_cvs/Dados.h"
#include "Cadastro/Cadastro_Equipes/Cadastro_Equipes.h"


int main() {
    setlocale(LC_ALL, "Portuguese");

    FILE *f = escrever_no_csv("luis.csv", "id,nome");

    fprintf(f, "0,higor\n");
    
    return 0;
}