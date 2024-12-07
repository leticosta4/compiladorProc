# compiladorProc
compilador feito em C para a linguagem Proc - 2024.2 Fundamentos de Compiladores

## gramática + outros p fazer
- [x] prog
- [x] decl_list_var
- [x] decl_def_prot
- [x] decl_var
- [x] tipo
- [x] prot
- [x] def
- [ ] cmd
    - [x] do
    - [x] while
    - [x] if, elif, else
    - [x] var
    - [x] gets
    - [x] puts
    - [x] getout
- [ ] atrib
- [ ] expr
- [ ] expr_simp
- [ ] termo
- [ ] fator
- [ ] op_rel
- [ ] limpar prints e comentarios
- [ ] tabela de simbolos
- [ ] talvez tirar a função `printa_valor_token()`
- [x] ajeitar dimensao array
- [x] ajeitar coisa do bool
- [ ] ends/cmds aninhados
- chamada em excesso do analex com as novas funcoes - aparentemente consumindo 2 vezes a mais


## dúvidas
- esse enum: `typedef enum{INT, REAL, CHAR, BOOL, NAO_APLICA}TIPO;` ta dando erro de redefinicao

## resolver/pensar sobre
- loop infinito com ENDs aninhados ????? wtf - n faço ideia de como resolver
- tirar stringcon
- bug nos contadores da linha aparentemente (so alguns casos)
- aplicar tabela de simbolos
- testar `op_rel()`
- chaves estranhas
- continuar implementação:
    - fator 
    - termo
    - expr_simp
    - expr
    - atrib