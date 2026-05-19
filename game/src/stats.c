#include "stats.h"
#include "raylib.h"
#include <stdio.h>
#include <math.h>

int SomaRecursiva(int vetor[], int n) {
    if (n == 0) return 0;
    return vetor[n - 1] + SomaRecursiva(vetor, n - 1);
}

int MinimoRecursivo(int vetor[], int n) {
    if (n == 1) return vetor[0];
    int minRestante = MinimoRecursivo(vetor, n - 1);
    return (vetor[n - 1] < minRestante) ? vetor[n - 1] : minRestante;
}

int MaximoRecursivo(int vetor[], int n) {
    if (n == 1) return vetor[0];
    int maxRestante = MaximoRecursivo(vetor, n - 1);
    return (vetor[n - 1] > maxRestante) ? vetor[n - 1] : maxRestante;
}

float SomaQuadradosRecursiva(int vetor[], int n) {
    if (n == 0) return 0;
    float quadrado = (float)(vetor[n - 1] * vetor[n - 1]);
    return quadrado + SomaQuadradosRecursiva(vetor, n - 1);
}

void GerarHeuristicaInconstancia(float mediaErros, float desvioPadrao, char* mensagemSaida) {
    if (desvioPadrao > 3.0f) {
        sprintf(mensagemSaida, "ALERTA: Padrao erratico (Desvio: %.1f). Forca bruta detectada.", desvioPadrao);
    } else {
        if (mediaErros <= 2.0f) {
            sprintf(mensagemSaida, "STATUS: Algoritmo letal e constante (Desvio: %.1f).", desvioPadrao);
        } else {
            sprintf(mensagemSaida, "CRITICO: Falhas constantes detectadas. Reveja o script.");
        }
    }
}

void AtualizarStats(GameStats *stats, int acertos, int erros, bool venceu) {
    stats->partidasJogadas++;
    stats->totalAcertos += acertos;
    stats->totalErros += erros;
    
    if (venceu) stats->vitoriasHacker++;

    if (stats->partidasJogadas <= 100) {
        stats->historicoErros[stats->partidasJogadas - 1] = erros;
    }

    int totalTentativas = stats->totalAcertos + stats->totalErros;
    if (totalTentativas > 0) {
        stats->precisaoMedia = ((float)stats->totalAcertos / totalTentativas) * 100.0f;
    }
}

void DrawHistorico(GameStats *stats) {
    DrawText("LOGS DE SISTEMA - ESTATISTICAS", 100, 80, 40, GREEN);
    
    DrawText(TextFormat("Sessoes Concluidas: %d", stats->partidasJogadas), 120, 160, 30, WHITE);
    DrawText(TextFormat("Ataques Neutralizados: %d", stats->vitoriasHacker), 120, 210, 30, SKYBLUE);
    DrawText(TextFormat("Total de Acertos (Sudoku): %d", stats->totalAcertos), 120, 260, 30, GREEN);
    DrawText(TextFormat("Total de Falhas (Sudoku): %d", stats->totalErros), 120, 310, 30, RED);
    
    DrawRectangle(120, 360, 400, 40, DARKGRAY);
    DrawRectangle(120, 360, (int)(4 * stats->precisaoMedia), 40, LIME);
    DrawText(TextFormat("Precisao do Hacker: %.1f%%", stats->precisaoMedia), 120, 410, 30, GOLD);

    if (stats->partidasJogadas > 0) {
        int n = stats->partidasJogadas;
        float mediaErros = (float)SomaRecursiva(stats->historicoErros, n) / n;
        int melhorSessao = MinimoRecursivo(stats->historicoErros, n);
        int piorSessao = MaximoRecursivo(stats->historicoErros, n);
        
        float somaQuad = SomaQuadradosRecursiva(stats->historicoErros, n);
        float variancia = (somaQuad / n) - (mediaErros * mediaErros);
        if (variancia < 0) variancia = 0; 
        float desvioPadrao = sqrt(variancia);

        char heuristica[150];
        GerarHeuristicaInconstancia(mediaErros, desvioPadrao, heuristica);

        DrawText(TextFormat("Media de Erros: %.1f | Menos Erros: %d | Mais Erros: %d", mediaErros, melhorSessao, piorSessao), 120, 480, 25, LIGHTGRAY);
        
        Color corHeuristica = (desvioPadrao > 3.0f || mediaErros > 2.0f) ? RED : LIME;
        DrawText(heuristica, 120, 520, 25, corHeuristica);
    }

    DrawText("PRESSIONE 'H' PARA VOLTAR AO MENU", 100, 620, 25, GRAY);
}