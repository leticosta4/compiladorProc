#include <stdio.h>
#include <stdbool.h>
#ifndef ANALEX
#define ANALEX
#define TAM_MAX_LEXEMA 50
#define NUM_PLV_RSVD 28

typedef enum{
    CONST = 1,
    PR,
    INIT,
    INT,
    REAL,
    BOOL,
    CHAR,
    ENDP,
    ENDW,
    ENDI,
    ENDV,
    VAR,
    FROM,
    DO,
    WHILE,
    IF,
    ELIF,
    ELSE,
    DT,
    TO,
    BY,
    GETINT,
    GETREAL,
    GETCHAR,
    PUTINT,
    PUTREAL,
    PUTCHAR,
    GETOUT
}PALAVRA_RESERVADA;

typedef enum{
    ADICAO = 1,
    SUBTRACAO,
    MULTIPLICACAO,
    DIVISAO,
    ABRE_PAREN,
    FECHA_PAREN,
    ABRE_COL,
    FECHA_COL,
    ACESSO_END,
    AND_LOGICO,
    OR_LOGICO,
    MAIOR_IGUAL,
    MAIOR_QUE,
    MENOR_IGUAL,
    MENOR_QUE,
    ATRIBUICAO,
    COMP_IGUALDADE,
    COMP_DIFERENTE,
    NEGACAO,
    VIRGULA,
    PONTO
}SINAL;

typedef enum{
    ID = 1,
    CHARCON,
    STRINGCON,
    INTCON,
    REALCON,
    SNL,
    PLV_RSVD,
    FINAL_EXP,
    FINAL_ARQ
}CATEGORIA_TOKEN;

typedef struct {
    CATEGORIA_TOKEN categoria;
    union {
        int codigo; //se for da categoria sinal ou palavra reservada
        char lexema[TAM_MAX_LEXEMA]; //se for ID ou stringcon
        int valor_inteiro; //se for INTCON
        float valor_real; //se for REALCON
        char c; //se for CHARCON
    };
}TOKEN;

extern TOKEN rcv_token;
extern FILE *arqivoProc;
extern int contLinha;  
extern char palavras_reservadas[NUM_PLV_RSVD][TAM_MAX_LEXEMA];
TOKEN AnaLex(FILE *);

#endif