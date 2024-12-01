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

int valor_var(){
    printa_valor_token();

    //adicionar na tabela de simb?
    rcv_token.processado = true;
    rcv_token = AnaLex(arqivoProc);
    return rcv_token.categoria;
}

void prog(){
    printf("inicio do programa: < prog >\n\n");
    while(rcv_token.categoria == PLV_RSVD && (rcv_token.codigo == CONST || rcv_token.codigo == INT || rcv_token.codigo == CHAR || rcv_token.codigo == REAL || rcv_token.codigo == BOOL)){
        printf("em prog: %d (tem que ser 1 ou de 5 a 8)\n", rcv_token.codigo);
        
        decl_list_var();
    } 
    while(rcv_token.categoria == PLV_RSVD && (rcv_token.codigo == PROT || rcv_token.codigo == DEF)) {
        printf("em prog: %d (tem que ser 2 ou 3)\n", rcv_token.codigo);
        decl_def_proc(); 
    }
    //algo da tabela de simbolos 
}

void decl_list_var(){
    printf("inicio da declaração da lista de variaveis: < decl_list_var >\n\n");
    if(rcv_token.categoria == PLV_RSVD && rcv_token.codigo == CONST){
        printf("em decl list var: %d (tem que ser 1)\n", rcv_token.codigo);
        rcv_token.processado = true; 
        rcv_token = AnaLex(arqivoProc);
    }

    tipo();
    decl_var();
    
    while(rcv_token.categoria == SNL && rcv_token.codigo == VIRGULA){
        printf("em decl list var: %d (tem que ser 22)\n", rcv_token.codigo);
        rcv_token.processado = true;
        rcv_token = AnaLex(arqivoProc);
        decl_var();   
    }
    printf("fim da declaração da lista de variaveis\n\n");
}


void decl_def_proc(){
    printf("inicio da declaração de funcoes ou prototipos de procedimentos: < decl_def_proc >\n\n");
    switch (rcv_token.codigo){
        case PROT:
            prot();
            break;
        case DEF:
            def();
            break;
    }
    printf("fim da declaração de procedimentos\n\n");
}


//vindas do decl_list_var:
void tipo(){
    printf("tipo de variaveis: < tipo >\n\n");
    if(rcv_token.codigo == INT || rcv_token.codigo == CHAR || rcv_token.codigo == REAL || rcv_token.codigo == BOOL){
        printf("em tipo: %d (tem que ser de 5 a 8)\n", rcv_token.codigo);
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
    printf("variavel declarada: %s\n", rcv_token.lexema);
    iniciar_tabsimb();
    //adicionar na tabela de simb? 
    rcv_token.processado = true;
    rcv_token = AnaLex(arqivoProc);

    while(rcv_token.categoria == SNL && rcv_token.codigo == ABRE_COL){ //vetor ou matriz
        rcv_token.processado = true;
        rcv_token = AnaLex(arqivoProc);

        if(!(rcv_token.categoria == INTCON || rcv_token.categoria == ID)){
            error("era esperado intcon ou um identificador");
        } else {
            printa_valor_token();
            //adicionar na tabela de simb
            
            rcv_token.processado = true;
            rcv_token = AnaLex(arqivoProc);

            if(!(rcv_token.categoria == SNL && rcv_token.codigo == FECHA_COL)){
                error("era esperado o fechamento do colchete");
            } else {
                printf("foi um array\n");
                rcv_token.processado = true;
                rcv_token = AnaLex(arqivoProc);
            }
        }
    }

    if(rcv_token.categoria == SNL && rcv_token.codigo == ATRIBUICAO){ //pode ocorrer sendo vetor ou matriz ou variavel normal tb
        int cat = valor_var();
        if (cat != 9 && cat != 8){
            if(rcv_token.categoria == SNL && rcv_token.codigo == ABRE_CHAVE){ //agora p o caso de vetor ou matriz especificamente
                rcv_token.processado = true; 
                rcv_token = AnaLex(arqivoProc);
                do{
                     cat = valor_var();
                    if(rcv_token.categoria = SNL && rcv_token.codigo == VIRGULA){ 
                        rcv_token.processado = true;
                        rcv_token = AnaLex(arqivoProc);
                    } else { break; }
                }while(1);

                if(rcv_token.categoria = SNL && rcv_token.codigo == FECHA_CHAVE){
                    rcv_token.processado = true; 
                    rcv_token = AnaLex(arqivoProc);
                } else { error("erra esperado fechamento do '{' com '}'"); }
            } else if(rcv_token.categoria == CHARCON || rcv_token.categoria == REALCON || rcv_token.categoria == INTCON || rcv_token.categoria == STRINGCON){
                //salvar na tabela de simbolos
                printa_valor_token();
                rcv_token.processado = true;
                rcv_token = AnaLex(arqivoProc);
            } else{ error("era esperado um identificador após '='"); }
        } else { printf("\naqui!!!!!!\n");}
    } 
    
    printf("fim da declaração da variavel\n\n");
}

//vindas do decl_def_prot
void prot(){
    printf("inicio da declaração de prototipos de procedimentos: < prot >\n\n");
    rcv_token.processado = true;
    rcv_token = AnaLex(arqivoProc);

    if(rcv_token.categoria == PLV_RSVD && rcv_token.codigo == INIT){
        error("declaração do bloco init não permitida");
    } else if(rcv_token.categoria = ID){ //idproc
        //salvar na tabela
        rcv_token.processado = true;
        rcv_token = AnaLex(arqivoProc);

        if(!(rcv_token.categoria == SNL && rcv_token.codigo == ABRE_PAREN)){
            error("era esperado abertura do parenteses na declaração de prototipo de procedimento");
        } else {
            rcv_token.processado = true;
            rcv_token = AnaLex(arqivoProc);
            while(1){ //enquanto estiver na condicao do else - ABRE_PAREN
                parametro();
                while(rcv_token.categoria == SNL && rcv_token.codigo == ABRE_COL){
                    rcv_token.processado = true;
                    rcv_token = AnaLex(arqivoProc);
                    if(!(rcv_token.categoria == SNL && rcv_token.codigo == FECHA_COL)){
                        error("era esperado o fechamento do colchetes");
                    } else {
                        printf("vetor de algo no parametro\n");
                        rcv_token.processado = true;
                        rcv_token = AnaLex(arqivoProc);
                    }
                }
                if(!(rcv_token.categoria == SNL && rcv_token.codigo == FECHA_PAREN)){
                    error("era esperado o fechamento do parenteses");
                } else {
                    printf("foi uma declaração de prototipo de procedimento\n");
                    rcv_token.processado = true;
                    rcv_token = AnaLex(arqivoProc);
                    break;
                }
            }
        }
    } else {
        error("era esperando um identificador após 'prot'");
    }
}

void def(){
    printf("inicio da declaração de funcoes: < def >\n\n");
    rcv_token.processado = true;
    rcv_token = AnaLex(arqivoProc);
    //salvar na tabela - deixar aqui se for para salvar o INIT só

    if(rcv_token.categoria == PLV_RSVD && rcv_token.codigo == INIT){
        printf("inicio do bloco principal do programa proc < init >\n\n");
        rcv_token.processado = true;
        rcv_token = AnaLex(arqivoProc);

        if(rcv_token.categoria == FINAL_EXP){ rcv_token = AnaLex(arqivoProc); }

        while(rcv_token.categoria == PLV_RSVD && (rcv_token.codigo == CONST || rcv_token.codigo == INT || rcv_token.codigo == CHAR || rcv_token.codigo == REAL || rcv_token.codigo == BOOL)){
            printf("em init: %d (tem que ser 1 ou de 5 a 8)\n", rcv_token.codigo);
            decl_list_var();
            rcv_token.processado = true;
            rcv_token = AnaLex(arqivoProc); 
        } 

        while(rcv_token.categoria == PLV_RSVD || rcv_token.categoria == ID){ //cmd
            // printf("ANTES: cat: %d | codigo: %d\n", rcv_token.categoria, rcv_token.codigo);
            // rcv_token.processado = true;
            // rcv_token = AnaLex(arqivoProc);
            // printf("DEPOIS: cat: %d | codigo: %d\n", rcv_token.categoria, rcv_token.codigo);
            
            if(rcv_token.codigo == ENDP){ break; }
            cmd();
        }

        if(!(rcv_token.categoria == PLV_RSVD && rcv_token.codigo == ENDP)){
            error("era esperado o término do bloco init com 'endp'");
        } else {
            printf("fim da implementação do bloco init\n");
            // rcv_token.processado = true;
            // rcv_token = AnaLex(arqivoProc);
        }
    } else if(rcv_token.categoria == ID){
        //alguma outra funcao

    } else{
        error("era esperado o identificador 'init' ou um de função qualquer após 'def'");
    }

    printf("fim do uso de def\n");    
}

void parametro(){
    if(rcv_token.categoria == SNL && rcv_token.codigo == ACESSO_END){
        printf("endereco\n");
        rcv_token.processado = true;
        rcv_token = AnaLex(arqivoProc); 
        //talvez tenha que guardar a informação na tabela
    }
    tipo();
}

void cmd(){}