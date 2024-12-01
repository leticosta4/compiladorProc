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
    - [ ] if
    - [ ] var
    - [ ] gets
    - [ ] puts
    - [] getout
- [ ] atrib
- [ ] expr
- [ ] expr_simp
- [ ] termo
- [ ] fator
- [ ] op_rel
- [ ] limpar prints e comentarios
- [ ] tabela de simbolos
- [ ] talvez tirar a função `printa_valor_token()`


## dúvidas
- esse enum: `typedef enum{INT, REAL, CHAR, BOOL, NAO_APLICA}TIPO;` ta dando erro de redefinicao
- acho que depois posso tirar todas as linhas com `rcv_token.processado = true;` pq meio que to controlando onde chamr analex
- precisa salvar a palavra reservada `init` na tabela de simbolos?

## resolver/pensar sobre
- <b>talvez</b> aquilo do lexema na analise sintatica
- <b>talvez</b> aquilo do final de expressao nao estar pulando linha
- bug nos contadores da linha aparentemente
- testar os if/elif/else
- testar os puts e os gets
### bom para debug

    printf("\n\nACABOU ENTRAR: cat: %d | codigo: %d\n\n", rcv_token.categoria, rcv_token.codigo);