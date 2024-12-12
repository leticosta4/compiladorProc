#include <stdio.h> 
#include <stdlib.h> 
#include <ctype.h> 
#include <string.h> 
#include <stdbool.h> 

#include "analex.h"
#include "anasint.h"
#include "funcaux.h"
#include "tabsimb.h"

registro_tabsimb info_token;

int valor_var(){
    rcv_token = AnaLex(arqivoProc);
    consome_fim_exp();
    return rcv_token.categoria;
}

void confere_atrib_constante(){
    if(info_token.constante == SIM){
        if(rcv_token.categoria == INTCON && info_token.tipo == _INT){
            info_token.valor_constante.inteiro = rcv_token.valor_inteiro;
        } else if(rcv_token.categoria == REALCON && info_token.tipo == _REAL){
            info_token.valor_constante.real = rcv_token.valor_real;
        } else if(rcv_token.categoria == CHARCON && info_token.tipo == _CHAR){
            info_token.valor_constante.real = rcv_token.valor_real;
        } else if(rcv_token.categoria == INTCON && info_token.tipo == _BOOL){
            info_token.valor_constante.v_bool = rcv_token.valor_inteiro; //0 ou 1
        } else{ error("era esperado tipo compatível para inicialização da constante"); }
    }
}

void prog(){
    printf("inicio do programa: < prog > | LINHA: %d\n\n", contLinha);
    iniciar_tabsimb();
    consome_fim_exp();

    while(rcv_token.categoria == PLV_RSVD && (rcv_token.codigo == CONST || rcv_token.codigo == INT || rcv_token.codigo == CHAR || rcv_token.codigo == REAL || rcv_token.codigo == BOOL)){
        printf("em prog: %d (tem que ser 1 ou de 5 a 8)\n", rcv_token.codigo);

        info_token.escopo = GLOBAL;
        info_token.categoria = VAR_GLOBAL;

        decl_list_var();
    }
    while(rcv_token.categoria == PLV_RSVD && (rcv_token.codigo == PROT || rcv_token.codigo == DEF)) {
        printf("em prog: %d (tem que ser 2 ou 3)\n", rcv_token.codigo);
        decl_def_proc(); 
    }
}

void decl_list_var(){
    printf("inicio da declaração da lista de variaveis: < decl_list_var > | LINHA: %d\n\n", contLinha);

    info_token.passagem = NAO_APLICA_PARAM;
    info_token.zumbi = NAO_APLICA_ZUMBI;
    info_token.array = VAR_SIMPLES;
    info_token = limpar_dimensoes_array(info_token); //algo que limpe as dimensoes do array p n sobrescrever
    info_token.constante = NAO; //se for dps altera
    //endereço é adicionado na função de inserção na tabela

    if(rcv_token.categoria == PLV_RSVD && rcv_token.codigo == CONST){
        printf("é uma constante\n");
        printf("em decl list var: %d (tem que ser 1)\n", rcv_token.codigo);
        info_token.constante = SIM;
        rcv_token = AnaLex(arqivoProc);
    }

    tipo();
    decl_var();
    
    while(rcv_token.categoria == SNL && rcv_token.codigo == VIRGULA){
        printf("em decl list var: %d (tem que ser 22)\n", rcv_token.codigo);
        rcv_token = AnaLex(arqivoProc);
        consome_fim_exp(); 
        decl_var();   
    }
    consome_fim_exp();
    printf("fim da declaração da lista de variaveis\n\n");
}

void decl_def_proc(){
    printf("inicio da declaração de funcoes ou prototipos de procedimentos: < decl_def_proc >\n\n");

    info_token.escopo = GLOBAL; //se for local é alterado depois em def()
    info_token.tipo = _NAO_APLICA_TIPO;
    //a categoria (prototipo || procedimento) é atribuida em prot() e nas condicionais de def()
    info_token.passagem = NAO_APLICA_PARAM;
    info_token.zumbi = NAO_APLICA_ZUMBI;
    info_token.array = NAO_APLICA_ARRAY;
    info_token.constante = NAO;
    //o endereco é na funcao de insercao

    switch (rcv_token.codigo){
        case PROT:
            info_token.categoria = PROTOTIPO;
            prot();
            break;
        case DEF:
            info_token.categoria = PROCEDIMENTO;
            def();
            break;
    }
    consome_fim_exp();
    if(rcv_token. categoria == PLV_RSVD && rcv_token.codigo == ENDP){
        rcv_token = AnaLex(arqivoProc);
        printf("fim da declaração de procedimentos\n\n");
    } 
    //a falta do ENDP ja foi tratada em def()
}

void cmd(){
    //o token ja chega processado p cmd mesmo
    printf("inicio de algum comando: < cmd > | LINHA: %d\n\n", contLinha);
    if(rcv_token.categoria == PLV_RSVD){
        switch(rcv_token.codigo){
            case PROT:
            case DEF:
            case INIT:
                error("ERRO SINTATICO > palavra reservada inválida para o cmd");
                break;
            case FINAL_EXP:
            case GETOUT:
                rcv_token = AnaLex(arqivoProc);
                break;
            case GETINT:
            case GETREAL:
            case GETCHAR:
            case GETSTR:
                rcv_token = AnaLex(arqivoProc);
                if(rcv_token.categoria != ID){ error("ERRO SINTATICO > era esperado um identificador para input com get"); }
                rcv_token = AnaLex(arqivoProc);
                break;
            case PUTINT:
                rcv_token = AnaLex(arqivoProc);
                if(rcv_token.categoria != ID && rcv_token.categoria != INTCON){ error("ERRO SINTATICO > era esperado um identificador ou constante inteiro para output do intcon com put"); }
                rcv_token = AnaLex(arqivoProc);
                break;
            case PUTREAL:
                rcv_token = AnaLex(arqivoProc);
                if(rcv_token.categoria != ID && rcv_token.categoria != REALCON){ error("ERRO SINTATICO > era esperado um identificador ou constante real para output do realcon com put"); }
                rcv_token = AnaLex(arqivoProc);
                break;
            case PUTCHAR:
                rcv_token = AnaLex(arqivoProc);
                if(rcv_token.categoria != ID && rcv_token.categoria != CHARCON){ error("ERRO SINTATICO > era esperado um identificador ou constante char para output do charcon com put"); }
                rcv_token = AnaLex(arqivoProc);
                break;
            case PUTSTR:
                rcv_token = AnaLex(arqivoProc);
                if(rcv_token.categoria != ID && rcv_token.categoria != STRINGCON){ error("ERRO SINTATICO > era esperado um identificador ou constante literal para output do stringcon com put"); }
                rcv_token = AnaLex(arqivoProc);
                break;
            case DO:
                printf("início de um do\n\n");
                rcv_token = AnaLex(arqivoProc);
                if(rcv_token.categoria != ID){ error("ERRO SINTATICO > era esperado identificador para chamada de procedimento");} 
                else{ //idproc  
                    rcv_token = AnaLex(arqivoProc);
                    if(!(rcv_token.categoria == SNL && rcv_token.codigo == ABRE_PAREN)){
                        error("ERRO SINTATICO > era esperado abertura de parenteses após o identificador para chamada do procedimento");
                    } else{
                        rcv_token = AnaLex(arqivoProc);
                        consome_fim_exp();
                        if(rcv_token.categoria == SNL || rcv_token.categoria == ID || rcv_token.categoria == INTCON || rcv_token.categoria == REALCON || rcv_token.categoria == CHARCON){
                            expr();

                            //ja veio processado do final de fator < final de termo < final de expr_simples
                            while (rcv_token.categoria == SNL && rcv_token.codigo == VIRGULA){
                                rcv_token = AnaLex(arqivoProc);
                                consome_fim_exp();
                                if(rcv_token.categoria != SNL && rcv_token.categoria != ID && rcv_token.categoria != INTCON && rcv_token.categoria != REALCON && rcv_token.categoria != CHARCON){
                                    error("ERRO SINTATICO > era esperada uma expressão após ','");
                                } else {
                                    expr();
                                }
                            }
                            
                            if(!(rcv_token.categoria == SNL && rcv_token.codigo == FECHA_PAREN)){
                                error("ERRO SINTATICO > era esperado ')' após a expressão no do");
                            } else{
                                printf("foi um do\n");
                                rcv_token = AnaLex(arqivoProc);
                                consome_fim_exp();
                            }
                        } else{ error("ERRO SINTATICO > era esperada uma expressão após '('"); }
                    }
                }
                break;
            case WHILE:
                printf("início de um while\n\n");
                rcv_token = AnaLex(arqivoProc); 
                consome_fim_exp();
                if(!(rcv_token.categoria == SNL && rcv_token.codigo == ABRE_PAREN)){
                    error("ERRO SINTATICO > era esperado abertura de parenteses após o 'while'");
                } else {
                    rcv_token = AnaLex(arqivoProc);
                    consome_fim_exp();
                    if(rcv_token.categoria == SNL || rcv_token.categoria == ID || rcv_token.categoria == INTCON || rcv_token.categoria == REALCON || rcv_token.categoria == CHARCON){
                        expr();

                        //ja veio processado do final de fator < final de termo < final de expr_simples
                        if(!(rcv_token.categoria == SNL && rcv_token.codigo == FECHA_PAREN)){
                            error("ERRO SINTATICO > era esperado um ')' após a expressão no while");
                        } else{
                            rcv_token = AnaLex(arqivoProc);
                            consome_fim_exp();
                            
                            while(rcv_token.categoria == PLV_RSVD || rcv_token.categoria == ID){
                                cmd();
                                consome_fim_exp();
                                if (rcv_token.categoria == PLV_RSVD && rcv_token.codigo == ENDW){
                                    printf("SAINDO while\n\n");
                                    break;
                                }
                                //analex é chamado de novo no final da função - n precisa chamar aqui
                            }
                            if(!(rcv_token.categoria == PLV_RSVD && rcv_token.codigo == ENDW)){
                                error("ERRO SINTATICO > era esperada a finalização do loop while com endw");
                            } 
                            rcv_token = AnaLex(arqivoProc);
                            consome_fim_exp();
                        }
                    } else{ error("ERRO SINTATICO > era esperada uma expressão após '('"); }
                }
                printf("fim do while\n\n");
                break;
            case IF:
                printf("início de um if\n\n");
                rcv_token = AnaLex(arqivoProc); 
                consome_fim_exp();
                if(!(rcv_token.categoria == SNL && rcv_token.codigo == ABRE_PAREN)){
                    error("ERRO SINTATICO > era esperado abertura de parenteses após o 'if'");
                } else{
                    rcv_token = AnaLex(arqivoProc);
                    consome_fim_exp();
                    if(rcv_token.categoria != ID && rcv_token.categoria != INTCON && rcv_token.categoria != REALCON && rcv_token.categoria != CHARCON && rcv_token.categoria != SNL){
                        error("ERRO SINTATICO > era esperado termo para inicio de epressão");
                    }
                    expr();

                    //ja veio processado do final de fator < final de termo < final de expr_simples
                    if(!(rcv_token.categoria == SNL && rcv_token.codigo == FECHA_PAREN)){
                        error("ERRO SINTATICO > era esperado um ')' após expressão do if");
                    } else{
                            rcv_token = AnaLex(arqivoProc);
                            consome_fim_exp();
                            
                            while(rcv_token.categoria == PLV_RSVD || rcv_token.categoria == ID){
                                cmd();
                                consome_fim_exp(); 
                                if (rcv_token.categoria == PLV_RSVD && (rcv_token.codigo == ENDI || rcv_token.codigo == ELIF || rcv_token.codigo == ELSE)){
                                    printf("SAINDO if\n\n");
                                    break;
                                }
                                //analex é chamado de novo no final da função - n precisa chamar aqui
                            }

                            while(rcv_token.categoria == PLV_RSVD && rcv_token.codigo == ELIF){
                                printf("inicio de um elif\n\n");
                                rcv_token = AnaLex(arqivoProc); 
                                consome_fim_exp();
                                if(!(rcv_token.categoria == SNL && rcv_token.codigo == ABRE_PAREN)){
                                    error("ERRO SINTATICO > era esperado abertura de parenteses após o 'elif'");
                                } else{
                                    rcv_token = AnaLex(arqivoProc);
                                    consome_fim_exp();
                                    if(rcv_token.categoria != ID && rcv_token.categoria != INTCON && rcv_token.categoria != REALCON && rcv_token.categoria != CHARCON && rcv_token.categoria != SNL){
                                        error("ERRO SINTATICO > era esperado termo para inicio de epressão");
                                    }
                                    expr();

                                    //ja veio processado do final de fator < final de termo < final de expr_simples
                                    if(!(rcv_token.categoria == SNL && rcv_token.codigo == FECHA_PAREN)){
                                        error("ERRO SINTATICO > era esperado um ')' após expressão do elif");
                                    } else{
                                        rcv_token = AnaLex(arqivoProc);
                                        consome_fim_exp();
                                        
                                        while(rcv_token.categoria == PLV_RSVD || rcv_token.categoria == ID){
                                            cmd();
                                            consome_fim_exp(); 
                                            if (rcv_token.categoria == PLV_RSVD && (rcv_token.codigo == ENDI || rcv_token.codigo == ELIF || rcv_token.codigo == ELSE)){
                                                printf("SAINDO elif\n\n");
                                                break;
                                            }
                                            //analex é chamado de novo no final da função - n precisa chamar aqui
                                        }
                                    }
                                }
                            }

                            while(rcv_token.categoria == PLV_RSVD && rcv_token.codigo == ELSE){
                                printf("inicio de um else\n\n");
                                rcv_token = AnaLex(arqivoProc); 
                                consome_fim_exp();
                                
                                while(rcv_token.categoria == PLV_RSVD || rcv_token.categoria == ID){
                                    cmd();
                                    consome_fim_exp(); 
                                    if (rcv_token.categoria == PLV_RSVD && rcv_token.codigo == ENDI){
                                        printf("SAINDO else\n\n");
                                        break;
                                    }
                                    //analex é chamado de novo no final da função - n precisa chamar aqui
                                }
                            }

                            if(!(rcv_token.categoria == PLV_RSVD && rcv_token.codigo == ENDI)){
                                error("ERRO SINTATICO > era esperada a finalização do bloco condicional com endi");
                            } 
                            rcv_token = AnaLex(arqivoProc);
                            consome_fim_exp();
                        }
                    
                }
                printf("fim do if {elif | else}\n");
                break;
            case VAR:
                printf("início de um bloco var\n\n");
                rcv_token = AnaLex(arqivoProc);
                if(rcv_token.categoria != ID || (rcv_token.categoria == PLV_RSVD && rcv_token.codigo == VAR)){ error("ERRO SINTATICO > era esperado identificador para expressao após o 'var'"); }
                else{
                    rcv_token = AnaLex(arqivoProc);
                    if(!(rcv_token.categoria == PLV_RSVD && rcv_token.codigo == FROM)){ error("ERRO SINTATICO > era esperada a palavra reservada 'from' após o identificador"); }
                    else{
                        rcv_token = AnaLex(arqivoProc);
                        consome_fim_exp();
                        if(rcv_token.categoria != ID && rcv_token.categoria != INTCON && rcv_token.categoria != REALCON && rcv_token.categoria != CHARCON && rcv_token.categoria != SNL){
                            error("ERRO SINTATICO > era esperado termo para inicio de epressão");
                        }
                        expr(); //expr1

                        //ja veio processado do final de fator < final de termo < final de expr_simples
                        if(!(rcv_token.categoria == PLV_RSVD && (rcv_token.codigo == TO || rcv_token.codigo == DT))){
                            error("ERRO SINTATICO > eram esperadas as palavras reservadas 'to' ou 'dt' após a expressão");
                        } else{
                            rcv_token = AnaLex(arqivoProc);
                            consome_fim_exp();
                            if(rcv_token.categoria != ID && rcv_token.categoria != INTCON && rcv_token.categoria != REALCON && rcv_token.categoria != CHARCON && rcv_token.categoria != SNL){
                                error("ERRO SINTATICO > era esperado termo para inicio de epressão");
                            }
                            expr(); //expr2

                            //ja veio processado do final de fator < final de termo < final de expr_simples
                            if(rcv_token.categoria == PLV_RSVD && rcv_token.codigo == BY){
                                rcv_token = AnaLex(arqivoProc);
                                if(!(rcv_token.categoria == INTCON || rcv_token.categoria == ID)){ error("ERRO SINTATICO > era esperado um valor inteiro ou identificador após 'by'"); }
                                rcv_token = AnaLex(arqivoProc);
                                consome_fim_exp();
                            }

                            while(rcv_token.categoria == PLV_RSVD || rcv_token.categoria == ID){
                                cmd();
                                consome_fim_exp(); 
                                if (rcv_token.categoria == PLV_RSVD && rcv_token.codigo == ENDV){
                                    printf("SAINDO var\n\n");
                                    break;
                                }
                                //analex é chamado de novo no final da função - n precisa chamar aqui
                            }

                            if(!(rcv_token.categoria == PLV_RSVD && (rcv_token.codigo == ENDV || rcv_token.codigo == ENDW || rcv_token.codigo == ENDI))){
                                error("ERRO SINTATICO > era esperada a finalização do bloco var com endv");
                            }
                            rcv_token = AnaLex(arqivoProc);
                            consome_fim_exp();
                        }
                    }
                }
                printf("fim do var\n\n");
                break;
        }
    }
    
    else if(rcv_token.categoria == ID){
        rcv_token = AnaLex(arqivoProc);
        consome_fim_exp();
        atrib(); //esse atrib ja faz o cmd terminar com processamento de token
    } 

    else if(rcv_token.categoria == FINAL_EXP){ rcv_token = AnaLex(arqivoProc); }
    
    else if(rcv_token.categoria == FINAL_ARQ){ error("ERRO SINTATICO > final do arquivo inesperadamente encontrado durante CMD - faltou fechamento de algum bloco [endi | endw | endi]"); }
    
    else { error("ERRO SINTATICO > era esperado um identificador ou palavra reservada para o cmd"); }
    printf("fim do cmd\n\n");
}

void atrib(){ //ja chega processado
    int d_cont = 1;
    printf("início de uma atribuição: < atrib >\n\n");
    
     if(rcv_token.categoria != SNL){ error("ERRO SINTATICO > termo inválido após identificador para atribuição"); }

     else{
        while(rcv_token.categoria == SNL && rcv_token.codigo == ABRE_COL){
            if(d_cont < 3){
                rcv_token = AnaLex(arqivoProc);
                consome_fim_exp();

                if(rcv_token.categoria != ID && rcv_token.categoria != INTCON && rcv_token.categoria != REALCON && rcv_token.categoria != CHARCON && rcv_token.categoria != SNL){
                    error("ERRO SINTATICO > era esperado termo para inicio de epressão");
                }

                //logica p tabela de simbolos?
                expr();

                //ja veio processado do final de fator < final de termo < final de expr_simples
                if(!(rcv_token.categoria == SNL && rcv_token.codigo == FECHA_COL)){ error("ERRO SINTATICO > era esperado um ']' após a expressão na atribuição"); }
                else{
                    d_cont++;
                    printf("array de algo");
                    
                    if(d_cont == 1){ info_token.array = VAR_SIMPLES; }
                    else if(d_cont == 2){ info_token.array = ID_VETOR; }
                    else{ info_token.array = ID_MATRIZ; }

                    rcv_token = AnaLex(arqivoProc);
                    consome_fim_exp();
                }
            }
        }
         //ja vem processado do cmd ou do final desse de cima
        if(rcv_token.categoria == SNL && rcv_token.codigo == ATRIBUICAO){
            rcv_token = AnaLex(arqivoProc);
            consome_fim_exp();

            if(rcv_token.categoria != ID && rcv_token.categoria != INTCON && rcv_token.categoria != REALCON && rcv_token.categoria != CHARCON && rcv_token.categoria != STRINGCON && rcv_token.categoria != SNL){
                error("ERRO SINTATICO > era esperado termo para inicio de epressão");
            }
            expr();
            //ja finaliza processado
        } else {
            error("ERRO SINTATICO > era esperado um '=' para atribuição");
        }
     }

     printf("fim de uma atribuição\n\n");
}

void expr(){ //ja chega processado
    int seguir;
    printf("inicio de uma expressão: < expr > | LINHA: %d\n\n", contLinha);
    expr_simples();

    //ja veio processado do final de fator < final de termo < final de expr_simples
    seguir = op_rel();
    //ja veio processado do final de op_rel
    if(seguir == 1){ expr_simples(); }

    printf("fim de uma expressão\n");
}

void expr_simples(){ //ja chega processado de expr que vem de cmd
    printf("inicio de uma expressão simples: < expr_simp > | LINHA: %d\n\n", contLinha);
    if(rcv_token.categoria == SNL && (rcv_token.codigo == ADICAO || rcv_token.codigo == SUBTRACAO)){
        rcv_token = AnaLex(arqivoProc);
    }
    termo();

    //ja veio processado do final de termo < final de fator
    while(rcv_token.categoria == SNL && (rcv_token.codigo == ADICAO || rcv_token.codigo == SUBTRACAO || rcv_token.codigo == AND_LOGICO)){
        rcv_token = AnaLex(arqivoProc);
        consome_fim_exp();
        termo();
    }

    printf("fim de uma expressão simples\n");
}

void termo(){ //ja chega processado de expr_simples (que vem de expr ou do sinal + || -)
    printf("inicio de um termo: < termo > | LINHA: %d\n\n", contLinha);
    fator();
    
    //ja veio processado do final de fator
    while(rcv_token.categoria == SNL && (rcv_token.codigo == MULTIPLICACAO || rcv_token.codigo == DIVISAO || rcv_token.codigo == OR_LOGICO)){
        rcv_token = AnaLex(arqivoProc);
        consome_fim_exp();
        fator();
    }

    printf("fim de um termo\n");
}

void fator(){ //ja chega processado de expr_simples (que vem de expr ou do sinal + || -)
    int cont_d = 1;
    printf("inicio de um fator: < fator > | LINHA: %d\n\n", contLinha);
    switch (rcv_token.categoria){
        case ID:
            rcv_token = AnaLex(arqivoProc);
            while(rcv_token.categoria == SNL && rcv_token.codigo == ABRE_COL){
                if(cont_d < 3){
                    rcv_token = AnaLex(arqivoProc);
                    if(rcv_token.categoria == SNL || rcv_token.categoria == ID || rcv_token.categoria == INTCON || rcv_token.categoria == REALCON || rcv_token.categoria == CHARCON){
                        expr();

                        if(rcv_token.categoria == SNL && rcv_token.codigo == FECHA_COL){
                            cont_d++;
                            printf("array de algo");

                            if(cont_d == 1){ info_token.array = VAR_SIMPLES; }
                            else if(cont_d == 2){ info_token.array = ID_VETOR; }
                            else{ info_token.array = ID_MATRIZ; }

                            rcv_token = AnaLex(arqivoProc);
                            consome_fim_exp();
                        } else{
                            error("ERRO SINTATICO > era esperado um ']' após a expressão");
                        }
                    } else{
                        error("ERRO SINTATICO > era esperada expressão após o '('");
                    }
                } 
            }
            break;
        case INTCON:
        case REALCON:
        case CHARCON:
        case STRINGCON:
            rcv_token = AnaLex(arqivoProc);
            consome_fim_exp();
            break;
        case SNL:
            if(rcv_token.codigo == ABRE_PAREN){
                rcv_token = AnaLex(arqivoProc);
                consome_fim_exp();
                if(rcv_token.categoria == SNL || rcv_token.categoria == ID || rcv_token.categoria == INTCON || rcv_token.categoria == REALCON || rcv_token.categoria == CHARCON){
                    expr();
                    if(rcv_token.codigo != FECHA_PAREN){
                        error("ERRO SINTATICO > era esperado um ')' após a expressão");
                    } else{
                        printf("foi uma expressao em fator");
                        rcv_token = AnaLex(arqivoProc);
                        consome_fim_exp();
                    }
                }
            } else if(rcv_token.codigo == NEGACAO){
                rcv_token = AnaLex(arqivoProc);
                consome_fim_exp();
                fator();
                rcv_token = AnaLex(arqivoProc);
                consome_fim_exp();
            } else { error("ERRO SINTATICO > sinal invalido encontrado em fator"); }
            break;
    }
    printf("fim de um fator\n");
}

int op_rel(){ //ja chega processado de expr
    if(rcv_token.categoria == SNL && (
        rcv_token.codigo == COMP_IGUALDADE ||
        rcv_token.codigo == COMP_DIFERENTE ||
        rcv_token.codigo == MAIOR_IGUAL ||
        rcv_token.codigo == MAIOR_QUE ||
        rcv_token.codigo == MENOR_IGUAL ||
        rcv_token.codigo == MENOR_QUE
    )){
        printf("inicio de um operador relacional\n\n");
        rcv_token = AnaLex(arqivoProc);
        consome_fim_exp();
        printf("fim de um operador relacional\n\n");
        return 1;
    } else{ return 0; }
}

//vindas do decl_list_var:
void tipo(){
    printf("tipo de variaveis: < tipo > | LINHA: %d\n\n", contLinha);
    if(rcv_token.categoria == PLV_RSVD && (rcv_token.codigo == INT || rcv_token.codigo == CHAR || rcv_token.codigo == REAL || rcv_token.codigo == BOOL)){
        printf("em tipo: %d (tem que ser de 5 a 8)\n", rcv_token.codigo);
        
        switch (rcv_token.codigo){
            case INT:
                info_token.tipo = _INT;
                break;
            case REAL:
                info_token.tipo = _REAL;
                break;
            case CHAR:
                info_token.tipo = _CHAR;
                break;
            case BOOL:
                info_token.tipo = _BOOL;
                break;
        }

        rcv_token = AnaLex(arqivoProc);
    } else {
        error("ERRO SINTATICO > era esperado a declaração do tipo de variável");
    }
}

void decl_var(){
    int cont_dim = 1;
    printf("inicio da declaração da variavel: < decl_var > | LINHA: %d\n\n", contLinha);

    if(rcv_token.categoria != ID){ error("ERRO SINTATICO > era esperado identificador"); }

    printf("variavel declarada: %s\n", rcv_token.lexema);
    strcpy(info_token.lexema, rcv_token.lexema);
    verifica_redeclaracao(info_token); //se o escopo for global ja foi definido em prog

    rcv_token = AnaLex(arqivoProc);

    if(info_token.constante == SIM && !(rcv_token.categoria == SNL && rcv_token.codigo == ATRIBUICAO)){
        error("era esperada inicialização da constante");
    }

    while(rcv_token.categoria == SNL && rcv_token.codigo == ABRE_COL){ //vetor ou matriz
        if(cont_dim < 3){
            int cat = valor_var();
            if(!(rcv_token.categoria == INTCON || rcv_token.categoria == ID)){
                error("ERRO SINTATICO > era esperado intcon ou um identificador");
            } else {
                info_token.dimensoes_array[cont_dim - 1] = rcv_token.valor_inteiro;

                rcv_token = AnaLex(arqivoProc);

                if(!(rcv_token.categoria == SNL && rcv_token.codigo == FECHA_COL)){
                    error("ERRO SINTATICO > era esperado o fechamento do colchete");
                } else {
                    cont_dim++;
                    printf("foi um array\n");
                    
                    if(cont_dim == 1){ info_token.array = VAR_SIMPLES; }
                    else if(cont_dim == 2){ info_token.array = ID_VETOR; }
                    else{ info_token.array = ID_MATRIZ; }

                    rcv_token = AnaLex(arqivoProc);
                    consome_fim_exp();
                }
            }
        } else { error("ERRO SINTATICO > foi encontrado array com número de dimensões superior a 2"); }
    }

    if(rcv_token.categoria == SNL && rcv_token.codigo == ATRIBUICAO){ //pode ocorrer sendo vetor ou matriz ou variavel normal tb
        int cat = valor_var();
        if (cat != 9 && cat != 8){
            if(rcv_token.categoria == SNL && rcv_token.codigo == ABRE_CHAVE){ //agora p o caso de vetor ou matriz especificamente   
                rcv_token = AnaLex(arqivoProc);
                do{
                     cat = valor_var();
                    if(rcv_token.categoria = SNL && rcv_token.codigo == VIRGULA){ 
                        rcv_token = AnaLex(arqivoProc);
                        consome_fim_exp(); 
                    } else { break; }
                }while(1);

                if(rcv_token.categoria = SNL && rcv_token.codigo == FECHA_CHAVE){
                    rcv_token = AnaLex(arqivoProc);
                } else { error("ERRO SINTATICO > erra esperado fechamento do '{' com '}'"); }
            } else if(rcv_token.categoria == CHARCON || rcv_token.categoria == REALCON || rcv_token.categoria == INTCON){
                confere_atrib_constante(); 

                rcv_token = AnaLex(arqivoProc);
            } else if(rcv_token.categoria == STRINGCON){
                error("não é permitida a declaração de variáveis com stringcon");
            } else{ error("ERRO SINTATICO > era esperado um identificador após '='"); }
        } else {
            if(cat == 9){ error("ERRO SINTATICO > fim do arquivo inesperado"); }
        }
    } 
    
    
    inserir_tabsimb(info_token); //insercao de variavel declarada
    info_token = limpar_dimensoes_array(info_token); //algo que limpe as dimensoes do array p n sobrescrever
    printf("fim da declaração da variavel\n\n");
}

//vindas do decl_def_prot
void prot(){
    int ca, _cd = 1; 

    printf("inicio da declaração de prototipos de procedimentos: < prot > | LINHA: %d\n\n", contLinha);
    rcv_token = AnaLex(arqivoProc);

    if(rcv_token.categoria == PLV_RSVD && rcv_token.codigo == INIT){
        error("ERRO SINTATICO > declaração do bloco init não permitida com prot");
    } else if(rcv_token.categoria == ID){ //idproc
        strcpy(info_token.lexema, rcv_token.lexema);
        verifica_redeclaracao(info_token);
        inserir_tabsimb(info_token); //insercao do prototipo de procedimento

        rcv_token = AnaLex(arqivoProc);

        if(!(rcv_token.categoria == SNL && rcv_token.codigo == ABRE_PAREN)){
            error("ERRO SINTATICO > era esperado abertura do parenteses na declaração de prototipo de procedimento");
        } else {
            rcv_token = AnaLex(arqivoProc);
            do{
                strcpy(info_token.lexema, "");
                info_token.categoria = PARAMETRO;
                info_token.zumbi = NAO_APLICA_ZUMBI;

                passagem_end_tipo();
                while(rcv_token.categoria == SNL && rcv_token.codigo == ABRE_COL){
                    if(_cd < 3){
                        rcv_token = AnaLex(arqivoProc);
                        if(!(rcv_token.categoria == SNL && rcv_token.codigo == FECHA_COL)){
                            error("ERRO SINTATICO > era esperado o fechamento do colchetes");
                        } else {
                            _cd++;
                            printf("array de algo no parametro\n");

                            if(_cd == 1){ info_token.array = VAR_SIMPLES; }
                            else if(_cd == 2){ info_token.array = ID_VETOR; }
                            else{ info_token.array = ID_MATRIZ; }
                            ca = valor_var();
                        }
                    } else{ error("ESSO SINTATICO > foi encontrado array com núemero de dimensões superior a 2 nos parâmetros do protótipo de procedimento"); }
                } 
                if(rcv_token.categoria == SNL && rcv_token.codigo == VIRGULA){
                    rcv_token = AnaLex(arqivoProc);
                    consome_fim_exp(); 
                } else { break; }

                inserir_tabsimb(info_token); //insercao do parametro de prototipo
            } while(1);
    
            inserir_tabsimb(info_token); //insercao do parametro de prototipo - ultimo dps da virgula

            if(!(rcv_token.categoria == SNL && rcv_token.codigo == FECHA_PAREN)){
                error("ERRO SINTATICO > era esperado o fechamento do parenteses");
            } else {
                printf("foi uma declaração de prototipo de procedimento\n");
                rcv_token = AnaLex(arqivoProc);
                consome_fim_exp();
            }
        }
    } else {
        error("ERRO SINTATICO > era esperando um identificador após 'prot'");
    }
}

void def(){
    int cate, cd = 1;

    printf("inicio da declaração de funcoes: < def > | LINHA: %d\n\n", contLinha);
    rcv_token = AnaLex(arqivoProc); 
    
    if(rcv_token.categoria == PLV_RSVD && rcv_token.codigo == INIT){
        printf("inicio do bloco principal do programa proc: < init >\n\n");

        strcpy(info_token.lexema, "init");
        verifica_redeclaracao(info_token); //verifica redeclaração do procedimento (lexema e dps categoria)
        info_token = limpar_dimensoes_array(info_token); 
        inserir_tabsimb(info_token); //insercao do procedimento init

        rcv_token = AnaLex(arqivoProc);
        consome_fim_exp();

        while(rcv_token.categoria == PLV_RSVD && (rcv_token.codigo == CONST || rcv_token.codigo == INT || rcv_token.codigo == CHAR || rcv_token.codigo == REAL || rcv_token.codigo == BOOL)){
            printf("em init: %d (tem que ser 1 ou de 5 a 8)\n", rcv_token.codigo);

            info_token.escopo = LOCAL;
            info_token.categoria = VAR_LOCAL;

            decl_list_var();
        } 

        while(rcv_token.categoria == PLV_RSVD || rcv_token.categoria == ID){ //cmd
            if(rcv_token.codigo == ENDP){ break; }
            cmd();
            consome_fim_exp();
        }

        if(!(rcv_token.categoria == PLV_RSVD && rcv_token.codigo == ENDP)){
            error("ERRO SINTATICO > era esperado o término do bloco init com 'endp'");
        } else {
            printf("fim da implementação do bloco init\n");
            apagar_var_locais(procura_posicao_proc("init")); 
            rcv_token = AnaLex(arqivoProc);
        }
    } else if(rcv_token.categoria == ID){
        printf("inicio de alguma implementação de bloco com def\n\n");
        char nome_def[TAM_MAX_LEXEMA];
        strcpy(nome_def, rcv_token.lexema);

        strcpy(info_token.lexema, rcv_token.lexema);
        verifica_redeclaracao(info_token); //verifica redeclaração do procedimento (lexema e dps categoria)
        info_token = limpar_dimensoes_array(info_token); 
        inserir_tabsimb(info_token); //insercao do procedimento

        rcv_token = AnaLex(arqivoProc);
        consome_fim_exp();
    
        if(!(rcv_token.categoria == SNL && rcv_token.codigo == ABRE_PAREN)){
            error("ERRO SINTATICO > era esperado abertura do parenteses na declaração de prototipo de procedimento");
        } else {
            rcv_token = AnaLex(arqivoProc);
            if(!(rcv_token.categoria == SNL && rcv_token.codigo == FECHA_PAREN) &&
                !(rcv_token.categoria == SNL && rcv_token.codigo == ACESSO_END) &&
                !(rcv_token.categoria == PLV_RSVD && (rcv_token.codigo == INT || rcv_token.codigo == REAL || rcv_token.codigo == CHAR || rcv_token.codigo == BOOL))){
                    error("era esperado ')', ou '&', ou algum tipo de variável após '('");
                } else if((rcv_token.categoria == SNL && rcv_token.codigo == ACESSO_END) ||
                          (rcv_token.categoria == PLV_RSVD && (rcv_token.codigo == INT || rcv_token.codigo == REAL || rcv_token.codigo == CHAR || rcv_token.codigo == BOOL))){

                            do {
                                info_token.categoria = PARAMETRO;
                                info_token.zumbi = VIVO;
                                info_token = limpar_dimensoes_array(info_token);
                                passagem_end_tipo();

                                if(rcv_token.categoria != ID){ error("ERRO SINTATICO > era esperado um identificador após a declaração do tipo"); } //ISSO NAO É OBRIGATORIO
                                else{
                                    strcpy(info_token.lexema, rcv_token.lexema);//salvar na tabela se for ID
                                    rcv_token = AnaLex(arqivoProc);

                                    while(rcv_token.categoria == SNL && rcv_token.codigo == ABRE_COL){
                                        if(cd < 3){
                                            rcv_token = AnaLex(arqivoProc);
                                            if(!(rcv_token.categoria == INTCON || rcv_token.categoria == ID)){
                                                error("ERRO SINTATICO > era esperado inteiro após '['");
                                            } else{
                                                info_token.dimensoes_array[cd - 1] = rcv_token.valor_inteiro;

                                                rcv_token = AnaLex(arqivoProc);
                                                if(!(rcv_token.categoria == SNL && rcv_token.codigo == FECHA_COL)){
                                                    error("ERRO SINTATICO > era esperado o fechamento do colchetes");
                                                } else {
                                                    cd++;
                                                    printf("array de algo no parametro\n");
                                                    
                                                    if(cd == 1){ info_token.array = VAR_SIMPLES; }
                                                    else if(cd == 2){ info_token.array = ID_VETOR; }
                                                    else{ info_token.array = ID_MATRIZ; }
                                                    
                                                    cate = valor_var();
                                                }
                                            }
                                        } else{ error("ESSO SINTATICO > foi encontrado array com núemero de dimensões superior a 2 nos parâmetros do protótipo de procedimento"); }
                                    }

                                    if(rcv_token.categoria == SNL && rcv_token.codigo == VIRGULA){
                                        rcv_token = AnaLex(arqivoProc);
                                        consome_fim_exp(); 
                                    } else { break; }
                                }
                                inserir_tabsimb(info_token); //insercao do parametro de procedimento
                            } while(1);
                            inserir_tabsimb(info_token); //insercao do parametro de procedimento - ultimo dps da virgula
                }

            if(!(rcv_token.categoria == SNL && rcv_token.codigo == FECHA_PAREN)){
                error("ERRO SINTATICO > era esperado o fechamento do parenteses");
            } else {
                
                rcv_token = AnaLex(arqivoProc);
                consome_fim_exp();
    
                while(rcv_token.categoria == PLV_RSVD && (rcv_token.codigo == CONST || rcv_token.codigo == INT || rcv_token.codigo == CHAR || rcv_token.codigo == REAL || rcv_token.codigo == BOOL)){
                    printf("em def > prot: %d (tem que ser 1 ou de 5 a 8)\n", rcv_token.codigo);
                    info_token.categoria = VAR_LOCAL;
                    decl_list_var();
                } 

                while(rcv_token.categoria == PLV_RSVD || rcv_token.categoria == ID){ //cmd
                    if(rcv_token.codigo == ENDP){ break; }
                    cmd();
                    consome_fim_exp();
                }
                if(!(rcv_token.categoria == PLV_RSVD && rcv_token.codigo == ENDP)){
                    error("ERRO SINTATICO > era esperado o término do procedimento com 'endp'");
                } else {
                    printf("fim da implementação do procedimento\n");
                    transformar_zumbi(procura_posicao_proc(nome_def));
                    apagar_var_locais(procura_posicao_proc(nome_def));
                    rcv_token = AnaLex(arqivoProc);
                }
            }
        }
    } else{
        error("ERRO SINTATICO > era esperado o identificador 'init' ou um de função qualquer após 'def'");
    }
    printf("fim do uso de def\n");  
}

void passagem_end_tipo(){
    printf("inicio de parametro de procedimento/função | LINHA: %d\n\n", contLinha);

    info_token.escopo = LOCAL;
    info_token.constante = NAO;
    info_token.passagem = VALOR; //se for referencia muda dps no if
    info_token.array = VAR_SIMPLES; //se n for dps muda no def e no prot

    if(rcv_token.categoria == SNL && rcv_token.codigo == ACESSO_END){
        printf("endereco\n");
        info_token.passagem = REFERENCIA;
        rcv_token = AnaLex(arqivoProc); 
        tipo();
    } 

    if(rcv_token.categoria == PLV_RSVD && (rcv_token.codigo == INT || rcv_token.codigo == CHAR || rcv_token.codigo == REAL || rcv_token.codigo == BOOL)){ tipo(); }
}