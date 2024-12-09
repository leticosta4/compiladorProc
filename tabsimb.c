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
char constantes[2][TAM_MAX_LEXEMA] = {"nao", "sim"};

void iniciar_tabsimb(){
    printf("tabela de símbolos inicializada\n\n");
    //talvez algo que limpe a tabela tb
    tabela_simbolos.topo = 0;
}

void inserir_tabsimb(registro_tabsimb info_token){ 
    printf("iniciada inserção na tabela de símbolos\n\n");

    if(tabela_simbolos.topo >= TAM_MAX_TAB){
        printf("ERRO TABSIMB > tabela de símbolos cheia.\n");
        exit(1);
    }

    buscar_redecl_var_proc(info_token); //tem que verificar se o token a ser inserido ja n tem na tabela

    info_token.endereco = tabela_simbolos.topo;
    tabela_simbolos.linhas[tabela_simbolos.topo] = info_token;
    tabela_simbolos.topo++;

    printar_tabsimb();
}

void buscar_redecl_var_proc(registro_tabsimb info_token){
    printf("iniciada busca POR REDECLARAÇÃO DE VARIAVEL OU PROCEDIMENTO na tabela de símbolos\n\n");

    for(int i = 0; i < tabela_simbolos.topo; i++){
        if(strcmp(tabela_simbolos.linhas[i].lexema, info_token.lexema) == 0){
            if(tabela_simbolos.linhas[i].categoria == VAR_GLOBAL && info_token.categoria == VAR_GLOBAL){
                error("ERRO TABSIMB > foi encontrada redeclaração de variável global");
            }
            if(tabela_simbolos.linhas[i].categoria == VAR_LOCAL && info_token.categoria == VAR_LOCAL){
                error("ERRO TABSIMB > foi encontrada redeclaração de variável local");
            }
            if(tabela_simbolos.linhas[i].categoria == PROCEDIMENTO && info_token.categoria == PROCEDIMENTO){
                error("ERRO TABSIMB > foi encontrada redeclaração de procedimento");
            }
            if(tabela_simbolos.linhas[i].categoria == PROTOTIPO && info_token.categoria == PROTOTIPO){
                error("ERRO TABSIMB > foi encontrada redeclaração de protótipo de procedimento");
            }
        }
    }

    //verificar lexema e escopo talvez
    
    printf("fim da busca TS\n\n");
}

int buscar_posicao_prox_token(){
    printf("iniciada busca DA PROX POSICAO DE INSERCAO na tabela de símbolos\n\n");
    //vai ser chamado em anasint.c antes de chamar a funcao de inserir
    return 0;
}


void remover_tabsimb(){
    printf("iniciada remoção na tabela de símbolos\n\n");
    if (tabela_simbolos.topo > 0) {
        tabela_simbolos.topo--;
    } else {
        printf("ERRO TABSIMB > Tabela de símbolos já vazia.\n");
    }
    printar_tabsimb();
    printf("fim da remoção TS\n\n");
}

void printar_tabsimb(){
    printf("\nTabela de Símbolos:\n");
    printf("-------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("| %-6s | %-10s | %-6s | %-10s | %-10s | %-10s | %-6s | %-8s | %-6s | %-6s | %-6s | %-8s | %-8s |\n", 
           "Índice", "Lexema", "Escopo", "Tipo", "Categoria", "Passagem", "Zumbi", "Array?", "Dim-1", "Dim-2", "Constante?", "Valor COnstante", "Endereço");
    printf("-------------------------------------------------------------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < tabela_simbolos.topo; i++) {
        printf("| %-6d | %-10s | %-6s | %-10s | %-10s | %-10s | %-6s | %-8s | %-6s | %-6s | %-6s | %-8s | %-8s |\n",
               i,
               tabela_simbolos.linhas[i].lexema,
               escopos[tabela_simbolos.linhas[i].escopo],
               tipos[tabela_simbolos.linhas[i].tipo],
               categorias[tabela_simbolos.linhas[i].categoria],
               passagens[tabela_simbolos.linhas[i].passagem],
               zumbis[tabela_simbolos.linhas[i].zumbi],
               arrays[tabela_simbolos.linhas[i].array],
               tabela_simbolos.linhas[i].dimensao1,
               tabela_simbolos.linhas[i].dimensao2,
               constantes[tabela_simbolos.linhas[i].constante],
               tabela_simbolos.linhas[i].valor_constante,
               tabela_simbolos.linhas[i].endereco);
        printf("-------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    }
}