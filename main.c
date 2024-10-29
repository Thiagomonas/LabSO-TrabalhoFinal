#include <stdio.h>
#include <locale.h>

#define clear() printf("\033[H\033[J");
#define gotoxy(x, y) printf("\033[%d;%dH", (y), (x));

int main(){
    setlocale(LC_ALL, "pt-br");

    printf("TEXTO DE INTRODUÇÃO DO JOGO\n\n"
    "Pressione enter para começar!\n");
    getchar();
    clear();
}