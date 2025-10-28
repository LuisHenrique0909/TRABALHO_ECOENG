#ifndef RESULT_H
#define RESULT_H

typedef enum {
    OK = 0,
    ERRO_LOGICA = -1,
    ERRO_ARQUIVO = -2,
    ERRO_MEMORIA = -3,
    ERRO_INVALIDO = -4,
    ERRO_PERMISSAO = -5
} Res;

typedef struct {
    Res code;
    const char *msg;
} Result;

// Retorno de sucesso (sem mensagem)
Result ok(void);

// Retorno de erro com código e mensagem
Result erro(Res code, const char *msg);

// Imprime o erro no terminal
void print_err(Result *r);

#endif