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
int decl_list_var(char possivel_proced[]);
void decl_def_proc();
void cmd(char procedimento[]);
int atrib(char p[]);
int expr(char p[]);
int expr_simples(char p[]);
int termo(char p[]);
int fator(char p[]);
int op_rel();
//vindas do decl_list_var:
void tipo();
void decl_var(char possivel_proced[]);
//vindas do decl_def_prot:
void def();
void prot();
void passagem_end_tipo();
//vindas do cmd
void _do(char em_qual_proced[]);
void _while(char em_qual_proced[]);
#endif 