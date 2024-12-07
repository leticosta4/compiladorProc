#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "tabsimb.h"

char escopos[2][TAM_MAX_LEXEMA] = {"externo_proc", "interno_proc"};
char tipos[5][TAM_MAX_LEXEMA] = {"_int", "_real", "_char", "_bool", "_nao_aplica_tipo"};
char categorias[5][TAM_MAX_LEXEMA] = {"var_global", "var_local", "procedimento", "parametro", "prototipo"};
char passagens[3][TAM_MAX_LEXEMA] = {"valor", "referencia", "nao_aplica_param"};
char zumbis[3][TAM_MAX_LEXEMA] = {"vivo", "eh_zumbi", "nao_aplica_zumbi"};
char arrays[3][TAM_MAX_LEXEMA] = {"var_simples", "id_vetor", "id_matriz"};
//fazer a funcao que correlaciona esses arrays com os enums

void iniciar_tabsimb(){
    tabela_simbolos.topo = 0; //-1;
}

void inserir_tabsimb(){}
void buscar_token_tabsimb(){}
void remover_tabsimb(){}