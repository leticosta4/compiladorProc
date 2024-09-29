#include <stdio.h>

typedef enum {
    ID, PALAVRA_RESERVADA, SINAL, CONST_LITERAL, CONST_REAL, CONST_INTEIRA, COMENTARIO, ESP_VAZIO, OPER_LOGICO, OPER_ARITM, OPER_RELAC
}CATEGORIA_TOKEN;

//fazer mais enums de:
// PALAVRA_RESERVADA, SINAL, OPER_LOGICO, OPER_ARTIM, OPER_RELAC

typedef enum {
    ABRE_PAREN, FECHA_PAREN, ABRE_CHAVE, FECHA_CHAVE, ABRE_COL, FECHA_COL, VIRGULA, PONTO_VIRG
}SINAL;

typedef enum {
    E, OU, NAO
}OPER_LOGICO;

typedef enum {
    ATRIBUI, ADICAO, SUBTRACAO, MULTIPLICACAO, DIVISAO, MODULO
}OPER_ARITM;

typedef enum {
    IGUAL, DIFERENTE, MAIOR, MENOR, MAIOR_IGUAL, MENOR_IGUAL
}OPER_RELAC;

//struct do token
typedef struct {
    CATEGORIA_TOKEN cat;
    union {
        int codigo;
        int valor;
        char lexema[25]; //talvez mudar depois
    };
    
}TOKEN;

TOKEN analex(){
    TOKEN token_base;
 
    //processamento do AFD -> fazer

    return token_base; //futuramente p a anaSint
}
