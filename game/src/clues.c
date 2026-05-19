/* =========================================================
   src/clues.c
   ========================================================= */

#include "clues.h"
#include <stddef.h>

LogicClue clues[] = {

    {
        "SISTEMA: Analise de Protocolo Paridade\n"
        "------------------------------------------\n"
        "REGRA: Se a Frequencia e PAR (P), o LED Azul pisca (Q).\n"
        "LOGICA: P -> Q | ~Q\n"
        "\n"
        "STATUS DO HARDWARE:\n"
        "  >> LED Azul encontra-se [APAGADO].\n"
        "------------------------------------------",

        "QUAL E A CONCLUSAO?\n"
        "\n"
        "[1] P\n"
        "[2] ~P\n"
        "[3] Q",

        2
    },

    {
        "SISTEMA: Scanner de Digito Final\n"
        "------------------------------------------\n"
        "REGRA: O ultimo digito e PAR (P) OU PRIMO (Q).\n"
        "LOGICA: P v Q | ~P\n"
        "\n"
        "STATUS DO HARDWARE:\n"
        "  >> Verificador de Paridade retornou [0].\n"
        "------------------------------------------",

        "QUAL E A CONCLUSAO?\n"
        "\n"
        "[1] ~Q\n"
        "[2] P\n"
        "[3] Q",

        3
    },

    {
        "SISTEMA: Filtro de Potencia de Sinal\n"
        "------------------------------------------\n"
        "REGRA: Se o sinal e FORTE (P), entao Valor > 500 (Q).\n"
        "LOGICA: P -> Q | P\n"
        "\n"
        "STATUS DO HARDWARE:\n"
        "  >> Sensor detectou Barra [VERDE].\n"
        "------------------------------------------",

        "QUAL E A CONCLUSAO?\n"
        "\n"
        "[1] Q\n"
        "[2] ~Q\n"
        "[3] P",

        1
    },

    {
        "SISTEMA: Verificador de Integridade\n"
        "------------------------------------------\n"
        "REGRA: Se o 2º digito e 0 (P), o Firewall trava (Q).\n"
        "LOGICA: P -> Q | ~Q\n"
        "\n"
        "STATUS DO HARDWARE:\n"
        "  >> Firewall operando SEM erro 'ZERO_DIV'.\n"
        "------------------------------------------",

        "QUAL E A CONCLUSAO?\n"
        "\n"
        "[1] P\n"
        "[2] ~P\n"
        "[3] Q",

        2
    },

    {
        "SISTEMA: Reconstrucao Sequencial\n"
        "------------------------------------------\n"
        "REGRA: Se o 1º e 7 (P), o 2º e 3 (Q) e o 3º e 9 (R).\n"
        "LOGICA: P -> Q -> R\n"
        "\n"
        "STATUS DO HARDWARE:\n"
        "  >> Entrada [7] confirmada no Kernel.\n"
        "------------------------------------------",

        "QUAL VALOR FOI CONFIRMADO NO SISTEMA?\n"
        "\n"
        "[1] 3\n"
        "[2] 9\n"
        "[3] 7",

        3
    }

};

const LogicClue* GetClue(int indice) {

    if (indice < 0) {
        return NULL;
    }

    if (indice >= 5) {
        indice = 4;
    }

    return &clues[indice];
}