#include "stats.h"
#include "raylib.h"
#include <stdio.h>

void AtualizarStats(GameStats *stats, int acertos, int erros, bool venceu) {
    stats->partidasJogadas++;
    stats->totalAcertos += acertos;
    stats->totalErros += erros;
    if (venceu) stats->vitoriasHacker++;

    int totalTentativas = stats->totalAcertos + stats->totalErros;
    if (totalTentativas > 0) {
        stats->precisaoMedia = ((float)stats->totalAcertos / totalTentativas) * 100.0f;
    }
}

void DrawHistorico(GameStats *stats) {
    DrawText("LOGS DE SISTEMA - ESTATISTICAS", 100, 80, 40, GREEN);
    
    DrawText(TextFormat("Sessoes Concluidas: %d", stats->partidasJogadas), 120, 180, 30, WHITE);
    DrawText(TextFormat("Ataques Bem-Sucedidos: %d", stats->vitoriasHacker), 120, 230, 30, SKYBLUE);
    DrawText(TextFormat("Total de Acertos (Sudoku): %d", stats->totalAcertos), 120, 280, 30, GREEN);
    DrawText(TextFormat("Total de Falhas (Sudoku): %d", stats->totalErros), 120, 330, 30, RED);
    
    DrawRectangle(120, 400, 400, 40, DARKGRAY);
    DrawRectangle(120, 400, (int)(4 * stats->precisaoMedia), 40, LIME);
    DrawText(TextFormat("Precisao do Hacker: %.1f%%", stats->precisaoMedia), 120, 450, 30, GOLD);

    DrawText("PRESSIONE 'H' PARA VOLTAR AO MENU", 100, 600, 25, GRAY);
}