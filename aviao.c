#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

#include "aviao.h"

int em_espera = 0;

void adicionar_lista(int pos_lista, sem_t *terminal){
    sem_wait(terminal);

    gotoxy(3, pos_lista);
    printf("avião %d", pos_lista);
    gotoend();
    fflush(stdout);

    FILE *arq = fopen("lista.txt", "a");
    fprintf(arq, "avião %d\n", pos_lista);
    fclose(arq);

    sem_post(terminal);
}

void *criar_requisicao(void *arg){
    sem_t *terminal = (sem_t *) arg;

    while (em_espera < TAM_LISTA){
        sleep(MIN_COOLDOWN + (rand() % 3));
        
        em_espera++;
        adicionar_lista(em_espera, terminal);
    }
    // Estourou a lista
    printf("Muitos aviões em espera, lamentável de vdd\n");
    exit(0);
}

void remover_linha(int pos_lista){
    char buf[50];
    FILE *arq = fopen("lista.txt", "r");
    FILE *tmp = fopen("tmp.txt", "w");

    int linha = 1;
    while(fgets(buf, sizeof(buf), arq) != NULL){
        if (linha != pos_lista){
            fputs(buf, tmp);
        }
        linha++;
    }

    fclose(arq);
    fclose(tmp);
    remove("lista.txt");
    rename("tmp.txt", "lista.txt");
}

void atualizar_lista(){
    char buf[50];
    FILE *arq = fopen("lista.txt", "r");
    
    int linha = 1;
    while (fgets(buf, sizeof(buf), arq) != NULL){
        gotoxy(3, linha);
        printf("%s", buf);
        linha++;
    }

    while (linha <= TAM_LISTA){
        // Limpar linhas
        printf("\033[%d;3H\033[K", linha);
        linha++;
    }
    gotoend();
}

void *liberar_pista(void *arg){
    sem_t *terminal = (sem_t *) arg;

    while (1){
        int pos_lista = getchar() - 48;

        if (pos_lista < 1 || pos_lista > 5) {
            gotoend();
            continue;
        }
        sem_wait(terminal);
        em_espera--;
        remover_linha(pos_lista);
        atualizar_lista();
        sem_post(terminal);
    }

    exit(0);
}