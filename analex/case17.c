#include <stdio.h>
#include <stdlib.h>

int main(){
    int estado = 0;
    char caracter;

    switch(estado){
        case 17:
            if(caracter == '/'){ //inicio de comentario
                estado = 19;
                //[...]
            } else { //DIVISAO
                estado = 18;
                // ungetc(caracter, arquivo);
                // token_base.categoria = SNL;
                // token_base.codigo = DIVISAO;
                // return token_base;
            }
            break;
        case 19:
            if(caracter != '\n'){
                //???
            } else {
                estado = 0;
                //????
            }
    }
    return 0;
}