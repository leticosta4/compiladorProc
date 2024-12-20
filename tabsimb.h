#ifndef TABSIMB
#define TABSIMB

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "analex.h"

#define TAM_MAX_TAB 1000
#define TAM_MAX_DIM 3

typedef enum escopo{GLOBAL, LOCAL}ESCOPO;
typedef enum tipo{_NAO_APLICA_TIPO, _INT, _REAL, _CHAR, _BOOL}TIPO;
typedef enum categoria{VAR_GLOBAL, VAR_LOCAL, PROCEDIMENTO, PARAMETRO, PROTOTIPO}CATEGORIA;
typedef enum passagem{NAO_APLICA_PARAM, VALOR, REFERENCIA}PASSAGEM;
typedef enum zumbi {NAO_APLICA_ZUMBI, VIVO, EH_ZUMBI}ZUMBI;
typedef enum array{NAO_APLICA_ARRAY, VAR_SIMPLES, ID_VETOR, ID_MATRIZ}ARRAY;
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
registro_tabsimb limpar_dimensoes_array(registro_tabsimb used_token);
void verifica_redeclaracao(registro_tabsimb token_aux);
int procura_posicao_proc(char nome_def[]);
int procura_existencia_prot(char nome_prot[]);
void remover_tabsimb();
void substituir_prot_proc(int posicao_prot, registro_tabsimb token_proced);
void substituir_parametros_prot_proc(int posicao_prot, registro_tabsimb token_param_proced);
void apagar_var_locais(int posicao_def);
void transformar_zumbi(int posicao_def);
void printar_tabsimb();
#endif