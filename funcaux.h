#ifndef FUNC_AUX
#define FUNC_AUX

#include <stdlib.h> 
#include <stdbool.h> 

typedef enum{
    AVANCA,
    MANTEM,
    RETROCEDE
}MOVIMENTO_ARVORE;

extern bool mostra_arvore;
extern char TABS[200];
extern int contLinha; //extern para garantir que a mesma variavel global sendo usada em arquivos diferentes

void error(char msg[]);
int verifica_plv_rsvd(const char lexema[]);
void printa_valor_token();
// void print_nodo_char(char info[], int mov);
// void print_nodo_int(char val, int mov);

#endif