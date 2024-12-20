#ifndef FUNC_AUX
#define FUNC_AUX

#include <stdlib.h> 
#include <stdbool.h> 

extern int contLinha; //extern para garantir que a mesma variavel global sendo usada em arquivos diferentes

void error(char msg[]);
int verifica_plv_rsvd(const char lexema[]);
void consome_fim_exp();
void debug(char onde[]);
#endif