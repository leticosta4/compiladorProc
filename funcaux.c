#include <stdio.h> 
#include <stdlib.h> 
#include <ctype.h> 
#include <string.h> 
#include <stdbool.h> 
#include "analex.h"
#include "funcaux.h"

void error(char msg[]){ 
    printf("%s na linha %d\n", msg, contLinha); 
    exit(1); 
} 

int verifica_plv_rsvd(const char lexema[]){
    for(int i = 1; i <= NUM_PLV_RSVD; i++){
        if(strcmp(lexema, palavras_reservadas[i-1]) == 0){ return i; } //retorna o cod da plv_rsvd p associar no enum
    }
    return -1;
}

void printa_valor_token(){
    switch(rcv_token.categoria){
        case CHARCON:
            printf("valor do charcon: %c\n", rcv_token.c);
            break;
        case INTCON:
            printf("valor do intcon: %d\n", rcv_token.valor_inteiro);
            break;
        case REALCON:
            printf("valor do realcon: %f\n", rcv_token.valor_real);
            break;
        case STRINGCON:
            printf("valor da stringcon: %s\n", rcv_token.lexema);
            break;
        case ID:
            printf("valor do identificador: %s\n", rcv_token.lexema);
            break;
        case PLV_RSVD:
            printf("valor da palavra reservada: %s\n", palavras_reservadas[rcv_token.codigo]);
            break;
    }
}

void print_nodo_char(char info[], int mov){
    if(mov == AVANCA){
        printf("%s%s\n", TABS, info);
        strcat(TABS, "\t");
    } else if(mov == MANTEM){

    }
}
// void print_nodo_char(char info[], int movim){
 
//     if(movim )
//     if (movim == AVANCA) { printf("%s%s\n", TABS, info); strcat(TABS, "\t"); }
//     else if (movim == MANTEM) { printf("%s%s\n", TABS, info); }
//     else if (movim == RETROCEDE) { TABS[strlen(TABS)-1] = '\0'; }
// }

// void PrintNodoInt(int val, int movim) {
 
//     if (movim == AVANCA) { printf("%s%d\n", TABS, val); strcat(TABS, "\t"); }
//     else if (movim == MANTEM) { printf("%s%d\n", TABS, val); }
//     else if (movim == RETROCEDE) { TABS[strlen(TABS)-1] = '\0'; }
// }