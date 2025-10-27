#include "Files.h"
#include "Dados.h"
#include "User.h"

#include <string.h>

int ultimo_id(const char *nome_arquivo) {
    FILE *f = abrir_csv(nome_arquivo);
    if (!f) return -1;

    char linha[256];
    int ultimo = -1;
    fgets(linha, sizeof(linha), f); // pula cabeçalho
    while (fgets(linha, sizeof(linha), f))
        sscanf(linha, "%d,", &ultimo);
    fclose(f);
    return ultimo;
}

void trocar(int *a, int *b) {
    int t = *a; *a = *b; *b = t;
}

int particionar(int *lista[], int inicio, int fim) {
    int p = *lista[fim], i = inicio - 1;
    for (int j = inicio; j < fim; j++)
        if (*lista[j] < p) trocar(lista[++i], lista[j]);
    trocar(lista[++i], lista[fim]);
    return i;
}

void quicksort(int *lista[], int inicio, int fim) {
    if (inicio >= fim) return;
    int p = particionar(lista, inicio, fim);
    quicksort(lista, inicio, p - 1);
    quicksort(lista, p + 1, fim);
}
