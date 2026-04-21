#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
//#include <raylib.h>

typedef struct formatoHistorico{
    char timestamp[20]; //dependendo de como vamos fazer o tempo isso aqui pode mudar
    int alvo;
    int tentativas;
    int baixo;
    int altos;
    int *palpites;
} FormatoHistorico;

int gerarNumeroAleatorio() {
    return (rand() % 100) + 1;
}

int lerArquivo(FormatoHistorico *structHistorico){

    FILE *arquivo = fopen("historico.txt","r");

    if(arquivo == NULL){
        return -1;
    }

    char linha[256];
    int i = 0;

    while(fgets(linha, sizeof(linha), arquivo) != NULL){

        char palpitesStr[200];

        // faz o parse da linha
        if(sscanf(linha, "%[^;];%d;%d;%d;%d;%[^\n]",
               structHistorico[i].timestamp,
               &structHistorico[i].alvo,
               &structHistorico[i].tentativas,
               &structHistorico[i].baixo,
               &structHistorico[i].altos,
               palpitesStr) != 6){
            continue; // pula linha inválida
        }

        // aloca memória para os palpites
        structHistorico[i].palpites =
            malloc(structHistorico[i].tentativas * sizeof(int));

        // separa CSV dos palpites
        char *token = strtok(palpitesStr, ",");
        int j = 0;

        while(token != NULL && j < structHistorico[i].tentativas){
            structHistorico[i].palpites[j]= atoi(token);
            token = strtok(NULL, ","); 
            j++;
        }

        i++;
    }

    fclose(arquivo);
    return i; // quantidade de linhas lidas
}

void salvarPartida(FormatoHistorico partida){

    FILE *arquivo = fopen("historico.txt", "a");

    if(arquivo == NULL){
        printf("Erro ao abrir arquivo!\n");
        return;
    }

    fprintf(arquivo, "%s;%d;%d;%d;%d;",
            partida.timestamp,
            partida.alvo,
            partida.tentativas,
            partida.baixo,
            partida.altos);

    for(int i = 0; i < partida.tentativas; i++){
        fprintf(arquivo, "%d", partida.palpites[i]);
        if(i < partida.tentativas - 1){
            fprintf(arquivo, ",");
        }
    }

    fprintf(arquivo, "\n");

    fclose(arquivo);
}

int main(){

    srand(time(NULL));  //Necessario para a funcao gerarNumeroAleatorio

}

/*
formato: timestamp;alvo;tentativas;baixos;altos;palpites_csv.
        2026-03-31 16:35:10;42;7;4;2;10,80,50,40,45,43,42

*/