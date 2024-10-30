#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

#include "aviao.h"

const char *nomes[NUM_NOMES] = {"avião 1", "avião 2", "avião 3", "avião 4", "avião 5"};

int len(char *str){
    int i = 1;
    while (str[i] != '\0'){
        i++;
    }
    return i;
}

char *criar_aviao(int *emergencia){
    *emergencia = rand() % 2;

    int i = rand() % NUM_NOMES;
    int tam = len(nomes[i]);
    char *nome = (char *) malloc(sizeof(char) * tam);
    for (int j = 0; j < tam; j++){
        nome[j] = nomes[i][j];
    }
    return nome;
}

void adicionar_lista(int pos_lista, char* nome, int emergencia){
    gotoxy(3, pos_lista);
    printf("%s", nome);
    if (emergencia){
        printf(" - [EMERGÊNCIA]");
    }
    gotoxy(1, 9);
    fflush(stdout);
}

void *criar_requisicao(void *args){
    int tempo = COOLDOWN;
    int val, val_ant;
    sem_t *lista = (sem_t *) args;
    
    sem_getvalue(lista, &val);
    while (tempo > 0){
        val_ant = val;
        sleep(1);
        sem_getvalue(lista, &val);

        // Outro avião foi adicionado na lista -> Resetar Cooldown
        if (val_ant != val){
            tempo = COOLDOWN;
            continue;
        }
        tempo--;
    }
    if(sem_trywait(lista)){
        // Toda a lista foi ocupada
        // PERDEU O JOGO
        return 1;
    }

    int pos_lista = 6 - val;
    int emergencia;
    char *nome = criar_aviao(&emergencia);
    adicionar_lista(pos_lista, nome, emergencia);
}

