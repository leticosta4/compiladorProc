#include <stdio.h> 
#include <stdlib.h> 
#include <ctype.h> 
#include <string.h> 
#include <stdbool.h> 
#include "analex.h"
#include "funcaux.h"

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