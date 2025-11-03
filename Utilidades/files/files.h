#ifndef FILES_H
#define FILES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define DATA_DIR "./dados/"

// --- Manipulação de arquivos ---
int arquivo_existe(const char *arquivo);
FILE *abrir_csv(const char *arquivo);
FILE *escrever_no_csv(const char *arquivo, const char *header);

// --- Funções auxiliares ---
int ultimo_id(const char *nome_arquivo);

#endif