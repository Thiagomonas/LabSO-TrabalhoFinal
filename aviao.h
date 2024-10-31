#define TAM_LISTA 5
#define MIN_COOLDOWN 3

#define gotoxy(x, y) printf("\033[%d;%dH", (y), (x));
#define gotoend() printf("\033[9;1H");
#define clear() printf("\033[H\033[J");
#define clearline(x, y) printf("\033[%d;%dH\033[K", (y), (x));

void *criar_requisicao(void *);
void *liberar_pista(void *);