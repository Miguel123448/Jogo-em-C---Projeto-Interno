#include "raylib.h"
#include <stdlib.h>
#include <time.h>
#include "sudoku.h"
#include "hacker.h"
#include "stats.h"

typedef enum {

    MENU,

    SUDOKU,

    LOGICA,

    ADIVINHACAO,

    FEEDBACK,

    VITORIA,

    DERROTA,

    HISTORICO

} GameState;

int main() {

    InitWindow(
        1280,
        720,
        "Hacker Sudoku"
    );

    srand((unsigned)time(NULL));

    SetTargetFPS(60);

    SudokuGame sudoku;

    HackerGame hacker;

    GameState estado = MENU;

    int tentativa = 0;

    int respostaLogica = 0;

    float tempoRestante = 60.0f;

    GameStats stats = {0};

    bool statsSalvas = false;

    while (!WindowShouldClose()) {

        tempoRestante -= GetFrameTime();

        if (
            tempoRestante <= 0 &&
            estado != MENU &&
            estado != VITORIA &&
            estado != DERROTA &&
            estado != HISTORICO
        ) {

            estado = DERROTA;
        }

        BeginDrawing();

        ClearBackground(BLACK);

        if (
            estado != MENU &&
            estado != VITORIA &&
            estado != DERROTA &&
            estado != HISTORICO
        ) {

            DrawText(
                TextFormat(
                    "TEMPO: %.0f",
                    tempoRestante
                ),
                1050,
                30,
                30,
                RED
            );
        }

        switch (estado) {

            case MENU:

                DrawText(
                    "ATAQUE HACKER DETECTADO",
                    260,
                    180,
                    45,
                    RED
                );

                DrawText(
                    "RESOLVA O SUDOKU",
                    390,
                    280,
                    30,
                    GREEN
                );

                DrawText(
                    "DECIFRE A LOGICA",
                    390,
                    330,
                    30,
                    GREEN
                );

                DrawText(
                    "DESCUBRA O CODIGO",
                    390,
                    380,
                    30,
                    GREEN
                );

                DrawText(
                    "PRESSIONE ENTER",
                    420,
                    520,
                    35,
                    WHITE
                );

                if (IsKeyPressed(KEY_ENTER)) {

                    InitSudoku(
                        &sudoku,
                        FACIL
                    );

                    InitHacker(&hacker);

                    tentativa = 0;

                    respostaLogica = 0;

                    tempoRestante = 60.0f;

                    statsSalvas = false;

                    estado = SUDOKU;
                }

                break;

            case SUDOKU:

                UpdateSudoku(&sudoku);

                DrawSudoku(&sudoku);

                DrawText(
                    "DESAFIO 1: SUDOKU",
                    700,
                    80,
                    35,
                    GREEN
                );

                DrawText(
                    "COMPLETE O SUDOKU",
                    700,
                    140,
                    25,
                    WHITE
                );

                DrawRectangle(
                    700,
                    220,
                    450,
                    250,
                    DARKGRAY
                );

                DrawText(
                    "DESAFIO LOGICO BLOQUEADO",
                    760,
                    320,
                    25,
                    GRAY
                );

                if (sudoku.completo) {

                    LiberarDica(&hacker);

                    estado = LOGICA;
                }

                if (sudoku.erroMaximo) {

                    estado = DERROTA;
                }

                break;

            case LOGICA:

                DrawSudoku(&sudoku);

                DrawDica(&hacker);

                DrawText(
                    "PRESSIONE 1, 2 OU 3",
                    650,
                    560,
                    30,
                    GREEN
                );

                if (IsKeyPressed(KEY_ONE))
                    respostaLogica = 1;

                if (IsKeyPressed(KEY_TWO))
                    respostaLogica = 2;

                if (IsKeyPressed(KEY_THREE))
                    respostaLogica = 3;

                if (respostaLogica != 0) {

                    if (
                        VerificarLogica(
                            &hacker,
                            respostaLogica
                        )
                    ) {

                        tentativa = 0;

                        respostaLogica = 0;

                        estado = ADIVINHACAO;

                    } else {

                        respostaLogica = 0;
                    }
                }

                if (hacker.perdeu) {

                    estado = DERROTA;
                }

                break;

            case ADIVINHACAO:

                DrawText(
                    "DESAFIO FINAL",
                    420,
                    80,
                    45,
                    GREEN
                );

                DrawText(
                    "DESCUBRA O CODIGO",
                    330,
                    160,
                    40,
                    WHITE
                );

                DrawText(
                    TextFormat(
                        "NUMERO ENTRE %d E %d",
                        hacker.minFaixa,
                        hacker.maxFaixa
                    ),
                    350,
                    230,
                    30,
                    YELLOW
                );

                DrawText(
                    TextFormat("%d", tentativa),
                    500,
                    330,
                    70,
                    WHITE
                );

                DrawText(
                    TextFormat(
                        "VIDAS: %d",
                        hacker.vidas
                    ),
                    450,
                    430,
                    35,
                    GREEN
                );

                if (IsKeyPressed(KEY_ZERO))
                    tentativa = tentativa * 10 + 0;

                if (IsKeyPressed(KEY_ONE))
                    tentativa = tentativa * 10 + 1;

                if (IsKeyPressed(KEY_TWO))
                    tentativa = tentativa * 10 + 2;

                if (IsKeyPressed(KEY_THREE))
                    tentativa = tentativa * 10 + 3;

                if (IsKeyPressed(KEY_FOUR))
                    tentativa = tentativa * 10 + 4;

                if (IsKeyPressed(KEY_FIVE))
                    tentativa = tentativa * 10 + 5;

                if (IsKeyPressed(KEY_SIX))
                    tentativa = tentativa * 10 + 6;

                if (IsKeyPressed(KEY_SEVEN))
                    tentativa = tentativa * 10 + 7;

                if (IsKeyPressed(KEY_EIGHT))
                    tentativa = tentativa * 10 + 8;

                if (IsKeyPressed(KEY_NINE))
                    tentativa = tentativa * 10 + 9;

                if (IsKeyPressed(KEY_BACKSPACE)) {

                    tentativa /= 10;
                }

                if (IsKeyPressed(KEY_ENTER)) {

                    TentarCodigo(
                        &hacker,
                        tentativa
                    );

                    if (hacker.venceu) {

                        estado = VITORIA;

                    } else if (hacker.perdeu) {

                        estado = DERROTA;

                    } else {

                        estado = FEEDBACK;
                    }
                }

                break;

            case FEEDBACK:

                DrawFeedback(&hacker);

                DrawText(
                    "PRESSIONE ENTER",
                    420,
                    520,
                    30,
                    WHITE
                );

                if (IsKeyPressed(KEY_ENTER)) {

                    tentativa = 0;

                    hacker.feedback =
                        FEEDBACK_NENHUM;

                    estado = ADIVINHACAO;
                }

                break;

             case VITORIA:
                
                if (!statsSalvas) {
                    AtualizarStats(
                        &stats,
                        1,
                        sudoku.erros,
                        true
                    );
                    statsSalvas = true;
                }

                DrawText(
                    "AMEACA NEUTRALIZADA",
                    280,
                    220,
                    55,
                    GREEN
                );

                DrawText(
                    "SISTEMA PROTEGIDO COM SUCESSO",
                    210,
                    330,
                    40,
                    WHITE
                );

                DrawText(
                    "PRESSIONE H PARA VER LOGS DE DEFESA",
                    210,
                    500,
                    30,
                    GOLD
                );

                if (IsKeyPressed(KEY_H)) {
                    estado = HISTORICO;
                }
                break;
                
            case DERROTA:
               
                if (!statsSalvas) {
                    AtualizarStats(
                        &stats,
                        0, 
                        sudoku.erros,
                        false
                    );
                    statsSalvas = true;
                }

                DrawText(
                    "SISTEMA COMPROMETIDO",
                    240,
                    220,
                    55,
                    RED
                );

                DrawText(
                    "FALHA CRITICA: DADOS VAZADOS",
                    240,
                    330,
                    40,
                    WHITE
                );

                DrawText(
                    "PRESSIONE H PARA RELATORIO DE DANOS",
                    190,
                    500,
                    30,
                    GRAY
                );

                if (IsKeyPressed(KEY_H)) {
                    estado = HISTORICO;
                }
                break;

            case HISTORICO:

                DrawHistorico(&stats);

                if (
                    IsKeyPressed(KEY_ENTER) ||
                    IsKeyPressed(KEY_H)
                ) {

                    estado = MENU;
                }

                break;
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}