#include "Pontuacao.h"
#include "Files.h"
#include <string.h>
#include <stdlib.h>

Result registrar_pontuacao_para_equipe(int id_equipe, const char *nome_equipe, const char *tipo, float tempo) {
    FILE *f = escrever_no_csv("resultados_robo.csv", "ID_EQUIPE,NOME_EQUIPE,DESAFIO,TEMPO,PONTOS\n");
    if (!f) return erro(ERRO_ARQUIVO, "Erro ao abrir resultados_robo.csv");

    Pontuacao p;
    p.id_equipe = id_equipe;
    strncpy(p.nome_equipe, nome_equipe, sizeof(p.nome_equipe));
    strncpy(p.tipo_desafio, tipo, sizeof(p.tipo_desafio));
    p.tempo = tempo;

    if (strcmp(tipo, "Seguidor") == 0)
        p.pontos = (int)(1000 / tempo);
    else
        p.pontos = 100; // padrão para sumô

    fprintf(f, "%d,%s,%s,%.2f,%d\n", p.id_equipe, p.nome_equipe, p.tipo_desafio, p.tempo, p.pontos);
    fclose(f);
    return ok();
}