#ifndef FUNC_AUX
#define FUNC_AUX

#include <stdbool.h>

// enum MOVIM {AVANCA, MANTEM, RETROCEDE};

// extern bool mostraArvore;
// extern char TABS[200];
extern int contLinha; //extern para garantir que a mesma variavel global sendo usada em arquivos diferentes

void error(char msg[]);
int verifica_plv_rsvd(const char lexema[]);
// void PrintNodo(char info[], int movim);
// void PrintNodoInt(int val, int movim);

#endif