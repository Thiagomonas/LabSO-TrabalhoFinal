#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <string.h>
#include <pthread.h>

#include "aviao.h"

pthread_t timer;
sem_t pista;

char *nomes[MAX_NOMES] = {"Airbus A318 NEO", "Airbus A319 NEO", "Airbus A320 NEO", "Airbus A321 NEO",
"Airbus A330", "Airbus A350", "Airbus A380", "Boeing 737-8 MAX", "Boeing 747-8", "Boeing 757",
"Boeing 767", "Boeing 777", "Boeing 787", "Embraer E195-E2", "Embraer E175-E2", "Embraer E190-E2"};
int em_espera = 0;

int min(int x, int y){
    if (x < y){
        return x;
    }
    return y;
}

char *selec_nome(){
    int i = rand() % MAX_NOMES;
    char *nome = (char *) malloc(sizeof(strlen(nomes[i])));
    int j = 0;
    while(nomes[i][j] != '\0'){
        nome[j] = nomes[i][j];
        j++;
    }
    nome[j] = '\0';

    return nome;
}

int adicionar_lista(char *nome, int pos_lista, sem_t *terminal){
    int emergencia = rand() % 5;
    sem_wait(terminal);

    gotoxy(3, pos_lista);
    if (!emergencia){
        // Emergência
        printf("%s - [EMERGÊNCIA]", nome);
    }
    else {
        printf("%s", nome);
    }
    gotoend();
    fflush(stdout);

    FILE *arq = fopen("lista.txt", "a");
    fprintf(arq, "%s\n", nome);
    fclose(arq);

    sem_post(terminal);

    return emergencia;
}

int tratar_emergencia(int pos_lista, char *nome){
    int tempo = 8;
    char buf[50];
    while (tempo > 0){
        FILE *arq = fopen("lista.txt", "r");
        int linha = 1;
        while (fgets(buf, sizeof(buf), arq) != NULL){
            int tam = strlen(buf) / sizeof(char);
            buf[tam - 1] = '\0';
            if (linha == pos_lista && strcmp(buf, nome)){
                fclose(arq);
                return 0;
            }
            linha++;
        }
        if (linha <= pos_lista){
            fclose(arq);
            return 0;
        }
        sleep(1);
        tempo--;
        fclose(arq);
    }
    // Tempo estourado
    free(nome);
    printf("Emergência não atendida! Você perdeu o jogo!\n");
    remove("lista.txt");
    exit(0);
}

void *criar_requisicao(void *arg){
    sem_t *terminal = (sem_t *) arg;
    char *nome = NULL;
    int num_avioes = 0;
    int nivel = 0;

    while (em_espera <= TAM_LISTA){
        if (num_avioes % 5 == 4){(nivel = min(nivel + 1, 3));}
        sleep(MIN_COOLDOWN + (rand() % 3) - nivel);
        if (em_espera == TAM_LISTA){
            break;
        }
        
        em_espera++;
        nome = selec_nome();
        int emergencia = adicionar_lista(nome, em_espera, terminal);
        if (!emergencia){
            emergencia = tratar_emergencia(em_espera, nome);
        }
        num_avioes++;
    }
    // Estourou a lista
    printf("Muitos aviões em espera! Você Perdeu!\n");
    remove("lista.txt");
    exit(0);
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
        clearline(3, linha);
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
    fflush(stdout);
}

void *tempo_pista(){
    sem_wait(&pista);
    sleep(TEMPO_PISTA);

    clearline(1, 6);
    gotoxy(1, 6);
    printf("[PISTA VAZIA]");
    gotoend();
    fflush(stdout);

    sem_post(&pista);
    return 0;
}

void *liberar_pista(void *arg){
    sem_t *terminal = (sem_t *) arg;
    sem_init(&pista, 0, 1);

    while (1){
        char input = getchar();
        int pos_lista = input - 48;

        // Tecla para sair do jogo
        if (input == 'q'){
            remove("lista.txt");
            printf("Você desistiu! Obrigado por jogar!\n");
            exit(0);
        }

        if (pos_lista < 1 || pos_lista > 5) {
            gotoend();
            continue;
        }
        sem_wait(terminal);
        char *nome = remover_linha(pos_lista);
        
        // Verifica se há um avião na posição
        if (nome != NULL){
            if(sem_trywait(&pista)){
                // Pista já está sendo utilizada
                remove("lista.txt");
                printf("Há mais de um avião na pista! Você perdeu!\n");
                exit(0);
            }
            sem_post(&pista);
            em_espera--;
            atualizar_lista(nome);
            pthread_create(&timer, NULL, tempo_pista, NULL);
        }
        sem_post(terminal);
    }

    exit(0);
}