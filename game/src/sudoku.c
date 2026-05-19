/* =========================================================
   src/sudoku.c
   ========================================================= */

#include "sudoku.h"
#include "raylib.h"

#include <stdlib.h>
#include <time.h>

/* =========================================================
    LÓGICA DO GERADOR DINÂMICO
   ========================================================= */

static int PodeColocar(
    int board[4][4],
    int row,
    int col,
    int value
) {

    for (int i = 0; i < 4; i++) {

        if (
            board[row][i] == value ||
            board[i][col] == value
        ) {

            return 0;
        }
    }

    int startRow = (row / 2) * 2;

    int startCol = (col / 2) * 2;

    for (
        int i = startRow;
        i < startRow + 2;
        i++
    ) {

        for (
            int j = startCol;
            j < startCol + 2;
            j++
        ) {

            if (board[i][j] == value) {

                return 0;
            }
        }
    }

    return 1;
}

static void ContarSolucoes(
    int board[4][4],
    int *contador
) {

    for (int r = 0; r < 4; r++) {

        for (int c = 0; c < 4; c++) {

            if (board[r][c] == 0) {

                for (int n = 1; n <= 4; n++) {

                    if (
                        PodeColocar(
                            board,
                            r,
                            c,
                            n
                        )
                    ) {

                        board[r][c] = n;

                        ContarSolucoes(
                            board,
                            contador
                        );

                        board[r][c] = 0;

                        if (*contador > 1) {

                            return;
                        }
                    }
                }

                return;
            }
        }
    }

    (*contador)++;
}

static int GerarGabarito(
    int board[4][4]
) {

    for (int r = 0; r < 4; r++) {

        for (int c = 0; c < 4; c++) {

            if (board[r][c] == 0) {

                int nums[] = {1, 2, 3, 4};

                for (int i = 0; i < 4; i++) {

                    int randIdx = rand() % 4;

                    int temp = nums[i];

                    nums[i] = nums[randIdx];

                    nums[randIdx] = temp;
                }

                for (int i = 0; i < 4; i++) {

                    if (
                        PodeColocar(
                            board,
                            r,
                            c,
                            nums[i]
                        )
                    ) {

                        board[r][c] = nums[i];

                        if (
                            GerarGabarito(board)
                        ) {

                            return 1;
                        }

                        board[r][c] = 0;
                    }
                }

                return 0;
            }
        }
    }

    return 1;
}

/* =========================================================
    FUNÇÕES DO JOGO
   ========================================================= */

void InitSudoku(
    SudokuGame *game,
    Dificuldade dificuldade
) {

    game->dificuldade = dificuldade;

    game->selectedRow = 0;

    game->selectedCol = 0;

    game->erros = 0;

    game->erroMaximo = false;

    game->maxErros = 3;

    game->completo = false;

    int tempBoard[4][4] = {0};

    GerarGabarito(tempBoard);

    int alvoVazias = 0;

    if (dificuldade == FACIL)
        alvoVazias = 6;

    else if (dificuldade == MEDIO)
        alvoVazias = 8;

    else if (dificuldade == DIFICIL)
        alvoVazias = 10;

    int posicoes[16];

    for (int i = 0; i < 16; i++) {

        posicoes[i] = i;
    }

    for (int i = 0; i < 16; i++) {

        int r = rand() % 16;

        int t = posicoes[i];

        posicoes[i] = posicoes[r];

        posicoes[r] = t;
    }

    int vaziasContador = 0;

    for (
        int i = 0;
        i < 16 && vaziasContador < alvoVazias;
        i++
    ) {

        int r = posicoes[i] / 4;

        int c = posicoes[i] % 4;

        int backup = tempBoard[r][c];

        tempBoard[r][c] = 0;

        int solucoes = 0;

        int copia[4][4];

        for (int x = 0; x < 4; x++) {

            for (int y = 0; y < 4; y++) {

                copia[x][y] = tempBoard[x][y];
            }
        }

        ContarSolucoes(
            copia,
            &solucoes
        );

        if (solucoes == 1) {

            vaziasContador++;

        } else {

            tempBoard[r][c] = backup;
        }
    }

    for (int i = 0; i < 4; i++) {

        for (int j = 0; j < 4; j++) {

            game->original[i][j] =
                tempBoard[i][j];

            game->board[i][j] =
                tempBoard[i][j];
        }
    }
}

bool SudokuCompleto(
    SudokuGame *game
) {

    for (int i = 0; i < 4; i++) {

        for (int j = 0; j < 4; j++) {

            if (
                game->board[i][j] == 0
            ) {

                return false;
            }
        }
    }

    return true;
}

void UpdateSudoku(
    SudokuGame *game
) {

    if (IsKeyPressed(KEY_RIGHT))
        game->selectedCol++;

    if (IsKeyPressed(KEY_LEFT))
        game->selectedCol--;

    if (IsKeyPressed(KEY_DOWN))
        game->selectedRow++;

    if (IsKeyPressed(KEY_UP))
        game->selectedRow--;

    if (game->selectedRow < 0)
        game->selectedRow = 0;

    if (game->selectedRow > 3)
        game->selectedRow = 3;

    if (game->selectedCol < 0)
        game->selectedCol = 0;

    if (game->selectedCol > 3)
        game->selectedCol = 3;

    if (game->erroMaximo) {

        return;
    }

    int row = game->selectedRow;

    int col = game->selectedCol;

    if (game->original[row][col] == 0) {

        for (int n = 1; n <= 4; n++) {

            if (
                IsKeyPressed(
                    KEY_ONE + (n - 1)
                )
            ) {

                if (
                    PodeColocar(
                        game->board,
                        row,
                        col,
                        n
                    )
                ) {

                    game->board[row][col] = n;

                } else {

                    game->erros++;

                    if (
                        game->erros >=
                        game->maxErros
                    ) {

                        game->erroMaximo =
                            true;
                    }
                }
            }
        }

        if (IsKeyPressed(KEY_ZERO)) {

            game->board[row][col] = 0;
        }
    }

    game->completo =
        SudokuCompleto(game);
}

void DrawSudoku(
    SudokuGame *game
) {

    int startX = 100;

    int startY = 120;

    int cell = 100;

    DrawText(
        "DESCRIPTOGRAFANDO BLOCO...",
        100,
        40,
        30,
        GREEN
    );

    for (int i = 0; i < 4; i++) {

        for (int j = 0; j < 4; j++) {

            Rectangle rect = {

                (float)startX + j * cell,
                (float)startY + i * cell,
                (float)cell,
                (float)cell
            };

            Color color = DARKGRAY;

            if (
                i == game->selectedRow &&
                j == game->selectedCol
            ) {

                color = GREEN;
            }

            DrawRectangleRec(
                rect,
                color
            );

            DrawRectangleLinesEx(
                rect,
                2,
                BLACK
            );

            if (
                game->original[i][j] != 0
            ) {

                DrawRectangleLinesEx(
                    rect,
                    4,
                    SKYBLUE
                );
            }

            if (
                game->board[i][j] != 0
            ) {

                DrawText(
                    TextFormat(
                        "%d",
                        game->board[i][j]
                    ),
                    (int)rect.x + 40,
                    (int)rect.y + 28,
                    42,
                    WHITE
                );
            }
        }
    }

    DrawText(
        TextFormat(
            "ERROS: %d/%d",
            game->erros,
            game->maxErros
        ),
        100,
        560,
        28,
        RED
    );

    DrawText(
        "SETAS = MOVER",
        100,
        620,
        22,
        GRAY
    );

    DrawText(
        "1-4 = INSERIR",
        300,
        620,
        22,
        GRAY
    );

    DrawText(
        "0 = APAGAR",
        520,
        620,
        22,
        GRAY
    );

    if (game->erroMaximo) {

        DrawText(
            "SISTEMA BLOQUEADO",
            100,
            660,
            28,
            RED
        );
    }
}