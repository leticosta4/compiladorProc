#ifndef TABSIMB
#define TABSIMB

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "analex.h"

#define TAM_MAX_TAB 100

typedef enum{EXTERNO_PROC, INTERNO_PROC}ESCOPO;
//typedef enum{INT, REAL, CHAR, BOOL, NAO_APLICA}TIPO;
typedef enum{VAR_GLOBAL, VAR_LOCAL, PROCEDIMENTO, PARAMETRO, PROTOTIPO}CATEGORIA;
typedef enum{VALOR, REFERENCIA, NAO_APLICA_PARAM}PASSAGEM;
typedef enum{VIVO, VIROU_ZUMBI, NAO_APLICA_ZUMBI}ZUMBI;
typedef enum{VAR_SIMPLES, ID_VETOR, ID_MATRIZ}ARRAY;

typedef struct{
    char lexema[TAM_MAX_LEXEMA];
    int escopo;
    int tipo;
    char categoria;
    int passagem;
    int zumbi;
    int array;
    int dimensao1, dimensao2;
    //endereco
    //rotulo
}registro_tabsimb;

typedef struct{
    registro_tabsimb linhas[TAM_MAX_TAB]; //inserção e remoção por pilha
    int topo;
}tipo_tab_simb;

extern tipo_tab_simb tabela_simbolos;
extern TOKEN rcv_token;

void iniciar_tabsimb();
void inserir_tabsimb();
void buscar_token_tabsimb();
void remover_tabsimb();
#endif