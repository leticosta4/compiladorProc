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
int identificador_bool = -1, label_desvio_call_init = 0;
FILE *proc_obj_file;

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
        } else{ error("ERRO SEMANTICO > era esperado tipo compatível para inicialização da constante"); }
    }
}

void prog(){
    int var_globais = 0, vg = 0, init_label;
    proc_obj_file = fopen("proc_obj_file._obj", "w");
    consome_fim_exp();

    if(proc_obj_file == NULL){ printf("ERRO > problema ao abrir arquivo objeto para o programa\n"); exit(1);} //talvez colocar um exit(1)
    fprintf(proc_obj_file, "INIP\n");

    while(rcv_token.categoria == PLV_RSVD && (rcv_token.codigo == CONST || rcv_token.codigo == INT || rcv_token.codigo == CHAR || rcv_token.codigo == REAL || rcv_token.codigo == BOOL)){
        var_globais++;
        info_token.escopo = GLOBAL;
        info_token.categoria = VAR_GLOBAL;
        info_token.tem_prototipo = NAO_APLICA_PROT;

        vg = decl_list_var("");
    }
    var_globais += vg;
    if(var_globais != 0){ fprintf(proc_obj_file, "AMEM %d\n", var_globais); }
    fprintf(proc_obj_file, "GOTO L1\n"); //DESVIO DE LABEL 

    while(rcv_token.categoria == PLV_RSVD && (rcv_token.codigo == PROT || rcv_token.codigo == DEF)) {
        decl_def_proc(); 

        busca_erro_decl_var_dps_decl_prot_proc_ou_cmd(rcv_token, 0);
    }
    busca_erro_decl_var_dps_decl_prot_proc_ou_cmd(rcv_token, 0);
    prototipo_sozinho();

    fprintf(proc_obj_file, "\nLABEL %d\n", label_desvio_call_init);
    init_label = busca_retorna_label("init"); 
    if(init_label > 0){ fprintf(proc_obj_file, "CALL L%d\n", init_label); }
    if(var_globais != 0){ fprintf(proc_obj_file, "DMEM %d\n", var_globais); }
    fprintf(proc_obj_file, "HALT\n");
}

int decl_list_var(char possivel_proced[]){
    int variaveis = 0;
    info_token.passagem = NAO_APLICA_PARAM;
    info_token.zumbi = NAO_APLICA_ZUMBI;
    info_token.array = VAR_SIMPLES;
    info_token = limpar_dimensoes_array(info_token); 
    info_token.constante = NAO; //se for dps altera
    //endereço é adicionado na função de inserção na tabela

    if(rcv_token.categoria == PLV_RSVD && rcv_token.codigo == CONST){
        info_token.constante = SIM;
        rcv_token = AnaLex(arqivoProc);
        consome_fim_exp();
    }

    tipo();
    decl_var(possivel_proced);
    variaveis+=1;

    while(rcv_token.categoria == SNL && rcv_token.codigo == VIRGULA){
        rcv_token = AnaLex(arqivoProc);
        consome_fim_exp(); 
        decl_var(possivel_proced);   
        variaveis+=1;
    }
    consome_fim_exp();
    return variaveis;
}

void decl_def_proc(){
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
            info_token.tem_prototipo = _SIM;
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
        consome_fim_exp();
    } 
    //a falta do ENDP ja foi tratada em def()
}

void cmd(char procedimento[]){
    int clausula, tipo_identificador, tipo_expr_cond, tipo_expr_var[2], tipo_atrib_id;
    //o token ja chega processado p cmd mesmo
    if(rcv_token.categoria == PLV_RSVD){
        switch(rcv_token.codigo){
            case PROT:
                error("ERRO SEMANTICO > não é permitido assinatura de protótipo de procedimento aqui");
                break;
            case DEF:
                error("ERRO SEMANTICO > não é permitido definicão de procedimento aqui");
                break;
            case INIT:
                error("ERRO SINTATICO > não é permitido definicão do bloco 'init' aqui");
                break;
            case FINAL_EXP:
            case GETOUT:
                rcv_token = AnaLex(arqivoProc);
                consome_fim_exp();
                break;
            case GETINT:
            case GETREAL:
            case GETCHAR:
            case GETSTR:
                rcv_token = AnaLex(arqivoProc);
                consome_fim_exp();
                if(rcv_token.categoria != ID){ error("ERRO SINTATICO > era esperado um identificador para input com get"); }
                rcv_token = AnaLex(arqivoProc);
                consome_fim_exp();
                break;
            case PUTINT:
                rcv_token = AnaLex(arqivoProc);
                consome_fim_exp();
                if(rcv_token.categoria != ID && rcv_token.categoria != INTCON){ error("ERRO SINTATICO > era esperado um identificador ou constante inteiro para output do intcon com put"); }
                rcv_token = AnaLex(arqivoProc);
                consome_fim_exp();
                break;
            case PUTREAL:
                rcv_token = AnaLex(arqivoProc);
                consome_fim_exp();
                if(rcv_token.categoria != ID && rcv_token.categoria != REALCON){ error("ERRO SINTATICO > era esperado um identificador ou constante real para output do realcon com put"); }
                rcv_token = AnaLex(arqivoProc);
                consome_fim_exp();
                break;
            case PUTCHAR:
                rcv_token = AnaLex(arqivoProc);
                consome_fim_exp();
                if(rcv_token.categoria != ID && rcv_token.categoria != CHARCON){ error("ERRO SINTATICO > era esperado um identificador ou constante char para output do charcon com put"); }
                rcv_token = AnaLex(arqivoProc);
                consome_fim_exp();
                break;
            case PUTSTR:
                rcv_token = AnaLex(arqivoProc);
                consome_fim_exp();
                if(rcv_token.categoria != ID && rcv_token.categoria != STRINGCON){ error("ERRO SINTATICO > era esperado um identificador ou constante literal para output do stringcon com put"); }
                rcv_token = AnaLex(arqivoProc);
                consome_fim_exp();
                break;
            case DO: 
                rcv_token = AnaLex(arqivoProc);
                _do(procedimento);
                break;
            case WHILE:
                rcv_token = AnaLex(arqivoProc); 
                consome_fim_exp();
                _while(procedimento);
                break;
            case IF:
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
                    tipo_expr_cond = expr(procedimento); 
                    printf("\nteste fo tipo q a expr_cond ficou > TIPO : %d\n", tipo_expr_cond);
                    if(associa_tipos_compat(tipo_expr_cond, _BOOL) != 0){
                        error("ERRO SEMANTICO > o tipo da expressão para o if deve ser booleana");
                    }

                    //ja veio processado do final de fator < final de termo < final de expr_simples
                    if(!(rcv_token.categoria == SNL && rcv_token.codigo == FECHA_PAREN)){
                        error("ERRO SINTATICO > era esperado um ')' após expressão do if");
                    } else{
                            rcv_token = AnaLex(arqivoProc);
                            consome_fim_exp();
                            
                            while(rcv_token.categoria == PLV_RSVD || rcv_token.categoria == ID){
                                cmd(procedimento);
                                consome_fim_exp(); 
                                if(rcv_token.categoria == PLV_RSVD && (rcv_token.codigo == ENDI || rcv_token.codigo == ELIF || rcv_token.codigo == ELSE)){ break; }
                                //analex é chamado de novo no final da função - n precisa chamar aqui
                            }

                            while(rcv_token.categoria == PLV_RSVD && rcv_token.codigo == ELIF){
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
                                    tipo_expr_cond = expr(procedimento);
                                    printf("\nteste fo tipo q a expr_cond ficou > TIPO : %d\n", tipo_expr_cond);
                                    if(associa_tipos_compat(tipo_expr_cond, _BOOL) != 0){
                                        error("ERRO SEMANTICO > o tipo da expressão para o elif deve ser booleana");
                                    }

                                    //ja veio processado do final de fator < final de termo < final de expr_simples
                                    if(!(rcv_token.categoria == SNL && rcv_token.codigo == FECHA_PAREN)){
                                        error("ERRO SINTATICO > era esperado um ')' após expressão do elif");
                                    } else{
                                        rcv_token = AnaLex(arqivoProc);
                                        consome_fim_exp();
                                        
                                        while(rcv_token.categoria == PLV_RSVD || rcv_token.categoria == ID){
                                            cmd(procedimento);
                                            consome_fim_exp(); 
                                            if (rcv_token.categoria == PLV_RSVD && (rcv_token.codigo == ENDI || rcv_token.codigo == ELIF || rcv_token.codigo == ELSE)){ break; }
                                            //analex é chamado de novo no final da função - n precisa chamar aqui
                                        }
                                    }
                                }
                            }

                            while(rcv_token.categoria == PLV_RSVD && rcv_token.codigo == ELSE){
                                rcv_token = AnaLex(arqivoProc); 
                                consome_fim_exp();
                                
                                while(rcv_token.categoria == PLV_RSVD || rcv_token.categoria == ID){
                                    cmd(procedimento);
                                    consome_fim_exp(); 
                                    if (rcv_token.categoria == PLV_RSVD && rcv_token.codigo == ENDI){
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
                break;
            case VAR:
                rcv_token = AnaLex(arqivoProc);
                consome_fim_exp();
                if(rcv_token.categoria != ID || (rcv_token.categoria == PLV_RSVD && rcv_token.codigo == VAR)){ error("ERRO SINTATICO > era esperado identificador para expressao após o 'var'"); }
                else{
                    rcv_token = AnaLex(arqivoProc);
                    consome_fim_exp();
                    if(!(rcv_token.categoria == PLV_RSVD && rcv_token.codigo == FROM)){ error("ERRO SINTATICO > era esperada a palavra reservada 'from' após o identificador"); }
                    else{
                        rcv_token = AnaLex(arqivoProc);
                        consome_fim_exp();
                        if(rcv_token.categoria != ID && rcv_token.categoria != INTCON && rcv_token.categoria != REALCON && rcv_token.categoria != CHARCON && rcv_token.categoria != SNL){
                            error("ERRO SINTATICO > era esperado termo para inicio de epressão");
                        }
                        tipo_expr_var[0] = expr(procedimento); //expr1

                        //ja veio processado do final de fator < final de termo < final de expr_simples
                        if(!(rcv_token.categoria == PLV_RSVD && (rcv_token.codigo == TO || rcv_token.codigo == DT))){
                            error("ERRO SINTATICO > eram esperadas as palavras reservadas 'to' ou 'dt' após a expressão");
                        } else{
                            clausula = (rcv_token.categoria == PLV_RSVD && rcv_token.codigo == TO)? 0 : 1; //0: to; 1: dt

                            rcv_token = AnaLex(arqivoProc);
                            consome_fim_exp();
                            if(rcv_token.categoria != ID && rcv_token.categoria != INTCON && rcv_token.categoria != REALCON && rcv_token.categoria != CHARCON && rcv_token.categoria != SNL){
                                error("ERRO SINTATICO > era esperado termo para inicio de epressão");
                            }
                            tipo_expr_var[1] = expr(procedimento); //expr2

                            //ANALISE SEMANTICA VER A CONDICAO DE QUAL EXPR TEM SER MAIOR - provavelmente uma funcao(clausula) - MAQUINA DE PILHA

                            //ja veio processado do final de fator < final de termo < final de expr_simples
                            if(rcv_token.categoria == PLV_RSVD && rcv_token.codigo == BY){
                                rcv_token = AnaLex(arqivoProc);
                                consome_fim_exp();
                                if(!(rcv_token.categoria == INTCON || rcv_token.categoria == ID)){ error("ERRO SINTATICO > era esperado um valor inteiro ou identificador após 'by'"); }
                                rcv_token = AnaLex(arqivoProc);
                                consome_fim_exp();
                            }

                            while(rcv_token.categoria == PLV_RSVD || rcv_token.categoria == ID){
                                cmd(procedimento);
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
        tipo_identificador = procura_existencia_identificador_em_proced(procura_posicao_proc(procedimento), rcv_token.lexema).tipo;
        rcv_token = AnaLex(arqivoProc);
        consome_fim_exp();
        tipo_atrib_id = atrib(procedimento); //esse atrib ja faz o cmd terminar com processamento de token
       
        if(associa_tipos_compat(tipo_identificador, tipo_atrib_id) != 0){
            error("ERRO SEMÂNTICO > identificador deve receber atribuição de tipo compatível");
        }
    } 

    else if(rcv_token.categoria == FINAL_EXP){ rcv_token = AnaLex(arqivoProc); consome_fim_exp(); }
    
    else if(rcv_token.categoria == FINAL_ARQ){ error("ERRO SINTATICO > final do arquivo inesperadamente encontrado durante CMD - faltou fechamento de algum bloco [endi | endw | endi]"); }
    
    else { error("ERRO SINTATICO > era esperado um identificador ou palavra reservada para o cmd"); }
}

int atrib(char p[]){ //ja chega processado
    int d_cont = 1, tipo_atribuicao, tipo_indice_array;    

     if(rcv_token.categoria != SNL){ error("ERRO SINTATICO > termo inválido após identificador para atribuição"); }

     else{
        while(rcv_token.categoria == SNL && rcv_token.codigo == ABRE_COL){
            if(d_cont < 3){
                rcv_token = AnaLex(arqivoProc);
                consome_fim_exp();

                if(rcv_token.categoria != ID && rcv_token.categoria != INTCON && rcv_token.categoria != REALCON && rcv_token.categoria != CHARCON && rcv_token.categoria != SNL){
                    error("ERRO SINTATICO > era esperado termo para inicio de epressão");
                }

                tipo_indice_array = expr(p); 
                if(tipo_indice_array != _INT){ //talvez tb possa char
                    error("ERRO SEMANTICO > a expressão usada como índice no array deve ter tipo int");
                }

                //ja veio processado do final de fator < final de termo < final de expr_simples
                if(!(rcv_token.categoria == SNL && rcv_token.codigo == FECHA_COL)){ error("ERRO SINTATICO > era esperado um ']' após a expressão na atribuição"); }
                else{
                    d_cont++;
                    
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
            tipo_atribuicao = expr(p); //ja finaliza processado
        } else {
            error("ERRO SINTATICO > era esperado um '=' para atribuição");
        }
     return tipo_atribuicao;
     }
}

int expr(char p[]){ //ja chega processado
    int seguir, tipo_expressoes[2], tipo_final_expr;
    
    tipo_expressoes[0] = expr_simples(p);

    //ja veio processado do final de fator < final de termo < final de expr_simples
    seguir = op_rel();

    if(seguir != -1){ //ja veio processado do final de op_rel
        tipo_expressoes[1] = expr_simples(p);
    
        if(associa_tipos_compat(tipo_expressoes[0], tipo_expressoes[1]) != 0){
            error("ERRO SEMÂNTICO > não é possível comparação entre tipos não compatíveis");
        } else { tipo_final_expr = _BOOL; }
    } else {tipo_final_expr = tipo_expressoes[0]; }

    printf("\nteste tipo expr: %d\n", tipo_final_expr);
    return tipo_final_expr;
}

int expr_simples(char p[]){ //ja chega processado de expr que vem de cmd
    int tipo_expr_simples, rel_logica = 0, tipo_aux_arit1 = -1, tipo_aux_arit2;

    if(rcv_token.categoria == SNL && (rcv_token.codigo == ADICAO || rcv_token.codigo == SUBTRACAO)){
        if(identificador_bool != -1){ error("ERRO SEMANTICO > não é permitida operação aritmetica com operando do tipo bool"); } //temp
        rcv_token = AnaLex(arqivoProc);
    }
    tipo_expr_simples = termo(p);

    //ja veio processado do final de termo < final de fator
    while(rcv_token.categoria == SNL && (rcv_token.codigo == ADICAO || rcv_token.codigo == SUBTRACAO || rcv_token.codigo == AND_LOGICO)){
        if(rcv_token.categoria == SNL && (rcv_token.codigo == ADICAO || rcv_token.codigo == SUBTRACAO)){
            if(identificador_bool != -1){ error("ERRO SEMANTICO > não é permitida operação aritmetica com operando do tipo bool"); } //temp
            tipo_aux_arit1 = tipo_expr_simples;
        } else if(rcv_token.categoria == SNL && rcv_token.codigo == AND_LOGICO){ rel_logica = 1; }

        rcv_token = AnaLex(arqivoProc);
        consome_fim_exp();
        tipo_aux_arit2 = termo(p);
    }
    if(rel_logica != 0){ tipo_expr_simples = _BOOL; }
    
    if(tipo_aux_arit1 != -1){ //foi expressao aritmetica e os tipos precisam ser compativeis
        if(associa_tipos_compat(tipo_aux_arit1, tipo_aux_arit2) != 0){
            error("ERRO SEMANTICO > não é permitida operação aritmética entre tipos não compatíveis");
        } 
    }

    return tipo_expr_simples;
}

int termo(char p[]){ //ja chega processado de expr_simples (que vem de expr ou do sinal + || -)
    int tipo_termo, rel_logica = 0, tipo_aux_arit1 = -1, tipo_aux_arit2;
    tipo_termo = fator(p);
    
    //ja veio processado do final de fator
    while(rcv_token.categoria == SNL && (rcv_token.codigo == MULTIPLICACAO || rcv_token.codigo == DIVISAO || rcv_token.codigo == OR_LOGICO)){ 
        if(rcv_token.categoria == SNL && (rcv_token.codigo == MULTIPLICACAO || rcv_token.codigo == DIVISAO)){
            if(identificador_bool != -1){ error("ERRO SEMANTICO > não é permitida operação aritmetica com operando do tipo bool"); } //temp
             tipo_aux_arit1 = tipo_termo;
        } else if(rcv_token.categoria == SNL && rcv_token.codigo == OR_LOGICO){
            rel_logica = 1;
        }
       
        rcv_token = AnaLex(arqivoProc);
        consome_fim_exp();
        tipo_aux_arit2 = fator(p);
    }
    if(rel_logica != 0){ tipo_termo = _BOOL; }
    if(tipo_aux_arit1 != -1){ //foi expressao aritmetica e os tipos precisam ser compativeis
        if(associa_tipos_compat(tipo_aux_arit1, tipo_aux_arit2) != 0){
            error("ERRO SEMANTICO > não é permitida operação aritmética entre tipos não compatíveis");
        } 
    }
    return tipo_termo;
}

int fator(char p[]){ //ja chega processado de expr_simples (que vem de expr ou do sinal + || -)
    int cont_d = 1, tipo_fator, tipo_indice_array, cat_temp, rel_logica = 0;

    switch (rcv_token.categoria){
        case ID:
            info_token = procura_existencia_identificador_em_proced(procura_posicao_proc(p), rcv_token.lexema); 
            tipo_fator = info_token.tipo;
            if(tipo_fator == _BOOL){ identificador_bool = 1; } 
            rcv_token = AnaLex(arqivoProc);
            consome_fim_exp();

            while(rcv_token.categoria == SNL && rcv_token.codigo == ABRE_COL){
                if(cont_d < 3){
                    rcv_token = AnaLex(arqivoProc);
                    consome_fim_exp();
                    if(rcv_token.categoria == SNL || rcv_token.categoria == ID || rcv_token.categoria == INTCON || rcv_token.categoria == REALCON || rcv_token.categoria == CHARCON){
                        tipo_indice_array = expr(p);
                        if(tipo_indice_array != _INT){
                            error("ERRO SEMANTICO > a expressão usada como índice de array deve ter tipo int");
                        }

                        if(rcv_token.categoria == SNL && rcv_token.codigo == FECHA_COL){
                            cont_d++;
                            
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
            tipo_fator = _INT; 
            rcv_token = AnaLex(arqivoProc);
            consome_fim_exp();
            break;
        case REALCON:
            tipo_fator = _REAL;
            rcv_token = AnaLex(arqivoProc);
            consome_fim_exp();
            break;
        case CHARCON:
        case STRINGCON:
            tipo_fator = _CHAR; 
            rcv_token = AnaLex(arqivoProc);
            consome_fim_exp();
            break;
        case SNL:
            if(rcv_token.codigo == ABRE_PAREN){
                rcv_token = AnaLex(arqivoProc);
                consome_fim_exp();
                if(rcv_token.categoria == SNL || rcv_token.categoria == ID || rcv_token.categoria == INTCON || rcv_token.categoria == REALCON || rcv_token.categoria == CHARCON){
                    cat_temp = rcv_token.categoria;
                    tipo_fator = expr(p); 

                    if(cat_temp == ID){ if(tipo_fator == _BOOL){ identificador_bool = 1; }} 

                    if(rcv_token.codigo != FECHA_PAREN){
                        error("ERRO SINTATICO > era esperado um ')' após a expressão");
                    } else{
                        rcv_token = AnaLex(arqivoProc);
                        consome_fim_exp();
                    }
                }
            } else if(rcv_token.codigo == NEGACAO){
                rel_logica = 1;
                rcv_token = AnaLex(arqivoProc);
                consome_fim_exp();
                tipo_fator = fator(p);
                //tinha chamada de analex aqui antes
            } else { error("ERRO SINTATICO > sinal invalido encontrado em fator"); }
            break;
    }
    if(rel_logica != 0){ tipo_fator = _BOOL; }
    return tipo_fator;
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
        rcv_token = AnaLex(arqivoProc);
        consome_fim_exp();

        return rcv_token.codigo;
    } else{ return -1; }
}

//vindas do decl_list_var:
void tipo(){
    if(rcv_token.categoria == PLV_RSVD && (rcv_token.codigo == INT || rcv_token.codigo == CHAR || rcv_token.codigo == REAL || rcv_token.codigo == BOOL)){
        
        switch(rcv_token.codigo){
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
        consome_fim_exp();
    } else {
        error("ERRO SINTATICO > era esperado a declaração do tipo de variável");
    }
}

void decl_var(char possivel_proced[]){
    int cont_dim = 1, nao_escalar = 0;
    registro_tabsimb aux;

    if(rcv_token.categoria != ID){ error("ERRO SINTATICO > era esperado identificador"); }

    printf("variavel declarada: %s\n", rcv_token.lexema);
    strcpy(info_token.lexema, rcv_token.lexema);
    verifica_redeclaracao(info_token); //se o escopo for global ja foi definido em prog

    rcv_token = AnaLex(arqivoProc);
    consome_fim_exp();

    if(info_token.constante == SIM && !(rcv_token.categoria == SNL && rcv_token.codigo == ATRIBUICAO)){
        error("era esperada inicialização da constante");
    }

    while(rcv_token.categoria == SNL && rcv_token.codigo == ABRE_COL){ //vetor ou matriz
        nao_escalar = 1;
        if(cont_dim < 3){
            int cat = valor_var();
            if(!(rcv_token.categoria == INTCON || rcv_token.categoria == ID)){
                error("ERRO SINTATICO > era esperado intcon ou um identificador");
            } else {
                if(rcv_token.categoria == ID){
                    aux = procura_existencia_identificador_em_proced(procura_posicao_proc(possivel_proced), rcv_token.lexema);
                    if(aux.constante != SIM){ error("a variável para array precisa ser uma constante"); }
                    else{
                        info_token.dimensoes_array[cont_dim - 1] = aux.valor_constante.inteiro;
                    }
                } else{ info_token.dimensoes_array[cont_dim - 1] = rcv_token.valor_inteiro; }
                
                rcv_token = AnaLex(arqivoProc);
                consome_fim_exp();

                if(!(rcv_token.categoria == SNL && rcv_token.codigo == FECHA_COL)){
                    error("ERRO SINTATICO > era esperado o fechamento do colchete");
                } else {
                    cont_dim++;
                    
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
                if(nao_escalar != 1){ error("não é permitido inicialização de variável escalar com '{}'"); }
                rcv_token = AnaLex(arqivoProc);
                consome_fim_exp();

                if(!(rcv_token.categoria == CHARCON || rcv_token.categoria == REALCON || rcv_token.categoria == INTCON)){
                    error("era esperado um charcon, realcon ou intcon após '{'");
                }
                verifica_compatibilidade_tipo_decl(rcv_token, info_token);
                do{
                     cat = valor_var();
                    if(rcv_token.categoria = SNL && rcv_token.codigo == VIRGULA){ 
                        rcv_token = AnaLex(arqivoProc);
                        consome_fim_exp(); 
                    } else { break; }
                }while(1);

                if(rcv_token.categoria = SNL && rcv_token.codigo == FECHA_CHAVE){
                    rcv_token = AnaLex(arqivoProc);
                    consome_fim_exp();
                } else { error("ERRO SINTATICO > erra esperado fechamento do '{' com '}'"); }
            } else if(rcv_token.categoria == CHARCON || rcv_token.categoria == REALCON || rcv_token.categoria == INTCON || rcv_token.categoria == STRINGCON){
                verifica_compatibilidade_tipo_decl(rcv_token, info_token); 
                confere_atrib_constante(); 

                rcv_token = AnaLex(arqivoProc);
                consome_fim_exp();
            } else{ error("ERRO SINTATICO > era esperado um identificador após '='"); }
        } else {
            if(cat == 9){ error("ERRO SINTATICO > fim do arquivo inesperado"); }
        }
    } 
    
    verifica_redeclaracao(info_token);
    inserir_tabsimb(info_token); //insercao de variavel declarada
    info_token = limpar_dimensoes_array(info_token); //algo que limpe as dimensoes do array p n sobrescrever
}

//vindas do decl_def_prot
void prot(){
    int ca = 0, _cd = 1, com_param = 0; 

    rcv_token = AnaLex(arqivoProc);
    consome_fim_exp();

    if(rcv_token.categoria == PLV_RSVD && rcv_token.codigo == INIT){
        error("ERRO SEMANTICO > declaração do bloco init não permitida com prot");
    } else if(rcv_token.categoria == ID){ //idproc
        strcpy(info_token.lexema, rcv_token.lexema);
        verifica_redeclaracao(info_token);
        inserir_tabsimb(info_token); //insercao do prototipo de procedimento

        rcv_token = AnaLex(arqivoProc);
        consome_fim_exp();

        if(!(rcv_token.categoria == SNL && rcv_token.codigo == ABRE_PAREN)){
            error("ERRO SINTATICO > era esperado abertura do parenteses na declaração de prototipo de procedimento");
        } else {
            rcv_token = AnaLex(arqivoProc);
            consome_fim_exp();

            if(rcv_token.categoria == SNL && rcv_token.codigo == FECHA_PAREN){ com_param = 1; }
            do{
                strcpy(info_token.lexema, "");
                info_token.categoria = PARAMETRO;
                info_token.zumbi = NAO_APLICA_ZUMBI;
                info_token.tem_prototipo = NAO_APLICA_PROT;

                passagem_end_tipo();
                while(rcv_token.categoria == SNL && rcv_token.codigo == ABRE_COL){
                    if(_cd < 3){
                        rcv_token = AnaLex(arqivoProc);
                        consome_fim_exp();

                        if(!(rcv_token.categoria == SNL && rcv_token.codigo == FECHA_COL)){
                            error("ERRO SINTATICO > era esperado o fechamento do colchetes");
                        } else {
                            _cd++;
                            
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

                if(com_param != 1){ inserir_tabsimb(info_token); } //insercao do parametro de prototipo se realmente tiver
            } while(1);
    
            if(com_param != 1){ inserir_tabsimb(info_token); } //insercao do parametro de prototipo se realmente tiver - ultimo dps da virgula

            if(!(rcv_token.categoria == SNL && rcv_token.codigo == FECHA_PAREN)){
                error("ERRO SINTATICO > era esperado o fechamento do parenteses");
            } else {
                rcv_token = AnaLex(arqivoProc);
                consome_fim_exp();
            }
        }
    } else {
        error("ERRO SINTATICO > era esperando um identificador após 'prot'");
    }
}

void def(){
    int cate = 0, cd = 1, substituir_prot = -2, cont_param_essa_def = 0, cont_param_proto = 0, variaveis_locais = 0, vl = 0, label_init = 0, label_def = 0; //substituir_prot é a posição do prototipo
    rcv_token = AnaLex(arqivoProc); 
    consome_fim_exp();
    
    if(rcv_token.categoria == PLV_RSVD && rcv_token.codigo == INIT){
        fprintf(proc_obj_file, "\nLABEL L1\n");
        label_desvio_call_init = gera_label();
        fprintf(proc_obj_file, "GOTO L%d\n", label_desvio_call_init); //DESVIO DE LABEL 
        
        label_init = gera_label();
        fprintf(proc_obj_file, "\nLABEL L%d\n", label_init); //a label da main
        fprintf(proc_obj_file, "INIPR 1\n");

        //atribuicoa de endereco correta e
        info_token.rotulo = label_init;
        info_token.tem_prototipo = NAO_APLICA_PROT;
        strcpy(info_token.lexema, "init");
        verifica_redeclaracao(info_token); //verifica redeclaração do procedimento (lexema e dps categoria)
        info_token = limpar_dimensoes_array(info_token); 
        inserir_tabsimb(info_token); //insercao do procedimento init
        info_token.rotulo = 0; //zerando

        rcv_token = AnaLex(arqivoProc);
        consome_fim_exp();

        while(rcv_token.categoria == PLV_RSVD && (rcv_token.codigo == CONST || rcv_token.codigo == INT || rcv_token.codigo == CHAR || rcv_token.codigo == REAL || rcv_token.codigo == BOOL)){
            info_token.escopo = LOCAL;
            info_token.categoria = VAR_LOCAL;

            vl = decl_list_var("init");
            variaveis_locais += vl;
        } 
        if(variaveis_locais != 0){ fprintf(proc_obj_file, "AMEM %d\n", variaveis_locais); }

        while(rcv_token.categoria == PLV_RSVD || rcv_token.categoria == ID){ //cmd
            if(rcv_token.codigo == ENDP){ break; }
            cmd("init");
            consome_fim_exp();
            busca_erro_decl_var_dps_decl_prot_proc_ou_cmd(rcv_token, 1);
        }

        if(!(rcv_token.categoria == PLV_RSVD && rcv_token.codigo == ENDP)){
            error("ERRO SINTATICO > era esperado o término do bloco init com 'endp'");
        } else {
            apagar_var_locais();
            rcv_token = AnaLex(arqivoProc);
            consome_fim_exp();

            if(variaveis_locais != 0){ fprintf(proc_obj_file, "DMEM %d\n", variaveis_locais); }
            fprintf(proc_obj_file, "RET 1, 0\n");
        }
    } else if(rcv_token.categoria == ID){
        char nome_def[TAM_MAX_LEXEMA];
        strcpy(nome_def, rcv_token.lexema);

        label_def = gera_label();
        fprintf(proc_obj_file, "\nLABEL L%d\n", label_def); 
        fprintf(proc_obj_file, "INIPR 1\n");

        info_token.rotulo = label_def;
        strcpy(info_token.lexema, rcv_token.lexema);
        verifica_redeclaracao(info_token); //verifica redeclaração do procedimento (lexema e dps categoria)
        info_token = limpar_dimensoes_array(info_token); 

        //verifica existencia do prototipo - existe: substitui naquela posicao
        substituir_prot = procura_existencia_prototipo_ou_proced(nome_def);

        if(substituir_prot == -1){
            info_token.tem_prototipo = _NAO;
            inserir_tabsimb(info_token); //insercao do procedimento - NOVO
        } else{
            info_token.tem_prototipo = _SIM;
            cont_param_proto = contar_params(substituir_prot); //tem prototipo
            substituir_prot_proc(substituir_prot, info_token);
        }
        info_token.rotulo = 0;

        rcv_token = AnaLex(arqivoProc);
        consome_fim_exp();
    
        if(!(rcv_token.categoria == SNL && rcv_token.codigo == ABRE_PAREN)){
            error("ERRO SINTATICO > era esperado abertura do parenteses na declaração de prototipo de procedimento");
        } else {
            rcv_token = AnaLex(arqivoProc);
            consome_fim_exp();

            info_token.tem_prototipo = NAO_APLICA_PROT;
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

                                if(rcv_token.categoria != ID){ error("ERRO SINTATICO > era esperado um identificador após a declaração do tipo"); } 
                                else{
                                    strcpy(info_token.lexema, rcv_token.lexema);//salvar na tabela se for ID
                                    rcv_token = AnaLex(arqivoProc);
                                    consome_fim_exp();

                                    while(rcv_token.categoria == SNL && rcv_token.codigo == ABRE_COL){
                                        if(cd < 3){
                                            rcv_token = AnaLex(arqivoProc);
                                            consome_fim_exp();

                                            if(!(rcv_token.categoria == INTCON || rcv_token.categoria == ID)){
                                                error("ERRO SINTATICO > era esperado inteiro após '['");
                                            } else{
                                                info_token.dimensoes_array[cd - 1] = rcv_token.valor_inteiro;

                                                rcv_token = AnaLex(arqivoProc);
                                                consome_fim_exp();
                                                
                                                if(!(rcv_token.categoria == SNL && rcv_token.codigo == FECHA_COL)){
                                                    error("ERRO SINTATICO > era esperado o fechamento do colchetes");
                                                } else {
                                                    cd++;
                                                    
                                                    if(cd == 1){ info_token.array = VAR_SIMPLES; }
                                                    else if(cd == 2){ info_token.array = ID_VETOR; }
                                                    else{ info_token.array = ID_MATRIZ; }
                                                    
                                                    cate = valor_var();
                                                }
                                            }
                                        } else{ error("ESSO SINTATICO > foi encontrado array com núemero de dimensões superior a 2 nos parâmetros do protótipo de procedimento"); }
                                    }

                                    cont_param_essa_def++;
                                    if(rcv_token.categoria == SNL && rcv_token.codigo == VIRGULA){
                                        rcv_token = AnaLex(arqivoProc);
                                        consome_fim_exp(); 
                                    } else { break; }
                                }

                                //a insercao vai depender se teve prototipo antes ou nao 
                                verifica_redecl_param(procura_posicao_proc(nome_def), info_token.lexema); //e se esse parametro ta repetido ou n
                                if(substituir_prot == -1){ inserir_tabsimb(info_token); //insercao do parametro de procedimento - NOVO
                                } else{
                                    substituir_parametros_prot_proc_testar_compat_tipos(substituir_prot, info_token, 0); //verificacao da compatibilidae de tipo embutida aqui
                                }
                                
                            } while(1);
                            
                            //a insercao vai depender se teve prototipo antes ou nao - ultimo dps da virgula
                             if((substituir_prot != -1) && (cont_param_essa_def != cont_param_proto)){
                                error("ERRO SEMANTICO > a quantidade de parametros de prototipo e de procedimento devem ser a mesma");
                            }
                            verifica_redecl_param(procura_posicao_proc(nome_def), info_token.lexema); //e se esse parametro ta repetido ou n
                            if(substituir_prot == -1){ inserir_tabsimb(info_token); //insercao do procedimento - NOVO
                            } else{
                                substituir_parametros_prot_proc_testar_compat_tipos(substituir_prot, info_token, 0); //verificacao da compatibilidae de tipo embutida aqui
                            }
                }

            if(!(rcv_token.categoria == SNL && rcv_token.codigo == FECHA_PAREN)){
                error("ERRO SINTATICO > era esperado o fechamento do parenteses");
            } else {
                
                rcv_token = AnaLex(arqivoProc);
                consome_fim_exp();
    
                while(rcv_token.categoria == PLV_RSVD && (rcv_token.codigo == CONST || rcv_token.codigo == INT || rcv_token.codigo == CHAR || rcv_token.codigo == REAL || rcv_token.codigo == BOOL)){
                    info_token.categoria = VAR_LOCAL;
                    vl = decl_list_var(nome_def);
                    variaveis_locais += vl;
                } 
                
                if(variaveis_locais != 0){ fprintf(proc_obj_file, "AMEM %d\n", variaveis_locais); }
               
                while(rcv_token.categoria == PLV_RSVD || rcv_token.categoria == ID){ //cmd
                    if(rcv_token.codigo == ENDP){ break; }
                    cmd(nome_def);
                    consome_fim_exp();
                }
                if(!(rcv_token.categoria == PLV_RSVD && rcv_token.codigo == ENDP)){
                    error("ERRO SINTATICO > era esperado o término do procedimento com 'endp'");
                } else {
                    transformar_zumbi(procura_posicao_proc(nome_def));
                    apagar_var_locais();
                    rcv_token = AnaLex(arqivoProc);
                    consome_fim_exp();

                    if(variaveis_locais != 0){ fprintf(proc_obj_file, "DMEM %d\n", variaveis_locais); }
                    fprintf(proc_obj_file, "RET 1, %d\n", cont_param_essa_def);
                }
            }
        }
    } else{
        error("ERRO SINTATICO > era esperado o identificador 'init' ou um de função qualquer após 'def'");
    }
}

void passagem_end_tipo(){
    info_token.escopo = LOCAL;
    info_token.constante = NAO;
    info_token.passagem = VALOR; //se for referencia muda dps no if
    info_token.array = VAR_SIMPLES; //se n for dps muda no def e no prot

    if(rcv_token.categoria == SNL && rcv_token.codigo == ACESSO_END){
        info_token.passagem = REFERENCIA;
        rcv_token = AnaLex(arqivoProc); 
        consome_fim_exp();
        tipo();
    } 

    if(rcv_token.categoria == PLV_RSVD && (rcv_token.codigo == INT || rcv_token.codigo == CHAR || rcv_token.codigo == REAL || rcv_token.codigo == BOOL)){ tipo(); }
}

//vindas do cmd
void _do(char em_qual_proced[]){ //o token ja chega processado
    int passou_expr = 0, cont_param_chamada = 0, cont_param_orig = 0, pos, tipo_param; 

    if(rcv_token.categoria != ID){ error("ERRO SINTATICO > era esperado identificador para chamada de procedimento");} 
    else{ //idproc  
        pos = procura_existencia_prototipo_ou_proced(rcv_token.lexema);
        if(pos == -1){ //ver se o procedimento existe - ANALISE SEMANTICA
            error("ERRO SEMANTICO > o procedimento a ser chamado com o 'do' precisa ter seu prototipo assinado ou ja ter sido definido");
        }
        cont_param_orig = contar_params(pos);

        rcv_token = AnaLex(arqivoProc);
        consome_fim_exp();

        if(!(rcv_token.categoria == SNL && rcv_token.codigo == ABRE_PAREN)){
            error("ERRO SINTATICO > era esperado abertura de parenteses após o identificador para chamada do procedimento");
        } else{
            rcv_token = AnaLex(arqivoProc);
            consome_fim_exp();
    
            if(!(rcv_token.categoria == SNL && rcv_token.codigo == FECHA_PAREN) &&
                !(rcv_token.categoria == SNL || rcv_token.categoria == ID || rcv_token.categoria == INTCON || rcv_token.categoria == REALCON || rcv_token.categoria == CHARCON)){
                    error("era esperado ')' ou alguma expressão após '('");
            } else if((rcv_token.categoria == SNL && rcv_token.codigo != FECHA_PAREN) || rcv_token.categoria == ID || rcv_token.categoria == INTCON || rcv_token.categoria == REALCON || rcv_token.categoria == CHARCON){
                passou_expr = 1;
                
                tipo_param = expr(em_qual_proced); 
                info_token.tipo = tipo_param;
                substituir_parametros_prot_proc_testar_compat_tipos(pos, info_token, 1);
                cont_param_chamada++;

                //ja veio processado do final de fator < final de termo < final de expr_simples
                while (rcv_token.categoria == SNL && rcv_token.codigo == VIRGULA){
                    rcv_token = AnaLex(arqivoProc);
                    consome_fim_exp();
                    if(rcv_token.categoria != SNL && rcv_token.categoria != ID && rcv_token.categoria != INTCON && rcv_token.categoria != REALCON && rcv_token.categoria != CHARCON){
                        error("ERRO SINTATICO > era esperada uma expressão após ','");
                    } else {
                        tipo_param = expr(em_qual_proced);
                        info_token.tipo = tipo_param;
                        substituir_parametros_prot_proc_testar_compat_tipos(pos, info_token, 1);
                        cont_param_chamada++;
                    }
                }
                
                if(!(rcv_token.categoria == SNL && rcv_token.codigo == FECHA_PAREN)){
                    error("ERRO SINTATICO > era esperado ')' após a expressão no do");
                } else{  
                    if(cont_param_chamada != cont_param_orig){
                        error("ERRO SEMANTICO > a quantidade de parametros do procedimento deve ser compatível com a de seu prototipo ou definição");
                    }

                    rcv_token = AnaLex(arqivoProc);
                    consome_fim_exp();
                }
            } 
            
            if((passou_expr == 0) && (rcv_token.categoria == SNL && rcv_token.codigo == FECHA_PAREN)){ //colocando essa condicional do passou_expr p o token não ser processado duas vezes
                rcv_token = AnaLex(arqivoProc);
                consome_fim_exp();
            }
        }
    }
}

void _while(char em_qual_proced[]){
    int tipo_expr_cond;
    if(!(rcv_token.categoria == SNL && rcv_token.codigo == ABRE_PAREN)){
        error("ERRO SINTATICO > era esperado abertura de parenteses após o 'while'");
    } else {
        //chamar o print da label do while

        rcv_token = AnaLex(arqivoProc);
        consome_fim_exp();
        if(rcv_token.categoria == SNL || rcv_token.categoria == ID || rcv_token.categoria == INTCON || rcv_token.categoria == REALCON || rcv_token.categoria == CHARCON){
            tipo_expr_cond = expr(em_qual_proced);
            if(associa_tipos_compat(_BOOL, tipo_expr_cond) != 0){ error("ERRO SEMANTICO > o tipo da expressão para o while deve ser booleana"); }

            //ja veio processado do final de fator < final de termo < final de expr_simples
            if(!(rcv_token.categoria == SNL && rcv_token.codigo == FECHA_PAREN)){
                error("ERRO SINTATICO > era esperado um ')' após a expressão no while");
            } else{
                rcv_token = AnaLex(arqivoProc);
                consome_fim_exp();
                
                while(rcv_token.categoria == PLV_RSVD || rcv_token.categoria == ID){
                    cmd(em_qual_proced);
                    consome_fim_exp();
                    if(rcv_token.categoria == PLV_RSVD && rcv_token.codigo == ENDW){ break; }
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
}