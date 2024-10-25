#include <stdio.h> 
#include <stdlib.h> 
#include <ctype.h> 
#include <string.h> 
#include <stdbool.h> 

#include "analex.h" 

#define TAM_LEXEMA 50 
#define TAM_NUM 20 

void error(char msg[]){ 
    printf("%s\n", msg); 
    exit(1); 
} 

TOKEN AnaLex(FILE *fd){
    int estado; 
    char lexema[TAM_LEXEMA] = ""; 
    int tam_lexema = 0; 
    char digitos[TAM_NUM] = ""; 
    int tamD = 0; 
    TOKEN token_base; 

    //começar processamento do afd
}

int main(){
    return 0;
}