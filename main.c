#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <locale.h>

#include "aviao.h"

pthread_t aviao, controlador;
sem_t terminal;

int main(){
    setlocale(LC_ALL, "pt-br");
    srand(time(NULL));
    sem_init(&terminal, 0, 1);

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

    pthread_create(&aviao, NULL, criar_requisicao, &terminal);
    pthread_create(&controlador, NULL, liberar_pista, &terminal);

    pthread_join(aviao, NULL);
    pthread_join(controlador, NULL);
    remove("lista.txt");
}