#include "Files.h"
#include "Dados.h"
#include "User.h"

#include <string.h>

int ultimo_id(const char *nome_arquivo){

    FILE *f = abrir_csv(nome_arquivo);
    if (!f) return -1;

    char linha[256];
    int ultimo = -1;

    fgets(linha, sizeof(linha), f);
    
    while( fgets(linha, sizeof(linha), f) ){
        linha[strcspn(linha, "\n")] = '\0';
        int id;
        if (sscanf(linha, "%d,", &id) == 1){
            ultimo = id;
        }
    }
    fclose(f);
    return ultimo;
}

void trocar(int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

int particionar(int *lista[], int inicio, int fim){
    int pivo = *lista[fim];
    int i = inicio - 1;

    int j;
    for (j = inicio; j < fim; j++) {
        if (*lista[j]< pivo){
            i++;
            trocar(lista[j], lista[i]);
        }
    }
    i++;
    trocar(lista[fim], lista[i]);
    return i;
}

void quicksort(int *lista[], int inicio, int fim){
    if(inicio >= fim) return;
    
    int indicePivo = particionar(lista, inicio, fim);
    quicksort(lista, inicio, indicePivo-1);
    quicksort(lista, indicePivo+1, fim);
}