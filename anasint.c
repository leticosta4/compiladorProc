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

    rcv_token = AnaLex(arqivoProc);

    if(rcv_token.categoria == PLV_RSVD){
        while(rcv_token.codigo == CONST || rcv_token.codigo == INT || rcv_token.codigo == CHAR || rcv_token.codigo == REAL){
            rcv_token.processado = true;
            decl_list_var();
        } 
        while(rcv_token.codigo == PROT || rcv_token.codigo == DEF) {
            rcv_token.processado = true;
            decl_def_proc();
        }
    } 

    if(rcv_token.codigo != FINAL_ARQ){ error("Fim do arquivo esperado"); }

    printf("\n\nexpressao linha %d foi\n\n", contLinha);
    //algo da tabela de simbolos 
}

void decl_list_var(){
    rcv_token = AnaLex(arqivoProc);
    if(rcv_token.categoria == FINAL_ARQ){
        rcv_token.processado = true;
        printf("\nFim do arquivo fonte encontrado!\n");
        //return;
    }

    if(rcv_token.codigo == CONST){ rcv_token.processado = true; }

    tipo();
    decl_var();

    while(rcv_token.codigo == VIRGULA){
        rcv_token.processado = true;
        //acho q tem q fzr alguma coisa p ler o proximo token e continuar
        decl_var();   
    }

    if(mostra_arvore){ print_nodo_char("fim declaração lista de variáveis", RETROCEDE); }
}


void decl_def_proc(){}
void init(){}
void proc_def(){}
//vindas do decl_lis_var:
void tipo(){}
void decl_var(){}