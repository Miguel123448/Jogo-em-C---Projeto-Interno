/* =========================================================
   include/hacker.h
   ========================================================= */

#ifndef HACKER_H
#define HACKER_H

#include <stdbool.h>

typedef enum {

    FEEDBACK_NENHUM,
    FEEDBACK_MUITO_ALTO,
    FEEDBACK_MUITO_BAIXO

} FeedbackFrequencia;

typedef struct {

    int codigoSecreto;

    int rastreamento;

    int dicasLiberadas;

    int vidas;

    int minFaixa;

    int maxFaixa;

    bool venceu;

    bool perdeu;

    bool logicaResolvida;

    FeedbackFrequencia feedback;

} HackerGame;

void InitHacker(HackerGame *game);

void LiberarDica(HackerGame *game);

void DrawDica(HackerGame *game);

bool VerificarLogica(
    HackerGame *game,
    int resposta
);

void TentarCodigo(
    HackerGame *game,
    int tentativa
);

void DrawFeedback(HackerGame *game);

#endif