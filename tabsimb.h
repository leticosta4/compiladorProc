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
typedef enum tem_prot{NAO_APLICA_PROT, _NAO, _SIM}TEM_PROT;

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
    int rotulo;
    TEM_PROT tem_prototipo;
}registro_tabsimb;

typedef struct{
    registro_tabsimb linhas[TAM_MAX_TAB]; //inserção e remoção por pilha, busca linear
    int topo;
}tipo_tab_simb;

extern tipo_tab_simb tabela_simbolos;
extern TOKEN rcv_token;

void inserir_tabsimb(registro_tabsimb token_ins);
registro_tabsimb limpar_dimensoes_array(registro_tabsimb used_token);
void verifica_redeclaracao(registro_tabsimb token_aux);
void verifica_redecl_param(int pos_proced, char nome_param[]);
int procura_posicao_proc(char nome_def[]);
int procura_existencia_prototipo_ou_proced(char nome_prot[]);
void remover_tabsimb();
void substituir_prot_proc(int posicao_prot, registro_tabsimb token_proced);
void substituir_parametros_prot_proc_testar_compat_tipos(int posicao_prot, registro_tabsimb token_param_proced, int flag_veio_do);
int contar_params(int posicao_prot_def);
void transformar_zumbi(int posicao_def);
void printar_tabsimb();
void verifica_compatibilidade_tipo_decl(TOKEN tk, registro_tabsimb info_tk);
void prototipo_sozinho();
registro_tabsimb procura_existencia_identificador_em_proced(int posicao_procedimento, char nome_identificador[]);
void apagar_var_locais();
#endif