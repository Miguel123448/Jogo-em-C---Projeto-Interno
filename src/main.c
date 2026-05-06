#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "raylib.h"

#define MAX_HISTORICO 100

typedef enum {
    MENU,
    JOGO,
    ESTATISTICAS,
    SAIR
} GameState;

typedef struct {
    char timestamp[20];
    int alvo;
    int tentativas;
    int baixos;
    int altos;
    int *palpites;
    int capacidade;
} FormatoHistorico;

// ===== PROTÓTIPOS =====
int lerArquivo(FormatoHistorico *historico);
void salvarPartida(FormatoHistorico partida);
FormatoHistorico iniciarPartida();

// ====================== MAIN ======================
int main() {

    srand(time(NULL));
    InitWindow(1280, 720, "Jogo de Adivinhacao");
    //ToggleFullscreen();
    SetExitKey(0);
    SetTargetFPS(60);

    GameState estado = MENU;

    const char *opcoes[3] = {"Iniciar Jogo", "Estatisticas", "Sair"};
    int opcaoSelecionada = 0;

    // INPUT
    char inputTexto[50] = "";
    int inputTamanho = 0;
    bool inputAtivo = false;
    Rectangle inputBox = {100, 200, 300, 40};

    char mensagem[50] = "Digite um numero";
    Color corMsg = BLACK;

    // HISTÓRICO
    FormatoHistorico historico[MAX_HISTORICO];
    int totalHistorico = 0;

    FormatoHistorico partida;

    while (!WindowShouldClose()) {

        if (IsKeyPressed(KEY_ESCAPE)) {
            if (estado == MENU) {
                break; // sai do jogo
            }else {
                estado = MENU; // volta pro menu
            }
        }

        // ===== INPUT MENU =====
        if (estado == MENU) {
            if (IsKeyPressed(KEY_DOWN)) opcaoSelecionada++;
            if (IsKeyPressed(KEY_UP)) opcaoSelecionada--;

            if (opcaoSelecionada < 0) opcaoSelecionada = 2;
            if (opcaoSelecionada > 2) opcaoSelecionada = 0;

            if (IsKeyPressed(KEY_ENTER)) {
                if (opcaoSelecionada == 0) {
                    partida = iniciarPartida();
                    estado = JOGO;

                    inputTexto[0] = '\0';
                    inputTamanho = 0;
                    strcpy(mensagem, "Digite um numero");
                }
                else if (opcaoSelecionada == 1) {
                    totalHistorico = lerArquivo(historico);
                    estado = ESTATISTICAS;
                }
                else {
                    estado = SAIR;
                }
            }
        }

        if (estado == SAIR) break;

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // ===== MENU =====
        if (estado == MENU) {

            DrawText("CyberCore", 500, 100, 40, DARKBLUE);

            for (int i = 0; i < 3; i++) {
                Color cor = (i == opcaoSelecionada) ? RED : DARKGRAY;
                DrawText(opcoes[i], 520, 250 + i * 40, 20, cor);
            }
        }

        // ===== JOGO =====
        else if (estado == JOGO) {

            // Botão voltar
            Rectangle botao = {100, 100, 250, 40};
            bool hover = CheckCollisionPointRec(GetMousePosition(), botao);
            DrawText("Voltar ao Menu", botao.x, botao.y, 30, hover ? RED : DARKGRAY);

            if (hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                estado = MENU;
            }

            // Clique input
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                inputAtivo = CheckCollisionPointRec(GetMousePosition(), inputBox);
            }

            // Digitação
            if (inputAtivo) {
                int tecla = GetCharPressed();

                while (tecla > 0) {
                    if (tecla >= '0' && tecla <= '9' && inputTamanho < 3) {
                        inputTexto[inputTamanho++] = (char)tecla;
                        inputTexto[inputTamanho] = '\0';
                    }
                    tecla = GetCharPressed();
                }

                if (IsKeyPressed(KEY_BACKSPACE) && inputTamanho > 0) {
                    inputTexto[--inputTamanho] = '\0';
                }

                if (IsKeyPressed(KEY_ENTER) && inputTamanho > 0) {

                    int valor = atoi(inputTexto);

                    // salva palpite
                    if (partida.tentativas >= partida.capacidade) {
                        partida.capacidade *= 2;

                        int *temp = realloc(partida.palpites, partida.capacidade * sizeof(int));
                        if (!temp) exit(1);

                        partida.palpites = temp;
                    }

                    partida.palpites[partida.tentativas++] = valor;

                    // lógica do jogo
                    if (valor > partida.alvo) {
                        strcpy(mensagem, "Muito alto!");
                        corMsg = RED;
                        partida.altos++;
                    }
                    else if (valor < partida.alvo) {
                        strcpy(mensagem, "Muito baixo!");
                        corMsg = BLUE;
                        partida.baixos++;
                    }
                    else {
                        strcpy(mensagem, "Acertou!");
                        corMsg = GREEN;

                        salvarPartida(partida);

                        free(partida.palpites);
                        estado = MENU;
                    }

                    inputTexto[0] = '\0';
                    inputTamanho = 0;
                }
            }

            DrawText(mensagem, inputBox.x, inputBox.y - 40, 20, corMsg);

            DrawRectangleRec(inputBox, LIGHTGRAY);
            DrawRectangleLinesEx(inputBox, 2, inputAtivo ? BLUE : DARKGRAY);

            DrawText(inputTexto, inputBox.x + 10, inputBox.y + 10, 20, BLACK);
        }

        // ===== ESTATISTICAS =====
        else if (estado == ESTATISTICAS) {

            DrawText("ESTATISTICAS", 450, 50, 30, DARKBLUE);

            if (totalHistorico <= 0) {
                DrawText("Nenhum historico encontrado.", 400, 200, 20, RED);
            }
            else {
                int y = 120;

                for (int i = 0; i < totalHistorico && i < 40; i++) {

                    char linha[300];

                    sprintf(linha,
                        "%s | Alvo: %d | Tentativas: %d | Altos: %d | Baixos: %d",
                        historico[i].timestamp,
                        historico[i].alvo,
                        historico[i].tentativas,
                        historico[i].altos,
                        historico[i].baixos
                    );

                    DrawText(linha, 100, y, 18, BLACK);
                    y += 30;
                }
            }

            DrawText("Pressione ESC para voltar", 400, 650, 20, GRAY);

            if (IsKeyPressed(KEY_ESCAPE)) {
                estado = MENU;
            }
        }

        EndDrawing();
    }

    // liberar memória
    for (int i = 0; i < totalHistorico; i++) {
        free(historico[i].palpites);
    }

    CloseWindow();
    return 0;
}

// ================= FUNÇÕES =================

FormatoHistorico iniciarPartida() {

    FormatoHistorico p;

    time_t t = time(NULL);
    strftime(p.timestamp, 20, "%Y-%m-%d %H:%M", localtime(&t));

    p.alvo = rand() % 100 + 1;
    p.tentativas = 0;
    p.altos = 0;
    p.baixos = 0;

    p.capacidade = 10;
    p.palpites = malloc(p.capacidade * sizeof(int));

    if (!p.palpites) exit(1);

    return p;
}

void salvarPartida(FormatoHistorico p) {

    FILE *f = fopen("historico.txt", "a");
    if (!f) return;

    fprintf(f, "%s;%d;%d;%d;%d;",
            p.timestamp, p.alvo, p.tentativas, p.baixos, p.altos);

    for (int i = 0; i < p.tentativas; i++) {
        fprintf(f, "%d", p.palpites[i]);
        if (i < p.tentativas - 1) fprintf(f, ",");
    }

    fprintf(f, "\n");
    fclose(f);
}

int lerArquivo(FormatoHistorico *h) {

    FILE *f = fopen("historico.txt", "r");
    if (!f) return 0;

    char linha[256];
    int i = 0;

    while (fgets(linha, sizeof(linha), f) && i < MAX_HISTORICO) {

        char palpitesStr[200];

        if (sscanf(linha, "%19[^;];%d;%d;%d;%d;%[^\n]",
            h[i].timestamp,
            &h[i].alvo,
            &h[i].tentativas,
            &h[i].baixos,
            &h[i].altos,
            palpitesStr) != 6) continue;

        h[i].palpites = malloc(h[i].tentativas * sizeof(int));

        char *token = strtok(palpitesStr, ",");
        int j = 0;

        while (token && j < h[i].tentativas) {
            h[i].palpites[j++] = atoi(token);
            token = strtok(NULL, ",");
        }

        i++;
    }

    fclose(f);
    return i;
}