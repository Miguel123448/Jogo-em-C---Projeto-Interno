/* =========================================================
   src/hacker.c
   ========================================================= */

#include "hacker.h"
#include "clues.h"

#include "raylib.h"

#include <stdio.h>
#include <stdlib.h>

void InitHacker(HackerGame *game) {

    game->codigoSecreto = rand() % 101;

    game->rastreamento = 0;

    game->dicasLiberadas = 0;

    game->vidas = 7;

    game->venceu = false;

    game->perdeu = false;

    game->logicaResolvida = false;

    game->feedback = FEEDBACK_NENHUM;

    game->minFaixa = game->codigoSecreto - 15;
    game->maxFaixa = game->codigoSecreto + 15;

    if (game->minFaixa < 0)
        game->minFaixa = 0;

    if (game->maxFaixa > 100)
        game->maxFaixa = 100;
}

void LiberarDica(HackerGame *game) {

    if (game->dicasLiberadas < 5) {
        game->dicasLiberadas++;
    }
}

bool VerificarLogica(
    HackerGame *game,
    int resposta
) {

    const LogicClue *clue =
        GetClue(game->dicasLiberadas - 1);

    if (clue == NULL)
        return false;

    if (resposta == clue->respostaCorreta) {

        game->logicaResolvida = true;

        return true;
    }

    game->rastreamento += 10;

    if (game->rastreamento >= 100) {
        game->perdeu = true;
    }

    return false;
}

void DrawDica(HackerGame *game) {

    DrawText(
        "DESAFIO LOGICO",
        650,
        50,
        35,
        GREEN
    );

    if (game->dicasLiberadas > 0) {

        const LogicClue *clue =
            GetClue(game->dicasLiberadas - 1);

        if (clue != NULL) {

            DrawText(
                clue->descricao,
                650,
                120,
                20,
                WHITE
            );

            DrawText(
                clue->pergunta,
                650,
                420,
                25,
                YELLOW
            );
        }

    } else {

        DrawText(
            "Complete o Sudoku para liberar a dica.",
            650,
            150,
            25,
            WHITE
        );
    }

    DrawText(
        TextFormat(
            "RASTREAMENTO: %d%%",
            game->rastreamento
        ),
        650,
        620,
        30,
        RED
    );
}

void TentarCodigo(
    HackerGame *game,
    int tentativa
) {

    if (tentativa == game->codigoSecreto) {

        game->venceu = true;

        game->feedback = FEEDBACK_NENHUM;

        return;
    }

    if (tentativa > game->codigoSecreto) {

        game->feedback = FEEDBACK_MUITO_ALTO;

    } else {

        game->feedback = FEEDBACK_MUITO_BAIXO;
    }

    game->vidas--;

    game->rastreamento += 15;

    if (
        game->vidas <= 0 ||
        game->rastreamento >= 100
    ) {

        game->perdeu = true;
    }
}

void DrawFeedback(HackerGame *game) {

    const char *texto;

    Color cor = WHITE;

    if (game->feedback == FEEDBACK_MUITO_ALTO) {

        texto = "CODIGO MUITO ALTO";

        cor = ORANGE;

    } else if (game->feedback == FEEDBACK_MUITO_BAIXO) {

        texto = "CODIGO MUITO BAIXO";

        cor = SKYBLUE;

    } else {

        texto = "";
    }

    DrawText(
        texto,
        350,
        220,
        50,
        cor
    );

    DrawText(
        TextFormat(
            "VIDAS: %d",
            game->vidas
        ),
        350,
        320,
        35,
        GREEN
    );

    DrawText(
        TextFormat(
            "RASTREAMENTO: %d%%",
            game->rastreamento
        ),
        350,
        380,
        35,
        RED
    );
}