#include <stdio.h> 
#include <stdlib.h> 
#include <ctype.h> 
#include <string.h> 
#include <stdbool.h> 
#include "analex.h"
#include "funcaux.h"
#include "tabsimb.h"


void error(char msg[]){ 
    printf("%s na linha %d\n", msg, contLinha); 
    exit(1); 
} 

int verifica_plv_rsvd(const char lexema[]){
    for(int i = 1; i <= NUM_PLV_RSVD; i++){
        if(strcmp(lexema, palavras_reservadas[i-1]) == 0){ return i; } //retorna o cod da plv_rsvd p associar no enum
    }
    return -1;
}

void consome_fim_exp(){
    while(rcv_token.categoria == FINAL_EXP){ rcv_token = AnaLex(arqivoProc); }
}

void debug(char onde[]){
    printf("\n\n%s: cat: %d | codigo: %d\n\n", onde, rcv_token.categoria, rcv_token.codigo);
}

void busca_erro_decl_var_dps_decl_prot_proc_ou_cmd(TOKEN sus_token, int escopo_var){
    if(sus_token.categoria == PLV_RSVD && (sus_token.codigo == CONST || sus_token.codigo == INT || sus_token.codigo == REAL || sus_token.codigo == CHAR || sus_token.codigo == BOOL)){
        switch(escopo_var){
            case GLOBAL:
                error("\nERRO SEMANTICO > a declaração de variável(is) global(is) deve ocorrer no iníco do programa, antes declaração de prototipo(s)/procedimento(s)");
                break;
            case LOCAL:
                error("\nERRO SEMANTICO > a declaração de variável(is) local(is) deve ocorrer no iníco do procedimento, antes de comando(s)");
                break;
        }
    }
}

int associa_tipos_compat(int tipo_base, int tipo_cmp){
    if(((tipo_base == _INT || tipo_base == _BOOL || tipo_base == _CHAR) && (tipo_cmp != _INT && tipo_cmp != _CHAR && tipo_cmp != _BOOL)) ||
        tipo_base == _REAL && tipo_cmp != _REAL){
            return -1;
    } 
    return 0;
}

//ver como modelar ainda
int gera_label(){
    static int cont_label = 1; //a 1 ja foi colocada estaticamente 
    cont_label++;
    return cont_label; //esse retorno vai ser concatenado com o 
}

int busca_retorna_label(char nome_busca[]){
    for(int i = 0; i <= tabela_simbolos.topo; i++){
        if(strcmp(tabela_simbolos.linhas[i].lexema, nome_busca) == 0){ return tabela_simbolos.linhas[i].rotulo; }}
}

registro_tabsimb atribui_endereco_var(registro_tabsimb token_var){
    static int contVG = 0, contVL = 0;
    if(token_var.categoria <= 1){
        switch(token_var.escopo){
            case GLOBAL:
                token_var.endereco[0] = 0;
                token_var.endereco[1] = contVG;
                contVG++;
                break;
            case LOCAL:
                token_var.endereco[0] = 1;
                token_var.endereco[1] = contVL;
                contVL++;
                break;
        }
    }
    return  token_var;
}