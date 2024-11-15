#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

// void PrintNodo(char info[], int movim) {
 
//     if (movim == AVANCA) { printf("%s%s\n", TABS, info); strcat(TABS, "\t"); }
//     else if (movim == MANTEM) { printf("%s%s\n", TABS, info); }
//     else if (movim == RETROCEDE) { TABS[strlen(TABS)-1] = '\0'; }
// }

// void PrintNodoInt(int val, int movim) {
 
//     if (movim == AVANCA) { printf("%s%d\n", TABS, val); strcat(TABS, "\t"); }
//     else if (movim == MANTEM) { printf("%s%d\n", TABS, val); }
//     else if (movim == RETROCEDE) { TABS[strlen(TABS)-1] = '\0'; }
// }