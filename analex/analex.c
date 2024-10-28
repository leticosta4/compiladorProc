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
                if(caracter == ' ' || caracter == '\t'){ //inicial volta no proprio estado zero
                    estado = 0;
                } else if(caracter == '_'){ //inicio de ID - mas o underline é opcional
                    estado = 1;
                    lexema[tam_lexema] = caracter;
                    lexema[++tam_lexema] = '\0';  
                } else if((caracter >= 'a' && caracter <= 'z') || (caracter >= 'A' && caracter <= 'Z')){ //inicio de ID
                    estado = 2;
                    lexema[tam_lexema] = caracter;
                    lexema[++tam_lexema] = '\0'; 
                } else if(caracter >= '0' && caracter <= '9'){ //inicio de INTCON ou REALCON
                    estado = 10;
                    digitos[tam_digito] = caracter;
                    digitos[++tam_digito] = '\0'; 
                } else if(caracter == '\''){ //inicio de CHARCON
                    estado = 4;
                    lexema[tam_lexema] = caracter;
                    lexema[++tam_lexema] = '\0'; 
                } else if(caracter == '"'){ //inicio de STRINGCON
                    estado = 15;
                    lexema[tam_lexema] = caracter;
                    lexema[++tam_lexema] = '\0';
                } else if(caracter == '+'){ //nesses assim sem o outro* já monta e retorna o token
                    estado = 21;
                    token_base.categoria = SNL;
                    token_base.codigo = ADICAO;
                    return token_base;
                } else if(caracter == '-'){ 
                    estado = 22;
                    token_base.categoria = SNL;
                    token_base.codigo = SUBTRACAO;
                    return token_base;
                } else if(caracter == '*'){ 
                    estado = 23;
                    token_base.categoria = SNL;
                    token_base.codigo = MULTIPLICACAO;
                    return token_base;
                } else if(caracter == ','){ 
                    estado = 24;
                    token_base.categoria = SNL;
                    token_base.codigo = VIRGULA;
                    return token_base;
                } else if(caracter == '('){ 
                    estado = 25;
                    token_base.categoria = SNL;
                    token_base.codigo = ABRE_PAREN;
                    return token_base;
                } else if(caracter == ')'){ 
                    estado = 26;
                    token_base.categoria = SNL;
                    token_base.codigo = FECHA_PAREN;
                    return token_base;
                } else if(caracter == '['){ 
                    estado = 27;
                    token_base.categoria = SNL;
                    token_base.codigo = ABRE_COL;
                    return token_base;
                } else if(caracter == ']'){ 
                    estado = 28;
                    token_base.categoria = SNL;
                    token_base.codigo = FECHA_COL;
                    return token_base;
                } 
                //esses abaixo ainda dependem do proxio caracter que se segue => tratado em outros cases (outros estados)
                else if(caracter == '/'){ estado = 17; } //TIRAR DUVIDA
                else if(caracter == '|'){ estado = 29; }
                else if(caracter == '!'){ estado = 31; }
                else if(caracter == '='){ estado = 34; }
                else if(caracter == '>'){ estado = 37; }
                else if(caracter == '<'){ estado = 40; }
                else if(caracter == '&'){ estado = 43; }
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
                    //ver se é uma PALAVRA RESERVADA
                    if(strcmp(lexema, "const") == 0){
                        token_base.categoria = PLV_RSVD;
                        token_base.codigo = CONST;
                    } else if(strcmp(lexema, "pr") == 0){
                        token_base.categoria = PLV_RSVD;
                        token_base.codigo = PR;
                    } else if(strcmp(lexema, "init") == 0){
                        token_base.categoria = PLV_RSVD;
                        token_base.codigo = INIT;
                    } else if(strcmp(lexema, "int") == 0){
                        token_base.categoria = PLV_RSVD;
                        token_base.codigo = INT;
                    } else if(strcmp(lexema, "real") == 0){
                        token_base.categoria = PLV_RSVD;
                        token_base.codigo = REAL;
                    } else if(strcmp(lexema, "bool") == 0){
                        token_base.categoria = PLV_RSVD;
                        token_base.codigo = BOOL;
                    } else if(strcmp(lexema, "char") == 0){
                        token_base.categoria = PLV_RSVD;
                        token_base.codigo = CHAR;
                    }  else if(strcmp(lexema, "endp") == 0){
                        token_base.categoria = PLV_RSVD;
                        token_base.codigo = ENDP;
                    } else if(strcmp(lexema, "endw") == 0){
                        token_base.categoria = PLV_RSVD;
                        token_base.codigo = ENDW;
                    } else if(strcmp(lexema, "endi") == 0){
                        token_base.categoria = PLV_RSVD;
                        token_base.codigo = ENDI;
                    } else if(strcmp(lexema, "endv") == 0){
                        token_base.categoria = PLV_RSVD;
                        token_base.codigo = ENDV;
                    } else if(strcmp(lexema, "var") == 0){
                        token_base.categoria = PLV_RSVD;
                        token_base.codigo = VAR;
                    } else if(strcmp(lexema, "from") == 0){
                        token_base.categoria = PLV_RSVD;
                        token_base.codigo = FROM;
                    } else if(strcmp(lexema, "do") == 0){
                        token_base.categoria = PLV_RSVD;
                        token_base.codigo = DO;
                    } else if(strcmp(lexema, "while") == 0){
                        token_base.categoria = PLV_RSVD;
                        token_base.codigo = WHILE;
                    } else if(strcmp(lexema, "if") == 0){
                        token_base.categoria = PLV_RSVD;
                        token_base.codigo = IF;
                    } else if(strcmp(lexema, "elif") == 0){
                        token_base.categoria = PLV_RSVD;
                        token_base.codigo = ELIF;
                    } else if(strcmp(lexema, "else") == 0){
                        token_base.categoria = PLV_RSVD;
                        token_base.codigo = ELSE;
                    } else if(strcmp(lexema, "dt") == 0){
                        token_base.categoria = PLV_RSVD;
                        token_base.codigo = DT;
                    } else if(strcmp(lexema, "to") == 0){
                        token_base.categoria = PLV_RSVD;
                        token_base.codigo = TO;
                    } else if(strcmp(lexema, "by") == 0){
                        token_base.categoria = PLV_RSVD;
                        token_base.codigo = BY;
                    } else if(strcmp(lexema, "getint") == 0){
                        token_base.categoria = PLV_RSVD;
                        token_base.codigo = GETINT;
                    } else if(strcmp(lexema, "getreal") == 0){
                        token_base.categoria = PLV_RSVD;
                        token_base.codigo = GETREAL;
                    } else if(strcmp(lexema, "getchar") == 0){
                        token_base.categoria = PLV_RSVD;
                        token_base.codigo = GETCHAR;
                    } else if(strcmp(lexema, "putint") == 0){
                        token_base.categoria = PLV_RSVD;
                        token_base.codigo = PUTINT;
                    } else if(strcmp(lexema, "putreal") == 0){
                        token_base.categoria = PLV_RSVD;
                        token_base.codigo = PUTREAL;
                    } else if(strcmp(lexema, "putchar") == 0){
                        token_base.categoria = PLV_RSVD;
                        token_base.codigo = PUTCHAR;
                    } else if(strcmp(lexema, "getout") == 0){
                        token_base.categoria = PLV_RSVD;
                        token_base.codigo = GETOUT;
                    } else{
                        token_base.categoria = ID;
                        strcpy(token_base.lexema, lexema);
                    }
                    return token_base;
                }
                break;
            case 4: //caracter qualquer ou barra descrescente
                if(caracter == '\\'){
                    estado = 6;
                    lexema[tam_lexema] = caracter;
                    lexema[++tam_lexema] = '\0';
                } else {
                    estado = 5;
                    lexema[tam_lexema] = caracter;
                    lexema[++tam_lexema] = '\0';
                }
                break;
            case 5:
                if(caracter == '\''){ //inicio de CHARCON
                    estado = 9;
                    // lexema[tam_lexema] = caracter;
                }
                break;
            case 6: //n ou 0 => quebra de linha ou espaço vazio
                if(caracter == 'n'){
                    estado = 7;
                    lexema[tam_lexema] = caracter;
                    lexema[++tam_lexema] = '\0';
                } else if(caracter == '0'){
                    estado = 8;
                    lexema[tam_lexema] = caracter;
                    lexema[++tam_lexema] = '\0';
                } else {
                    error("erro no char - não reconhecido");
                }
            case 7:
                if(caracter == '\''){ 
                    estado = 9;
                    // lexema[tam_lexema] = caracter;
                }
                break;
            case 8:
                if(caracter == '\''){ 
                    estado = 9;
                    // lexema[tam_lexema] = caracter;
                }
                break;
            case 9: //CHARCON
                token_base.categoria = CHARCON;
                token_base.c = lexema[--tam_lexema];
                return token_base;
                break;
            case 10:
                if(caracter >= '0' && caracter <= '9'){ 
                    estado = 10;
                    digitos[tam_digito] = caracter;
                    digitos[++tam_digito] = '\0'; 
                } else if(caracter == '.'){ //INICIO DE REALCON
                    estado = 12;
                    digitos[tam_digito] = caracter;
                    digitos[++tam_digito] = '\0'; 
                } else { //INTCON
                    estado = 11;
                    ungetc(caracter, arquivo);
                    token_base.categoria = INTCON;
                    token_base.valor_inteiro = atoi(digitos);
                    return token_base;
                }
                break;
            case 12:
                if(caracter >= '0' && caracter <= '9'){ 
                    estado = 13;
                    digitos[tam_digito] = caracter;
                    digitos[++tam_digito] = '\0'; 
                } 
                break;
            case 13:
                if(caracter >= '0' && caracter <= '9'){ 
                    estado = 13;
                    digitos[tam_digito] = caracter;
                    digitos[++tam_digito] = '\0'; 
                } else { //REALCON
                    estado = 14; 
                    ungetc(caracter, arquivo);
                    token_base.categoria = REALCON;
                    token_base.valor_real = atof(digitos);
                    return token_base;
                }
                break;
            case 15:
                if(caracter != '"'){
                    estado = 15;
                    lexema[tam_lexema] = caracter;
                    lexema[++tam_lexema] = '\0';
                } else {
                    estado = 16;
                    token_base.categoria = STRINGCON;
                    strcpy(token_base.lexema, lexema);
                    return token_base;
                }
                break;
            case 17: //comentario ou divisao
                //?????
                break;
            case 29:
                if(caracter == '|'){
                    estado = 30;
                    token_base.categoria = SNL;
                    token_base.codigo = OR_LOGICO;
                    return token_base;
                } else {
                    error("Erro no caracter após | => não identificado");
                }
                break;
            case 31: //negação ou diferente
                if(caracter == '='){
                    estado = 32;
                    token_base.categoria = SNL;
                    token_base.codigo = COMP_DIFERENTE;
                    return token_base;
                } else {
                    estado = 33;
                    ungetc(caracter, arquivo);
                    token_base.categoria = SNL;
                    token_base.codigo = NEGACAO;
                    return token_base;
                }
                break;
            case 34: //igualdade ou atribuicao
                if(caracter == '='){
                    estado = 35;
                    token_base.categoria = SNL;
                    token_base.codigo = COMP_IGUALDADE;
                    return token_base;
                } else{
                    estado = 36;
                    ungetc(caracter, arquivo); //devolver o outro* com ungetc
                    token_base.categoria = SNL;
                    token_base.codigo = ATRIBUICAO;
                    return token_base;
                }
                break;
            case 37: //maior igual ou maior que
                if(caracter == '='){
                    estado = 38;
                    token_base.categoria = SNL;
                    token_base.codigo = MAIOR_IGUAL;
                    return token_base;
                } else {
                    estado = 49;
                    ungetc(caracter, arquivo);
                    token_base.categoria = SNL;
                    token_base.codigo = MAIOR_QUE;
                    return token_base;
                }
                break;

            case 40: //menor igual ou menor que
                if(caracter == '='){
                    estado = 41;
                    token_base.categoria = SNL;
                    token_base.codigo = MENOR_IGUAL;
                    return token_base;
                } else{
                    estado = 42;
                    ungetc(caracter, arquivo);
                    token_base.categoria = SNL;
                    token_base.codigo = MENOR_QUE;
                    return token_base;
                }
                break;
            case 43: //endereco ou AND
                if(caracter == '&'){
                    estado = 44;
                    token_base.categoria = SNL;
                    token_base.codigo = AND_LOGICO;
                    return token_base;
                } else {
                    estado = 45;
                    ungetc(caracter, arquivo);
                    token_base.categoria = SNL;
                    token_base.codigo = ACESSO_END;
                    return token_base;
                }
                break;   
        }
    }
}

int main(){
    return 0;
}