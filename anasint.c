#include <stdio.h> 
#include <stdlib.h> 
#include <ctype.h> 
#include <string.h> 
#include <stdbool.h> 

#include "analex.h"
#include "anasint.h"
#include "funcaux.h"
#include "tabsimb.h"

tipo_tab_simb tabela_simbolos;

void valor_var(){
    if(rcv_token.categoria == CHARCON || rcv_token.categoria == INTCON || rcv_token.categoria == REALCON || rcv_token.categoria == STRINGCON){
        rcv_token.processado = true;
        rcv_token = AnaLex(arqivoProc);
        //adicionar na tabela de simb?
    } else {
        error("era esperado valor charcon ou intcont ou realcon ou stringcon");
    }
}

void prog(){
    iniciar_tabsimb();

    printf("inicio do programa: < prog >\n\n");
    if(rcv_token.categoria == PLV_RSVD){
        while(rcv_token.codigo == CONST || rcv_token.codigo == INT || rcv_token.codigo == CHAR || rcv_token.codigo == REAL || rcv_token.codigo == BOOL){
            rcv_token.processado = true;
            rcv_token = AnaLex(arqivoProc);
            decl_list_var();
        } 
        while(rcv_token.codigo == PROT || rcv_token.codigo == DEF) {
            //aqui talvez nao seja bom ja consumir o token pq eu preciso fazer uma verificacao na funcao que é chamada, logo ainda é o mesmo token nao o prox
            rcv_token.processado = true;
            rcv_token = AnaLex(arqivoProc);
            decl_def_proc();
        }
    } 

    if(rcv_token.codigo != FINAL_ARQ){ error("Fim do arquivo esperado"); }

    printf("\n\nexpressao linha %d foi\n\n", contLinha);
    //algo da tabela de simbolos 
}

void decl_list_var(){
    printf("inicio da declaração da lista de variaveis: < decl_list_var >\n\n");
    if(rcv_token.categoria == PLV_RSVD && rcv_token.codigo == CONST){
        rcv_token.processado = true;
        rcv_token = AnaLex(arqivoProc);
    }

    tipo();
    decl_var();

    while(rcv_token.categoria == SNL && rcv_token.codigo == VIRGULA){
        rcv_token.processado = true;
        rcv_token = AnaLex(arqivoProc);
        decl_var();   
    }
}


void decl_def_proc(){
    printf("inicio da declaração de funcoes ou procedimentos: < decl_def_proc >\n\n");

    
}

void init(){}
void proc_def(){}

//vindas do decl_lis_var:
void tipo(){
    printf("tipo de variaveis: < tipo >\n\n");
    if(rcv_token.codigo == INT || rcv_token.codigo == CHAR || rcv_token.codigo == REAL || rcv_token.codigo == BOOL){
        rcv_token.processado = true;
        rcv_token = AnaLex(arqivoProc);
    } else {
        error("era esperado a declaração do tipo de variável");
    }
}

void decl_var(){
    printf("inicio da declaração da variavel: < decl_var >\n\n");

    //aqui acho que o escopo do token p tabsimb vai ser sempre EXTERNO_PROC

    if(rcv_token.categoria != ID){ error("era esperado identificador"); }

    //fazer verificação na tabela de simbolos p ver se o identificador ja foi usado p inicializar ou nao - semantico?
    rcv_token.processado = true;
    rcv_token = AnaLex(arqivoProc);
    //adicionar na tabela de simb? 

    while(rcv_token.categoria == SNL && rcv_token.codigo == ABRE_COL){ //vetor ou matriz
        rcv_token.processado = true;
        rcv_token = AnaLex(arqivoProc);

        if(!(rcv_token.categoria == INTCON || rcv_token.categoria == ID)){
            error("era esperado intcon ou um identificador");
        } else {
            rcv_token.processado = true;
            rcv_token = AnaLex(arqivoProc);
            //adicionar na tabela de simb?

            if(!(rcv_token.categoria == SNL && rcv_token.codigo == FECHA_COL)){
                error("era esperado o fechamento do colchete");
            } else {
                rcv_token.processado = true;
                rcv_token = AnaLex(arqivoProc);
            }
        }
    }

    if(rcv_token.categoria == SNL && rcv_token.codigo == ATRIBUICAO){ //pode ocorrer sendo vetor ou matriz ou variavel normal tb
        rcv_token.processado = true;
        rcv_token = AnaLex(arqivoProc);
        valor_var();
        if(rcv_token.categoria = SNL && rcv_token.codigo == VIRGULA){ //agora p o caso de vetor ou matriz especificamente
            rcv_token.processado = true;
            rcv_token = AnaLex(arqivoProc);
            valor_var();
        }
    } 
}