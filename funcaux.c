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
    if(sus_token.categoria == PLV_RSVD && (sus_token.codigo == INT || sus_token.codigo == REAL || sus_token.codigo == CHAR || sus_token.codigo == BOOL)){
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

//temporario para a parte da GERACAO DE CODIGO - MAQUINA DE PILHA
// void gera_label(char *label){
//     char strcont[10];
//     static int cont = 1;

//     strcpy(label, "L");
//     itoa(cont, strcont, 10);
//     strcat(label, strcont);
//     cont++;
// }