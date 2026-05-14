/* =========================================================
   include/clues.h
   ========================================================= */

#ifndef CLUES_H
#define CLUES_H

typedef struct {

    const char *descricao;

    const char *pergunta;

    int respostaCorreta;

} LogicClue;

const LogicClue* GetClue(int indice);

#endif