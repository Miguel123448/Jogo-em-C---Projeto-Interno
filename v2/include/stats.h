/* =========================================================
   include/stats.h
   ========================================================= */

#ifndef STATS_H
#define STATS_H

#include <stdbool.h>

typedef struct {

    int partidasJogadas;

    int totalAcertos;

    int totalErros;

    int vitoriasHacker;

    float precisaoMedia;

} GameStats;

void AtualizarStats(
    GameStats *stats,
    int acertos,
    int erros,
    bool venceu
);

void DrawHistorico(
    GameStats *stats
);

#endif