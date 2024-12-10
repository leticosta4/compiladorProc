#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "tabsimb.h"
#include "funcaux.h"

tipo_tab_simb tabela_simbolos;
char escopos[2][TAM_MAX_LEXEMA] = {"externo_proc", "interno_proc"};
char tipos[5][TAM_MAX_LEXEMA] = {"_nao_aplica_tipo", "_int", "_real", "_char", "_bool"};
char categorias[5][TAM_MAX_LEXEMA] = {"var_global", "var_local", "procedimento", "parametro", "prototipo"};
char passagens[3][TAM_MAX_LEXEMA] = {"nao_aplica_param", "valor", "referencia"};
char zumbis[3][TAM_MAX_LEXEMA] = {"nao_aplica_zumbi", "vivo", "eh_zumbi"};
char arrays[4][TAM_MAX_LEXEMA] = {"nao_aplica_array", "var_simples", "id_vetor", "id_matriz"};
char constantes[2][TAM_MAX_LEXEMA] = {"nao", "sim"};

void iniciar_tabsimb(){
    printf("tabela de símbolos inicializada\n\n");
    //talvez algo que limpe a tabela tb
    tabela_simbolos.topo = 0;
}

void inserir_tabsimb(registro_tabsimb token_ins){ 
    printf("iniciada inserção na tabela de símbolos\n\n");

    if(tabela_simbolos.topo >= TAM_MAX_TAB){
        printf("ERRO TABSIMB > tabela de símbolos cheia.\n");
        exit(1);
    }

    verifica_redeclaracao(token_ins); //tem que verificar se o token a ser inserido ja n tem na tabela

    token_ins.endereco = tabela_simbolos.topo;
    tabela_simbolos.linhas[tabela_simbolos.topo] = token_ins;
    tabela_simbolos.topo++;

    printar_tabsimb();
}

void verifica_redeclaracao(registro_tabsimb token_aux){
    printf("iniciada busca POR REDECLARAÇÃO DE VARIAVEL OU PROCEDIMENTO na tabela de símbolos\n\n");

    for(int i = 0; i < tabela_simbolos.topo; i++){
        if((strcmp(tabela_simbolos.linhas[i].lexema, token_aux.lexema) == 0) && (tabela_simbolos.linhas[i].categoria == token_aux.categoria)){
            switch(token_aux.categoria){
                case VAR_GLOBAL:
                    error("ERRO TABSIMB > foi encontrada redeclaração de variável global");
                    break;
                case VAR_LOCAL:
                    error("ERRO TABSIMB > foi encontrada redeclaração de variável local");
                    break;
                case PROTOTIPO:
                    error("ERRO TABSIMB > foi encontrada redeclaração de protótipo de procedimento");
                    break;
                case PROCEDIMENTO:
                    if(strcmp(tabela_simbolos.linhas[i].lexema, "init")){
                        error("ERRO TABSIMB > deve haver somente um bloco init");
                    } else{ error("ERRO TABSIMB > foi encontrada redeclaração de procedimento"); }
                    break;
            }
        }
    }

    //verificar lexema e escopo talvez
    
    printf("fim da busca TS\n\n");
}

registro_tabsimb verifica_declaracao(registro_tabsimb token_aux){ //REVISAR
    printf("iniciada busca por DECLARACAO DE CONSTANTE, PROTOTIPO E VARIAVEL"); //VER AINDA COMO USAR P VARIAVEL EM ATRIB PROVAVELMENTE
    registro_tabsimb tk_ret;
    int i;

    for(i = 0; i < tabela_simbolos.topo; i++){
        if(strcmp(tabela_simbolos.linhas[i].lexema, token_aux.lexema) == 0){
            //garantir mesma cat p diferenciar variavel e prototipo, por exemplo
            if((token_aux.categoria == PROCEDIMENTO) && (tabela_simbolos.linhas[i].categoria == PROTOTIPO)){
                printf("achou a declaração do prototipo para este procedimento\n");
                tk_ret = tabela_simbolos.linhas[i];
                break;
            } else if((tabela_simbolos.linhas[i].categoria == VAR_GLOBAL) || (tabela_simbolos.linhas[i].categoria == VAR_LOCAL)){
                printf("achou variavel equivalente a constante para tamanho do array\n");
                tk_ret = tabela_simbolos.linhas[i];
                break;
            }
        }
    }
    if(i >= tabela_simbolos.topo){
        error("ERRO TAB_SIMB > declaração não encontrada");
    } else{ return tk_ret; }
}

// int buscar_posicao_prox_token(){
//     printf("iniciada busca DA PROX POSICAO DE INSERCAO na tabela de símbolos\n\n");
//     //vai ser chamado em anasint.c antes de chamar a funcao de inserir
//     return 0;
// }


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
        printf("| %-6d | %-10s | %-6s | %-10s | %-10s | %-10s | %-6s | %-8s | %-6d | %-6d | %-8s | %-8s | %-8d |\n",
               i,
               tabela_simbolos.linhas[i].lexema,
               escopos[tabela_simbolos.linhas[i].escopo],
               tipos[tabela_simbolos.linhas[i].tipo],
               categorias[tabela_simbolos.linhas[i].categoria],
               passagens[tabela_simbolos.linhas[i].passagem],
               zumbis[tabela_simbolos.linhas[i].zumbi],
               arrays[tabela_simbolos.linhas[i].array],
               tabela_simbolos.linhas[i].dimensoes_array[0],
               tabela_simbolos.linhas[i].dimensoes_array[1],
               constantes[tabela_simbolos.linhas[i].constante],
               tabela_simbolos.linhas[i].valor_constante,
               tabela_simbolos.linhas[i].endereco);
        printf("-------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    }
}