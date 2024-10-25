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

TOKEN AnaLex(FILE *arquivo){
    int estado; 
    char lexema[TAM_LEXEMA] = ""; 
    int tam_lexema = 0; 
    char digitos[TAM_NUM] = ""; 
    int tam_digito = 0; 
    TOKEN token_base; 

    estado = 0; //ja poderia colocar la em cima

    //começar processamento do afd
    while(true){
        char c = fgetc(arquivo);
        switch(estado){
            case 0:
                if(c == ' ' || c == '\t'){
                    estado = 0;
                } else if(c == '+'){ //nesses assim sem o outro* já monta e retorna o token
                    estado = 30;
                    token_base.categoria = SNL;
                    token_base.codigo = ADICAO;
                    return token_base;
                } 
                //CONTINUAR BOA SORTE
                break;
        }
    }
}

int main(){
    return 0;
}