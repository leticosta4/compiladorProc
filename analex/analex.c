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
        char caracter = fgetc(arquivo);
        switch(estado){
            case 0:
                if(caracter == ' ' || caracter == '\t'){
                    estado = 0;
                } else if(caracter == '+'){ //nesses assim sem o outro* já monta e retorna o token
                    estado = 30;
                    token_base.categoria = SNL;
                    token_base.codigo = ADICAO;
                    token_base.c = caracter; 
                    return token_base;
                } else if(caracter == '_'){ //inicio de ID
                    estado = 1;
                    lexema[tam_lexema] = caracter;
                    lexema[++tam_lexema] = '\0';  
                } else if((caracter >= 'a' && caracter <= 'z') || (caracter >= 'A' && caracter <= 'Z')){
                    estado = 2;
                    lexema[tam_lexema] = caracter;
                    lexema[++tam_lexema] = '\0'; 
                }
                //stringcon, charcon, intcon, realcon FAZER
                else if(caracter == '-'){ 
                    estado = 23;
                    token_base.categoria = SNL;
                    token_base.codigo = SUBTRACAO;
                    return token_base;
                } else if(caracter == '*'){ 
                    estado = 24;
                    token_base.categoria = SNL;
                    token_base.codigo = MULTIPLICACAO;
                    return token_base;
                } else if(caracter == ','){ 
                    estado = 47;
                    token_base.categoria = SNL;
                    token_base.codigo = VIRGULA;
                    return token_base;
                } else if(caracter == '.'){ //realmente precisa???????
                    estado = 48;
                    token_base.categoria = SNL;
                    token_base.codigo = PONTO;
                    return token_base;
                } else if(caracter == '('){ 
                    estado = 28;
                    token_base.categoria = SNL;
                    token_base.codigo = ABRE_PAREN;
                    return token_base;
                } else if(caracter == ')'){ 
                    estado = 29;
                    token_base.categoria = SNL;
                    token_base.codigo = FECHA_PAREN;
                    return token_base;
                } else if(caracter == '['){ 
                    estado = 31;
                    token_base.categoria = SNL;
                    token_base.codigo = ABRE_COL;
                    return token_base;
                } else if(caracter == ']'){ 
                    estado = 32;
                    token_base.categoria = SNL;
                    token_base.codigo = FECHA_COL;
                    return token_base;
                } 
                //esses abaixo ainda dependem do proxio caracter que se segue => tratado em outros cases (outros estados)
                else if(caracter == '/'){ estado = 19; } //TIRAR DUVIDA
                else if(caracter == '='){ estado = 25; }
                else if(caracter == '!'){ estado = 33; }
                else if(caracter == '&'){ estado = 36; }
                else if(caracter == '>'){ estado = 39; }
                else if(caracter == '<'){ estado = 42; }
                else if(caracter == '|'){ estado = 45; }
                else if(caracter == '\n'){
                    estado = 0;
                    token_base.categoria = FINAL_EXP;
                    linha++;
                    return token_base;
                } else if(caracter == EOF){
                    token_base.categoria = FINAL_ARQ;
                    return token_base;
                } else {
                    error("Caracter invalido na expressao!");
                }
                break;

            case 1: //underline ou letra
                if(caracter == '_'){
                    estado = 1;
                    lexema[tam_lexema] = caracter;
                    lexema[++tam_lexema] = '\0';  
                } else{
                    estado = 2;
                    lexema[tam_lexema] = caracter;
                    lexema[++tam_lexema] = '\0';  
                } 
                break;
            case 2: //precisa ser letra, digito ou underline SE NAO FOR DEVOLVE O OUTRO
                if((caracter == '_') || (caracter >= 'a' && caracter <= 'z') || (caracter >= 'A' && caracter <= 'Z') || (caracter >= '0' && caracter <= '9')){
                    estado = 2;
                    lexema[tam_lexema] = caracter;
                    lexema[++tam_lexema] = '\0';  
                } else{
                    estado = 3;
                    ungetc(caracter, arquivo);

                    //ver se é uma palavra reservada
                    if(strcmp(lexema, "const") == 0){
                        token_base.categoria = PLV_RSVD;
                        token_base.codigo = CONST;
                    } 
                    //fazer todos os outros elifs das palavras reservadas
                    else{
                        token_base.categoria = ID;
                        strcpy(token_base.lexema, lexema);
                    }
                    return token_base;
                }
                break;
            case 19: //comentario ou divisao
                //?????
                break;
            case 25: //igualdade ou atribuicao
                if(caracter == '='){
                    estado = 26;
                    token_base.categoria = SNL;
                    token_base.codigo = COMP_IGUALDADE;
                    return token_base;
                } else{
                    estado = 27;
                    ungetc(caracter, arquivo); //devolver o outro* com ungetc
                    token_base.categoria = SNL;
                    token_base.codigo = ATRIBUICAO;
                    return token_base;
                }
                break;

            case 33: //negação ou diferente
                if(caracter == '='){
                    estado = 34;
                    token_base.categoria = SNL;
                    token_base.codigo = COMP_DIFERENTE;
                    return token_base;
                } else {
                    estado = 35;
                    ungetc(caracter, arquivo);
                    token_base.categoria = SNL;
                    token_base.codigo = NEGACAO;
                    return token_base;
                }
                break;

            case 36: //endereco ou AND
                if(caracter == '&'){
                    estado = 37;
                    token_base.categoria = SNL;
                    token_base.codigo = AND_LOGICO;
                    return token_base;
                } else {
                    estado = 38;
                    ungetc(caracter, arquivo);
                    token_base.categoria = SNL;
                    token_base.codigo = ACESSO_END;
                    return token_base;
                }
                break;

            case 39: //maior igual ou maior que
                if(caracter == '='){
                    estado = 40;
                    token_base.categoria = SNL;
                    token_base.codigo = MAIOR_IGUAL;
                    return token_base;
                } else {
                    estado = 41;
                    ungetc(caracter, arquivo);
                    token_base.categoria = SNL;
                    token_base.codigo = MAIOR_QUE;
                    return token_base;
                }
                break;

            case 42:
                if(caracter == '='){
                    estado = 43;
                    token_base.categoria = SNL;
                    token_base.codigo = MENOR_IGUAL;
                    return token_base;
                } else{
                    estado = 44;
                    ungetc(caracter, arquivo);
                    token_base.categoria = SNL;
                    token_base.codigo = MENOR_QUE;
                    return token_base;
                }
                break;

            case 45:
                if(caracter == '|'){
                    estado = 46;
                    token_base.categoria = SNL;
                    token_base.codigo = OR_LOGICO;
                    return token_base;
                }
                break;
        }
    }
}

int main(){
    return 0;
}