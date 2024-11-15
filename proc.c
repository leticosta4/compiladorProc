#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#include "analex.h"
#include "anasint.h"
#include "funcaux.h"

char palavras_reservadas[NUM_PLV_RSVD][TAM_MAX_LEXEMA] = {"const", "pr", "init", "int", "real", "bool", "char", "endp", "endw", "endi", "endv", "var", "from", "do", "while", "if", "elif", "else", "dt", "to", "by", "getint", "getreal", "getchar", "putint", "putreal", "putchar", "getout"};
FILE *arqivoProc;
TOKEN rcv_token;
int contLinha;
bool mostraArvore;
// char TABS[200] = "";

void testaAnalex();
void testaAnasint();

void testaAnalex(){
    if ((arqivoProc = fopen("testeProc.txt", "r")) == NULL){ printf("Arquivo de entrada da expressao nao encontrado!"); }

    printf("\nLINHA: %d\n\n", contLinha);

    //processamento de tokens
    while(1){
        rcv_token = AnaLex(arqivoProc);
        switch (rcv_token.categoria){
            case ID:
                printf("<ID, %s>\n", rcv_token.lexema);
                break;
            case CHARCON:
                printf("<CHARCON, %c>\n", rcv_token.c);
                break;
            case STRINGCON:
                printf("<STRINGCON, %s>\n", rcv_token.lexema);
                break;
            case INTCON:
                printf("<INTCON, %d>\n", rcv_token.valor_inteiro);
                break;
            case REALCON:
                printf("<REALCON, %f>\n", rcv_token.valor_real);
                break;
            case SNL:
                switch(rcv_token.codigo){
                    case ADICAO:
                        printf("<SNL, ADICAO>\n");
                        break;
                    case SUBTRACAO:
                        printf("<SNL, SUBTRACAO>\n");
                        break;
                    case MULTIPLICACAO:
                        printf("<SNL, MULTIPLICACAO>\n");
                        break;
                    case DIVISAO:
                        printf("<SNL, DIVISAO>\n");
                        break;
                    case ABRE_PAREN:
                        printf("<SNL, ABRE_PAREN>\n");
                        break;
                    case FECHA_PAREN:
                        printf("<SNL, FECHA_PAREN>\n");
                        break;
                    case ABRE_COL:
                        printf("<SNL, ABRE_COL>\n");
                        break;
                    case FECHA_COL:
                        printf("<SNL, FECHA_COL>\n");
                        break;
                    case ACESSO_END:
                        printf("<SNL, ACESSO_END>\n");
                        break;
                    case AND_LOGICO:
                        printf("<SNL, AND_LOGICO>\n");
                        break;
                    case OR_LOGICO:
                        printf("<SNL, OR_LOGICO>\n");
                        break;
                    case MAIOR_IGUAL:
                        printf("<SNL, MAIOR_IGUAL>\n");
                        break;
                    case MAIOR_QUE:
                        printf("<SNL, MAIOR_QUE>\n");
                        break;
                    case MENOR_IGUAL:
                        printf("<SNL, MENOR_IGUAL>\n");
                        break;
                    case MENOR_QUE:
                        printf("<SNL, MENOR_QUE>\n");
                        break;
                    case ATRIBUICAO:
                        printf("<SNL, ATRIBUICAO>\n");
                        break;
                    case COMP_IGUALDADE:
                        printf("<SNL, COMP_IGUALDADE>\n");
                        break;
                    case COMP_DIFERENTE:
                        printf("<SNL, COMP_DIFERENTE>\n");
                        break;
                    case NEGACAO:
                        printf("<SNL, NEGACAO>\n");
                        break;
                    case VIRGULA:
                        printf("<SNL, VIRGULA>\n");
                        break;
                    case PONTO:
                        printf("<SNL, PONTO>\n");
                        break;
                }
                break;
            case PLV_RSVD:
                switch(rcv_token.codigo){
                    case CONST:
                        printf("<PLV_RSVD, CONST>\n");
                        break;
                    case PR:
                        printf("<PLV_RSVD, PR>\n");
                        break;
                    case INIT:
                        printf("<PLV_RSVD, INIT>\n");
                        break;
                    case INT:
                        printf("<PLV_RSVD, INT>\n");
                        break;
                    case REAL:
                        printf("<PLV_RSVD, REAL>\n");
                        break;
                    case BOOL:
                        printf("<PLV_RSVD, BOOL>\n");
                        break;
                    case CHAR:
                        printf("<PLV_RSVD, CHAR>\n");
                        break;
                    case ENDP:
                        printf("<PLV_RSVD, ENDP>\n");
                        break;
                    case ENDW:
                        printf("<PLV_RSVD, ENDW>\n");
                        break;
                    case ENDI:
                        printf("<PLV_RSVD, ENDI>\n");
                        break;
                    case ENDV:
                        printf("<PLV_RSVD, ENDV>\n");
                        break;
                    case VAR:
                        printf("<PLV_RSVD, VAR>\n");
                        break;
                    case FROM:
                        printf("<PLV_RSVD, FROM>\n");
                        break;
                    case DO:
                        printf("<PLV_RSVD, DO>\n");
                        break;
                    case WHILE:
                        printf("<PLV_RSVD, WHILE>\n");
                        break;
                    case IF:
                        printf("<PLV_RSVD, IF>\n");
                        break;
                    case ELIF:
                        printf("<PLV_RSVD, ELIF>\n");
                        break;
                    case ELSE:
                        printf("<PLV_RSVD, ELSE>\n");
                        break;
                    case DT:
                        printf("<PLV_RSVD, DT>\n");
                        break;
                    case TO:
                        printf("<PLV_RSVD, TO>\n");
                        break;
                    case BY:
                        printf("<PLV_RSVD, BY>\n");
                        break;
                    case GETINT:
                        printf("<PLV_RSVD, GETINT>\n");
                        break;
                    case GETREAL:
                        printf("<PLV_RSVD, GETREAL>\n");
                        break;
                    case GETCHAR:
                        printf("<PLV_RSVD, GETCHAR>\n");
                        break;
                    case PUTINT:
                        printf("<PLV_RSVD, PUTINT>\n");
                        break;
                    case PUTREAL:
                        printf("<PLV_RSVD, PUTREAL>\n");
                        break;
                    case PUTCHAR:
                        printf("<PLV_RSVD, PUTCHAR>\n");
                        break;
                    case GETOUT:
                        printf("<PLV_RSVD, GETOUT>\n");
                        break;
                }
                break;
            case FINAL_EXP:
                printf("<FINAL_EXP, %d>\n", 0);
                printf("\nLINHA: %d\n\n", contLinha);
                break;
            case FINAL_ARQ:
                printf("<FIM DO ARQUIVO ENCONTRADO>\n");
                break;
        }

        if(rcv_token.categoria == FINAL_ARQ){ break; }
    }

    fclose(arqivoProc);
}

void testaAnasint(){
    if ((arqivoProc = fopen("testeProc.txt", "r")) == NULL){ printf("Arquivo de entrada da expressao nao encontrado!"); }

    printf("\nLINHA: %d\n\n", contLinha);

    while(true){
        rcv_token = AnaLex(arqivoProc);
        if(rcv_token.categoria == FINAL_ARQ){
            printf("\nFim do arquivo fonte encontrado!\n");
            break;
        }
        //chamar função atrib do anasint => no caso vai ser a prog
        if(rcv_token.categoria == FINAL_EXP){
            printf("\nLINHA %d: Expressão sintaticamente correta!\n\n", contLinha - 1);
        } else{
            error("Erro de sintaxe!\n");
        }
    }

    fclose(arqivoProc);
}

int main(){
    //system("chcp 65001");  // Troca a página de códigos de caracteres da console para UTF-8 na execução
    
    contLinha = 1;
    printf("\n\n[Análise Léxica -------------------]\n");
    testaAnalex();

    // contLinha = 1;
    // mostraArvore = false;
    // printf("\n\n[Análise Sintática ----------------]\n");
    // testaAnasint();
    
    return 0;
}