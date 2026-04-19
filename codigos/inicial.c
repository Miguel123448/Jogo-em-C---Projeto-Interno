#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
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

#include <string.h>  // strtok
#include <stdlib.h>  // malloc, atoi

int lerArquivo(FormatoHistorico *structHistorico){

    int qtdLinhas;
    FILE *arquivo = fopen("historico.txt","r");

    if(arquivo == NULL){
        return -1;
    }

    // lê a quantidade de registros
    fscanf(arquivo, "%d\n", &qtdLinhas);

    for(int i = 0; i < qtdLinhas; i++){

        char palpitesStr[100];

        // lê cada linha formatada
        fscanf(arquivo, "%[^;];%d;%d;%d;%d;%[^\n]\n",
               structHistorico[i].timestamp,
               &structHistorico[i].alvo,
               &structHistorico[i].tentativas,
               &structHistorico[i].baixo,
               &structHistorico[i].altos,
               palpitesStr);

        // aloca memória para os palpites
        structHistorico[i].palpites = malloc(structHistorico[i].tentativas * sizeof(int));

      //TA FALTANDO PEGAR O PALPITES_CSV.
    }

    fclose(arquivo);
    return qtdLinhas;
}

int main(){

    srand(time(NULL));  //Necessario para a funcao gerarNumeroAleatorio

}

/*
formato: timestamp;alvo;tentativas;baixos;altos;palpites_csv.
        2026-03-31 16:35:10;42;7;4;2;10,80,50,40,45,43,42

*/