#include <stdio.h>
#include <time.h>
#include <locale.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#include "aviao.h"

#define TAM_LISTA 5


void *input(){
    while (1){
        gotoxy(1, 9);
        getchar();
    }
}

int main(){
    // Inicialização de variáveis
    setlocale(LC_ALL, "pt-br");
    srand(time(NULL));

    sem_t lista;
    sem_init(&lista, 0, TAM_LISTA);
    pthread_t aviao, leitor;
    
    // Introdução do jogo 
    printf("TEXTO DE INTRODUÇÃO DO JOGO\n\n"
    "Pressione enter para começar!\n");
    getchar();
    clear();
 
    printf("1. \n"
    "2. \n"
    "3. \n"
    "4. \n"
    "5. \n"
    "[PISTA LIVRE]\n\n"
    "Insira o número da posição para liberar o avião para a pista:\n");
    pthread_create(&leitor, NULL, input, NULL);
    for(int i = 0; i < TAM_LISTA + 1; i++){
        pthread_create(&aviao, NULL, criar_requisicao, &lista);
    }

    pthread_join(aviao, NULL);
    pthread_join(leitor, NULL);
    return 0;
}