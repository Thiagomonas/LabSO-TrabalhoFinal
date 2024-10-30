#define NUM_NOMES 5
#define COOLDOWN 5
#define gotoxy(x, y) printf("\033[%d;%dH", (y), (x));
#define clear() printf("\033[H\033[J");

void *criar_requisicao();