#include <stdio.h> 
#include <stdlib.h> 
#include <ctype.h> 
#include <string.h> 
#include <stdbool.h> 

#include "analex.h" 

#define TAM_LEXEMA 50 
#define TAM_NUM 20 

void error(char msg[]){ 
    printf("%s na linha %d\n", msg, contLinha); 
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
                //esses abaixo ainda dependem do proxio caracter que se segue => tratado em outros cases (outros estados)
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
                } else if(caracter == '\''){ error("Erro no charcon"); } 
                else if(isprint(caracter)){
                    estado = 5;
                    lexema[tam_lexema] = caracter;
                    lexema[++tam_lexema] = '\0';
                } else{ error("Erro no charcon"); }
                break;
            case 5:
                if(caracter == '\''){ //inicio de CHARCON
                    estado = 9;
                }
                break;
            case 6: //n ou 0 => quebra de linha ou espaço vazio
                if(caracter == 'n'){
                    estado = 7;
                    lexema[tam_lexema] = 10;//caracter;
                    lexema[++tam_lexema] = '\0';
                } else if(caracter == '0'){
                    estado = 8;
                    lexema[tam_lexema] = 0;//caracter;
                    lexema[++tam_lexema] = '\0';
                } else {
                    error("erro no charcon");
                }
            case 7:
                if(caracter == '\''){ 
                    estado = 9; //tem que mudar para 45
                }
                break;
            case 8:
                if(caracter == '\''){ 
                    estado = 9; //tem que mudar para 46
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
                if(caracter == '\n' || caracter == EOF){ estado = 0; }
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
            // case 45: //CHARCON - \n RETORNAR O VALOR 13
            //     token_base.categoria = CHARCON;
            //     token_base.c = lexema[--tam_lexema];
            //     return token_base;
            //     break;
            // case 46: //CHARCON - \0 RETORNAR O VALOR 0
            //     token_base.categoria = CHARCON;
            //     token_base.c = lexema[--tam_lexema];
            //     return token_base;
            //     break;  
        }
    }
}

int main(){
    FILE *arqivoProc;
    TOKEN rcv_token;

    //if de abertura do arquivo com erro ou nao 
    if ((arqivoProc = fopen("testeProc.txt", "r")) == NULL){ printf("Arquivo de entrada da expressao nao encontrado!"); }

    printf("\nLINHA: %d\n\n", contLinha);

    //fazer um ngcio de processador de tokens com print f quando receber o token
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

    return 0;
}