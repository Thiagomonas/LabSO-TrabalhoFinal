#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <string.h>

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
    int *status = (int *) malloc(sizeof(int));
    *status = 1;
    return status;
}

char *remover_linha(int pos_lista){
    char buf[50];
    char *nome = NULL;
    FILE *arq = fopen("lista.txt", "r");
    FILE *tmp = fopen("tmp.txt", "w");

    int linha = 1;
    while(fgets(buf, sizeof(buf), arq) != NULL){
        if (linha != pos_lista){
            fputs(buf, tmp);
        }
        else {
            nome = (char *) malloc(strlen(buf));
            int i = 0;
            while(buf[i] != '\n'){
                nome[i] = buf[i];
                i++;
            }
            nome[i] = '\0';
        }
        linha++;
    }

    fclose(arq);
    fclose(tmp);
    remove("lista.txt");
    rename("tmp.txt", "lista.txt");

    return nome;
}

void atualizar_lista(char *nome){
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
        clearline(3, linha);
        linha++;
    }
    clearline(1, 6);
    gotoxy(1, 6);
    printf("%s está na pista!", nome);
    gotoend();
}

void *liberar_pista(void *arg){
    sem_t *terminal = (sem_t *) arg;

    while (1){
        char input = getchar();
        int pos_lista = input - 48;

        if (input == 'q'){
            break;
        }

        if (pos_lista < 1 || pos_lista > 5) {
            gotoend();
            continue;
        }
        sem_wait(terminal);
        char *nome = remover_linha(pos_lista);
        if (nome != NULL){
            em_espera--;
            atualizar_lista(nome);
        }
        sem_post(terminal);
    }

    exit(0);
}