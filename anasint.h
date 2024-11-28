#ifndef ANASINT
#define ANASINT

#include <stdio.h>
#include <stdbool.h>

#include "analex.h"
//#include "funcaux.h"

#define TAM_MAX_LEXEMA 50

extern TOKEN rcv_token;
extern FILE *arqivoProc;
extern int contLinha;  
extern char palavras_reservadas[NUM_PLV_RSVD][TAM_MAX_LEXEMA];
extern bool mostra_arvore;
extern char TABS[200];

int valor_var();
void prog();
void decl_list_var();
void decl_def_proc();
void init();
void proc_def();
//vindas do decl_lis_var:
void tipo();
void decl_var();
#endif 