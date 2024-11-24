#include <stdio.h> 
#include <stdlib.h> 
#include <ctype.h> 
#include <string.h> 
#include <stdbool.h> 

#include "analex.h"
#include "anasint.h"
#include "funcaux.h"

void prog(){
    //iniciar a tabrla de simbolos
    if(mostra_arvore){ print_nodo_char("<prog>", AVANCA); }

    while(rcv_token.categoria == PLV_RSVD && (rcv_token.codigo == CONST || rcv_token.codigo == INT || rcv_token.codigo == CHAR || rcv_token.codigo == REAL )){
        decl_list_var();
    }  
    if(mostra_arvore){ print_nodo_char("< %s >", palavras_reservadas[rcv_token.codigo], MANTEM); }

    while(rcv_token.categoria == PLV_RSVD && rcv_token.codigo == PR){
        decl_proc_prot();
    }  
    if(mostra_arvore){ print_nodo_char("< %s >", palavras_reservadas[rcv_token.codigo], MANTEM); }


    if(!(rcv_token.categoria == PLV_RSVD && rcv_token.codigo == INIT)){
        error("Declaração no bloco init esperada");
    } init();


    while (rcv_token.categoria == PLV_RSVD && rcv_token.codigo == PR) {
        proc_def();
    }  
    if(mostra_arvore){ print_nodo_char("< %s >", palavras_reservadas[rcv_token.codigo], MANTEM); }

    if(rcv_token != FINAL_ARQ){ error("Fim do arquivo esperado"); }

    printf("\n\nexpressao linha %d foi\n\n", contLinha);

    if(mostra_arvore){ print_nodo_char("", RETROCEDE); }
    //algo da tabela de simbolos 
}

void decl_list_var(){
    if(mostra_arvore){ print_nodo_char("<decl_list_var>", AVANCA); }

    if(rcv_token.codigo == CONST){
        rcv_token.processado = true;
        if(mostra_arvore){ print_nodo_char("<const>", MANTEM); }
    }

    tipo();
    decl_var();

    while(rcv_token.codigo == VIRGULA){
        rcv_token.processado = true;
        //acho q tem q fzr alguma coisa p ler o proximo token e continuar
        decl_var();   
    }

    if(mostra_arvore){ print_nodo_char("fim declaração lista de variáveis", RETROCEDE); }
}


void decl_proc_prot(){}
void init(){}
void proc_def(){}
//vindas do decl_lis_var:
void tipo(){}
void decl_var(){}