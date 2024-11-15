#include <stdio.h> 
#include <stdlib.h> 
#include <ctype.h> 
#include <string.h> 
#include <stdbool.h> 

#include "analex.h" 
#include "funcaux.h"

#define TAM_LEXEMA 50 
#define TAM_NUM 20 
#define NUM_PLV_RSVD 28

TOKEN AnaLex(FILE *arquivo){
    int estado; 
    char lexema[TAM_LEXEMA] = ""; 
    int tam_lexema = 0; 
    char digitos[TAM_NUM] = ""; 
    int tam_digito = 0; 
    int codigo_enum_pal_rsvd = 0;
    TOKEN token_base; 

    estado = 0; 

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
                } else if(caracter == '+'){ //nesses assim sem o outro* já monta e retorna o token
                    estado = 20;
                    token_base.categoria = SNL;
                    token_base.codigo = ADICAO;
                    return token_base;
                } else if(caracter == '-'){ 
                    estado = 21;
                    token_base.categoria = SNL;
                    token_base.codigo = SUBTRACAO;
                    return token_base;
                } else if(caracter == '*'){ 
                    estado = 22;
                    token_base.categoria = SNL;
                    token_base.codigo = MULTIPLICACAO;
                    return token_base;
                } else if(caracter == ','){ 
                    estado = 23;
                    token_base.categoria = SNL;
                    token_base.codigo = VIRGULA;
                    return token_base;
                } else if(caracter == '('){ 
                    estado = 24;
                    token_base.categoria = SNL;
                    token_base.codigo = ABRE_PAREN;
                    return token_base;
                } else if(caracter == ')'){ 
                    estado = 25;
                    token_base.categoria = SNL;
                    token_base.codigo = FECHA_PAREN;
                    return token_base;
                } else if(caracter == '['){ 
                    estado = 26;
                    token_base.categoria = SNL;
                    token_base.codigo = ABRE_COL;
                    return token_base;
                } else if(caracter == ']'){ 
                    estado = 27;
                    token_base.categoria = SNL;
                    token_base.codigo = FECHA_COL;
                    return token_base;
                } 
                //esses abaixo ainda dependem do proximo caracter que se segue => tratado em outros cases (outros estados)
                else if(caracter == '/'){ estado = 17; } //inicio de COMENTARIO OU DIVISAO
                else if(caracter == '|'){ estado = 28; }
                else if(caracter == '!'){ estado = 30; }
                else if(caracter == '='){ estado = 33; }
                else if(caracter == '>'){ estado = 36; }
                else if(caracter == '<'){ estado = 39; }
                else if(caracter == '&'){ estado = 42; }
                else if(caracter == '\n'){
                    estado = 0;
                    token_base.categoria = FINAL_EXP;
                    contLinha++;
                    return token_base;
                } else if(caracter == EOF){
                    token_base.categoria = FINAL_ARQ;
                    return token_base;
                } else {
                    error("Caracter invalido");
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
                    codigo_enum_pal_rsvd = verifica_plv_rsvd(lexema);
                    if(codigo_enum_pal_rsvd != -1){
                        token_base.categoria = PLV_RSVD;
                        token_base.codigo = codigo_enum_pal_rsvd;
                    } else {
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
                } else if(caracter == '\''){ error("Erro no charcon"); } 
                else if(isprint(caracter)){
                    estado = 5;
                    lexema[tam_lexema] = caracter;
                    lexema[++tam_lexema] = '\0';
                } else{ error("Erro no charcon"); }
                break;
            case 5:
                if(caracter == '\''){ //inicio de CHARCON
                    token_base.categoria = CHARCON;
                    token_base.c = lexema[--tam_lexema];
                    return token_base;
                } else{ error("Erro no charcon"); }
                break;
            case 6: //n ou 0 => quebra de linha ou espaço vazio
                if(caracter == 'n'){
                    estado = 8;
                } else if(caracter == '0'){
                    estado = 7;
                } else {
                    error("erro no charcon");
                }
            case 7:
                if(caracter == '\''){ 
                    token_base.categoria = CHARCON;
                    token_base.c = 0;
                    return token_base;
                } 
                break;
            case 8:
                if(caracter == '\''){ 
                    token_base.categoria = CHARCON;
                    token_base.c = 10; 
                    return token_base;
                } 
                break;
            case 10:
                if(caracter >= '0' && caracter <= '9'){ 
                    estado = 10;
                    digitos[tam_digito] = caracter;
                    digitos[++tam_digito] = '\0'; 
                    if(tam_digito > TAM_NUM){ error("numero maximo para intcon atingido\n"); }
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
                    if(tam_digito > TAM_NUM){ error("numero maximo para realcon atingido"); }
                } 
                break;
            case 13:
                if(caracter >= '0' && caracter <= '9'){ 
                    estado = 13;
                    digitos[tam_digito] = caracter;
                    digitos[++tam_digito] = '\0'; 
                    if(tam_digito > TAM_NUM){ error("numero maximo para realcon atingido"); }
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
                    if(tam_lexema > TAM_LEXEMA){ error("stringcon inválida"); }
                } else {
                    estado = 16;
                    token_base.categoria = STRINGCON;
                    strcpy(token_base.lexema, lexema);
                    return token_base;
                }
                break;
            case 17: //comentario ou divisao
                if(caracter == '/'){ //inicio de comentario - FILTRO
                    estado = 19;
                } else{
                    estado = 18;
                    ungetc(caracter, arquivo);
                    token_base.categoria = SNL;
                    token_base.codigo = DIVISAO;
                    return token_base;
                }
                break;
            case 19:
                if(caracter == '\n' || caracter == EOF){
                    estado = 0;
                    contLinha++;
                }
                break; 
            case 28:
                if(caracter == '|'){
                    estado = 29;
                    token_base.categoria = SNL;
                    token_base.codigo = OR_LOGICO;
                    return token_base;
                } else {
                    error("Erro no caracter após | => não identificado");
                }
                break;
            case 30: //negação ou diferente
                if(caracter == '='){
                    estado = 31;
                    token_base.categoria = SNL;
                    token_base.codigo = COMP_DIFERENTE;
                    return token_base;
                } else {
                    estado = 32;
                    ungetc(caracter, arquivo);
                    token_base.categoria = SNL;
                    token_base.codigo = NEGACAO;
                    return token_base;
                }
                break;
            case 33: //igualdade ou atribuicao
                if(caracter == '='){
                    estado = 34;
                    token_base.categoria = SNL;
                    token_base.codigo = COMP_IGUALDADE;
                    return token_base;
                } else{
                    estado = 35;
                    ungetc(caracter, arquivo); //devolver o outro* com ungetc
                    token_base.categoria = SNL;
                    token_base.codigo = ATRIBUICAO;
                    return token_base;
                }
                break;
            case 36: //maior igual ou maior que
                if(caracter == '='){
                    estado = 37;
                    token_base.categoria = SNL;
                    token_base.codigo = MAIOR_IGUAL;
                    return token_base;
                } else {
                    estado = 38;
                    ungetc(caracter, arquivo);
                    token_base.categoria = SNL;
                    token_base.codigo = MAIOR_QUE;
                    return token_base;
                }
                break;

            case 39: //menor igual ou menor que
                if(caracter == '='){
                    estado = 40;
                    token_base.categoria = SNL;
                    token_base.codigo = MENOR_IGUAL;
                    return token_base;
                } else{
                    estado = 41;
                    ungetc(caracter, arquivo);
                    token_base.categoria = SNL;
                    token_base.codigo = MENOR_QUE;
                    return token_base;
                }
                break;
            case 42: //endereco ou AND
                if(caracter == '&'){
                    estado = 43;
                    token_base.categoria = SNL;
                    token_base.codigo = AND_LOGICO;
                    return token_base;
                } else {
                    estado = 44;
                    ungetc(caracter, arquivo);
                    token_base.categoria = SNL;
                    token_base.codigo = ACESSO_END;
                    return token_base;
                }
                break; 
        }
    }
}