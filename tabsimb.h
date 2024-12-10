#ifndef TABSIMB
#define TABSIMB

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "analex.h"

#define TAM_MAX_TAB 1000
#define TAM_MAX_DIM 3

typedef enum escopo{EXTERNO_PROC, INTERNO_PROC}ESCOPO;
typedef enum tipo{_INT, _REAL, _CHAR, _BOOL, _NAO_APLICA_TIPO}TIPO;
typedef enum categoria{VAR_GLOBAL, VAR_LOCAL, PROCEDIMENTO, PARAMETRO, PROTOTIPO}CATEGORIA;
typedef enum passagem{VALOR, REFERENCIA, NAO_APLICA_PARAM}PASSAGEM;
typedef enum zumbi {VIVO, EH_ZUMBI, NAO_APLICA_ZUMBI}ZUMBI;
typedef enum array{VAR_SIMPLES, ID_VETOR, ID_MATRIZ, NAO_APLICA_ARRAY}ARRAY;
typedef enum eh_const{NAO, SIM}EH_CONST;

typedef struct valor_const{
    union {
        int inteiro; 
        float real; 
        char v_char;
        bool v_bool;
    };
}VALOR_CONST;

typedef struct{
    char lexema[TAM_MAX_LEXEMA];
    ESCOPO escopo;
    TIPO tipo;
    CATEGORIA categoria;
    PASSAGEM passagem;
    ZUMBI zumbi;
    ARRAY array;
    int dimensoes_array[2];
    EH_CONST constante;
    VALOR_CONST valor_constante;
    int endereco;
    //rotulo;
}registro_tabsimb;

typedef struct{
    registro_tabsimb linhas[TAM_MAX_TAB]; //inserção e remoção por pilha, busca linear
    int topo;
}tipo_tab_simb;

extern tipo_tab_simb tabela_simbolos;
extern TOKEN rcv_token;

void iniciar_tabsimb();
void inserir_tabsimb(registro_tabsimb token_ins);
void verifica_redeclaracao(registro_tabsimb token_aux);
// int buscar_posicao_prox_token();
//fazer funcao de resetar o info_token
registro_tabsimb verifica_declaracao(); //prototipo de procedimento, constante e variavel ?
void remover_tabsimb();
void printar_tabsimb();
#endif