#ifndef RANKING_H
#define RANKING_H

#include "Desafio_Robo.h"

// Agrupar por equipe somando pontos
    typedef struct {
        char nome_equipe[100];
        int total_pontos;
        float melhor_tempo;
        int participacoes;
    } EquipeRanking;

// Protótipos das funções
void mostrar_ranking(TipoDesafio tipo);
void mostrar_ranking_geral();  // NOVA FUNÇÃO
void menu_ranking();

#endif