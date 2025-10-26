#ifndef Dados_H
#define Dados_H

int arquivo_existe(const char* arquivo);
FILE* abrir_csv(const char* arquivo);
FILE* escrever_no_csv(const char* arquivo, const char* header);

#endif
