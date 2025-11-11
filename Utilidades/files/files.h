#ifndef FILES_H
#define FILES_H

#include <stdio.h>
#include "Result.h"

// Caminho padrão dos arquivos de dados
#define DATA_DIR "./dados/"

// Funções principais de manipulação
void criar_diretorio_dados();
FILE* abrir_csv(const char *arquivo);
FILE* escrever_no_csv(const char *arquivo, const char *cabecalho);
int arquivo_existe(const char *arquivo);
long contar_linhas(const char *arquivo);
Result limpar_arquivo(const char *arquivo);

// Função utilitária de string
void limpar_linha(char *str);

#endif