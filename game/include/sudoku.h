/* =========================================================
   include/sudoku.h
   ========================================================= */

#ifndef SUDOKU_H
#define SUDOKU_H

#include <stdbool.h>

#define SIZE 4

typedef enum {

    FACIL = 1,
    MEDIO,
    DIFICIL

} Dificuldade;

typedef struct {

    int board[SIZE][SIZE];

    int original[SIZE][SIZE];

    int selectedRow;

    int selectedCol;

    int erros;

    int maxErros;

    bool erroMaximo;

    bool completo;

    int blocoAtual;

    Dificuldade dificuldade;

} SudokuGame;

void InitSudoku(
    SudokuGame *game,
    Dificuldade dificuldade
);

void UpdateSudoku(
    SudokuGame *game
);

void DrawSudoku(
    SudokuGame *game
);

bool SudokuCompleto(
    SudokuGame *game
);

#endif