#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "tabsimb.h"
#include "funcaux.h"

tipo_tab_simb tabela_simbolos;
char escopos[2][TAM_MAX_LEXEMA] = {"global", "local"};
char tipos[5][TAM_MAX_LEXEMA] = {"_nao_aplica_tipo", "_int", "_real", "_char", "_bool"};
char categorias[5][TAM_MAX_LEXEMA] = {"var_global", "var_local", "procedimento", "parametro", "prototipo"};
char passagens[3][TAM_MAX_LEXEMA] = {"nao_aplica_param", "valor", "referencia"};
char zumbis[3][TAM_MAX_LEXEMA] = {"nao_aplica_zumbi", "vivo", "eh_zumbi"};
char arrays[4][TAM_MAX_LEXEMA] = {"nao_aplica_array", "var_simples", "id_vetor", "id_matriz"};
char constantes[2][TAM_MAX_LEXEMA] = {"nao", "sim"};
char tem_prototipos[3][TAM_MAX_LEXEMA] = {"nao_aplica_prot", "_nao", "_sim"};

void limpar_tabsimb(){
    memset(&tabela_simbolos, 0, sizeof(tabela_simbolos));
}

void inserir_tabsimb(registro_tabsimb token_ins){ 
    printf("iniciada inserção na tabela de símbolos\n\n");

    if(tabela_simbolos.topo >= TAM_MAX_TAB){
        printf("ERRO TABSIMB > tabela de símbolos cheia.\n");
        exit(1);
    }

    //verifica_redeclaracao(token_ins); //tem que verificar se o token a ser inserido ja n tem na tabela
    token_ins.endereco = tabela_simbolos.topo;
    tabela_simbolos.linhas[tabela_simbolos.topo] = token_ins;
    tabela_simbolos.topo++;

    printar_tabsimb();
}

registro_tabsimb limpar_dimensoes_array(registro_tabsimb used_token){
    used_token.dimensoes_array[0] = 0;
    used_token.dimensoes_array[1] = 0;
    return used_token;
}

void verifica_redeclaracao(registro_tabsimb token_aux){
    printf("iniciada busca POR REDECLARAÇÃO DE VARIAVEL GLOBAL, PROTOTIPO OU PROCEDIMENTO na tabela de símbolos\n\n");

    for(int i = 0; i < tabela_simbolos.topo; i++){
        if((strcmp(tabela_simbolos.linhas[i].lexema, token_aux.lexema) == 0) && (tabela_simbolos.linhas[i].categoria == token_aux.categoria)){
            switch(token_aux.categoria){
                case VAR_GLOBAL:
                    error("ERRO SEMANTICO > foi encontrada redeclaração de variável global");
                    break;
                case VAR_LOCAL:
                    error("ERRO SEMANTICO > foi encontrada redeclaração de variável local");
                    break;
                case PROTOTIPO:
                    error("ERRO SEMANTICO > foi encontrada redeclaração de protótipo de procedimento");
                    break;
                case PROCEDIMENTO:
                    if(strcmp(tabela_simbolos.linhas[i].lexema, "init") == 0){
                        error("ERRO SEMANTICO > deve haver somente um bloco init");
                    } else{ error("ERRO SEMANTICO > foi encontrada redeclaração de procedimento"); }
                    break;
            }
        }
    }

    //verificar lexema e escopo talvez
    
    printf("fim da busca TS\n\n");
}

void verifica_redecl_param(int pos_proced, char nome_param[]){ //what_cat: 1 = var_local; 3 = param
    printf("verificando repetição dos parametros do procedimento\n");

    if(pos_proced >= 0){
        for(int i = pos_proced + 1; i <= tabela_simbolos.topo; i++){
            if(tabela_simbolos.linhas[i].categoria != PARAMETRO){ break; }
            if((tabela_simbolos.linhas[i].categoria == PARAMETRO) && (strcmp(tabela_simbolos.linhas[i].lexema, nome_param) == 0)){
                error("ERRO SEMANTICO > foi encontrada repetição de parametro para o procedimento");
            }
        }
    }    

    printf("acabou a verificação de repetição dos parametros do procedimento\n");
}

void remover_tabsimb(){
    printf("iniciada remoção na tabela de símbolos\n\n");
    if (tabela_simbolos.topo > 0) {
        tabela_simbolos.topo--;
    } else {
        printf("ERRO TABSIMB > Tabela de símbolos já vazia.\n");
    }
    printf("fim da remoção TS\n\n");
}

int procura_posicao_proc(char nome_def[]){
    printf("buscando a posição de um procedimento\n");
    
    int i;
    for(i = 0; i < tabela_simbolos.topo; i++){
        if((strcmp(tabela_simbolos.linhas[i].lexema, nome_def) == 0) && (tabela_simbolos.linhas[i].categoria == PROCEDIMENTO)){
            return i;
        }
    }
    if(i >= tabela_simbolos.topo){
        error("parte de parametros/var_locais > procedimento não encontrado"); 
    }

    printf("terminou a busca da posição do procedimento\n");
}

int procura_existencia_prototipo_ou_proced(char nome_prot[]){
    printf("verificando a existencia da assinatura de prototipo/definição para o procedimento\n");

    int j;
    for(j = 0; j < tabela_simbolos.topo; j++){
        if(((strcmp(tabela_simbolos.linhas[j].lexema, nome_prot) == 0) && ((tabela_simbolos.linhas[j].categoria == PROTOTIPO) || (tabela_simbolos.linhas[j].categoria == PROCEDIMENTO))) ||
           ((strcmp(tabela_simbolos.linhas[j].lexema, nome_prot) == 0) && (tabela_simbolos.linhas[j].categoria == PROCEDIMENTO) && (tabela_simbolos.linhas[j].tem_prototipo == _SIM))){ //no caso de ser chamado novamente a categoria ja vai ter mudado de prototipo p procedimento
            printf("prototipo/definição do procedimento encontrado!\n");
            return j;
        }
    }
    if(j >= tabela_simbolos.topo){
        printf("não existe assinatura de prototipo para esse procedimento nem a definição do mesmo\n");
        return -1;
    }

    printf("terminou a busca da existencia da assinatura de prototipo/definição para o procedimento\n");
}

void substituir_prot_proc(int posicao_prot, registro_tabsimb token_proced){
    printf("substituindo prototipo por procedimento\n");
    if(posicao_prot >= 0){
        token_proced.endereco = tabela_simbolos.linhas[posicao_prot].endereco; //p n reinicializar
        tabela_simbolos.linhas[posicao_prot] = token_proced;
    }

    printar_tabsimb();
    printf("acabou a substituição de prototipo por procedimento\n");
}

void substituir_parametros_prot_proc_testar_compat_tipos(int posicao_prot, registro_tabsimb token_param_proced){
    printf("substituindo os parametros dos prototipos para os do procedimento\n");

    if(posicao_prot >= 0){
        for(int i = posicao_prot + 1; i <= tabela_simbolos.topo; i++){
            if(tabela_simbolos.linhas[i].categoria != PARAMETRO){ break; }
            if(tabela_simbolos.linhas[i].categoria == PARAMETRO){
                if(tabela_simbolos.linhas[i].tipo != token_param_proced.tipo){
                    error("ERRO SEMÂNTICO > os parametros de procedimento devem possuir o mesmo tipo");
                } else{
                    if(tabela_simbolos.linhas[i].array != token_param_proced.array){
                        error("ERRO SEMÂNTICO > os parametros de procedimento devem ser compatíveis [ou array ou varável escalar]");
                    }
                }
                if(strcmp(tabela_simbolos.linhas[i].lexema, "") == 0){ //essa verificacao do lexema vazio é especifico p substituicao dos params no prototipo
                    token_param_proced.endereco = tabela_simbolos.linhas[i].endereco; //p n reinicializar
                    tabela_simbolos.linhas[i] = token_param_proced;
                    break;
                }
            }
        }
    }    

    printar_tabsimb();
    printf("acabou a substituição de parametros dos prototipos para os do procedimento\n");
}

int contar_params(int posicao_prot_def){
    int cont_param = 0, i;
    if(posicao_prot_def >= 0){
        for(i = posicao_prot_def + 1; i <= tabela_simbolos.topo; i++){
            if(tabela_simbolos.linhas[i].categoria != PARAMETRO){ break;
            } else{ cont_param++; }
        }
    } 

    if(i >= tabela_simbolos.topo){
        error("nenhum parametro encontrado"); 
    }
    return cont_param;
}

void apagar_var_locais(int posicao_def){
    printf("apagando variaveis locais\n");

    if(posicao_def >= 0){
        for(int i = posicao_def; i <= tabela_simbolos.topo; i++){
            while(tabela_simbolos.linhas[i+1].categoria == PARAMETRO){
                i++;
            } 
            if(tabela_simbolos.linhas[i+1].categoria == VAR_LOCAL){ remover_tabsimb(); }
            else{ break; }
                
        }
    }
    printf("acabou a remoção das var locais\n");
    //printar_tabsimb();
}

void transformar_zumbi(int posicao_def){
    printf("transformando em zumbi\n");
    
    if(posicao_def >= 0){
        for(int i = posicao_def + 1; i <= tabela_simbolos.topo; i++){
            if(tabela_simbolos.linhas[i].categoria != PARAMETRO){ break; }

            if(tabela_simbolos.linhas[i].zumbi == VIVO){
                tabela_simbolos.linhas[i].zumbi = EH_ZUMBI;
            }
        }
    }
    printf("acabou a transformação de vivo em zumbi\n");
    //printar_tabsimb();
}

void printar_tabsimb(){
    printf("\nTabela de Símbolos:\n");
    printf("--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("| %-6s | %-16s | %-13s | %-16s | %-15s | %-16s | %-16s | %-16s | %-6s | %-6s | %-6s | %-15s | %-8s | %-12s  |\n", 
           "ID", "Lexema", "Escopo", "Tipo", "Categoria", "Passagem", "Zumbi", "Array?", "Dim-1", "Dim-2", "Constante?", "Valor Constante", "Endereço", "Tem prototipo?");
    printf("--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < tabela_simbolos.topo; i++){
        printf("| %-6d | %-16s | %-13s | %-16s | %-15s | %-16s | %-16s | %-16s | %-6d | %-6d | %-10s",
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
                constantes[tabela_simbolos.linhas[i].constante]);
        if(tabela_simbolos.linhas[i].constante == SIM){
            switch(tabela_simbolos.linhas[i].tipo){
                case _INT:
                    printf(" | %-15d |", tabela_simbolos.linhas[i].valor_constante.inteiro);
                    break;
                case _BOOL:
                    printf(" | %-15d |", tabela_simbolos.linhas[i].valor_constante.v_bool);
                    break;
                case _REAL:
                    printf(" | %-15f |", tabela_simbolos.linhas[i].valor_constante.real);
                    break;
                case _CHAR:
                    printf(" | %-15c |", tabela_simbolos.linhas[i].valor_constante.v_char);
                    break;
            } 
        } else{ printf(" | %-15d |", (i * 0)); }
        printf(" %-8d | %-15s |\n", tabela_simbolos.linhas[i].endereco, tem_prototipos[tabela_simbolos.linhas[i].tem_prototipo]);
    printf("--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    }
}

void verifica_compatibilidade_tipo(TOKEN tk, registro_tabsimb info_tk){
    if(info_tk.tipo == _INT && (tk.categoria != INTCON && tk.categoria != CHARCON)){
        error("ERRO SEMANTICO > variável do tipo int é compatível somente com inteiro ou caracter");
    } else if(info_tk.tipo == _CHAR){
        if((info_tk.array == ID_VETOR || info_tk.array == ID_MATRIZ) && tk.categoria != STRINGCON){
            error("ERRO SEMANTICO > array tipo char é compatível somente com string de caracteres");
        } else if(info_tk.array == VAR_SIMPLES && (tk.categoria != CHARCON && tk.categoria != INTCON)){
            error("ERRO SEMANTICO > variável do tipo char é compatível somente com caracter ou inteiro");
        } 
    } else if(info_tk.tipo == _BOOL){
        if(info_tk.array != VAR_SIMPLES){
            error("ERRO SEMANTICO > variável do tipo bool deve ser escalar somente");
        } else if(info_tk.array == VAR_SIMPLES && (tk.categoria != INTCON && tk.categoria != CHARCON)){
            error("ERRO SEMANTICO > variável do tipo bool é compatível somente com inteiro");
        }
    } else if(info_tk.tipo == _REAL && tk.categoria != REALCON){
        error("ERRO SEMANTICO > variável do tipo real é compatível somente com real");
    }
}

void prototipo_sozinho(){
    for(int i = 0; i <= tabela_simbolos.topo; i++){
        if(tabela_simbolos.linhas[i].categoria == PROTOTIPO){
            error("ERRO SEMÂNTICO > não deve existir prototipo sem procedimento declarado");
        }
    }
}

registro_tabsimb procura_existencia_identificador_em_proced(int posicao_procedimento, char nome_identificador[]){
    int i, diff = 0;
    if(posicao_procedimento >= 0){
        for(i = posicao_procedimento + 1; i <= tabela_simbolos.topo; i++){
            if(tabela_simbolos.linhas[i].categoria != PARAMETRO && tabela_simbolos.linhas[i].categoria != VAR_LOCAL){ diff = 1; break; }
            else{
                if(strcmp(tabela_simbolos.linhas[i].lexema, nome_identificador) == 0){
                    printf("achou var-local/param equivalente ao identificador\n");
                    return tabela_simbolos.linhas[i];
                }
            }
        }
        if((i >= tabela_simbolos.topo) || diff == 1){
            for(i = 0; i < tabela_simbolos.topo; i++){
                if(tabela_simbolos.linhas[i].categoria != VAR_GLOBAL){ diff = 2; break; }
                else{
                    if(strcpy(tabela_simbolos.linhas[i].lexema, nome_identificador) == 0){
                        printf("achou var-global equivalente ao identificador\n");
                        return tabela_simbolos.linhas[i];
                    }
                }
            }
            if((i >= tabela_simbolos.topo) || diff != 0){
                printf("%s\n", tabela_simbolos.linhas[i].lexema);
                error("não foi encontrada declaração do identificador");
            }
        }
    }
}