#ifndef FILES_H
#define FILES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define DATA_DIR "./dados/"

// ====================== FUNÇÕES PRINCIPAIS ======================

// Verifica se o diretório de dados existe; se não, cria.
void verificar_diretorio_dados();

// Verifica se o arquivo existe no diretório /dados/
int arquivo_existe(const char *arquivo);

// Abre um CSV apenas para leitura
FILE *abrir_csv(const char *arquivo);

// Abre (ou cria) um CSV para escrita (modo append), mantendo dados anteriores
FILE *escrever_no_csv(const char *arquivo, const char *header);

// Remove o caractere '\n' de uma string
void limpar_linha(char *linha);

#endif