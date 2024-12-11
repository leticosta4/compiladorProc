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

int valor_var();
void confere_atrib_constante();
void prog();
void decl_list_var();
void decl_def_proc();
void cmd();
void atrib();
void expr();
void expr_simples();
void termo();
void fator();
int op_rel();
//vindas do decl_list_var:
void tipo();
void decl_var();
//vindas do decl_def_prot:
void def();
void prot();
void passagem_end_tipo();
#endif 