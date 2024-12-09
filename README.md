# compiladorProc
compilador feito em C para a linguagem Proc - 2024.2 Fundamentos de Compiladores
- [x] analisador léxico
- [x] analisador sintático
- [ ] tabela de símbolos
- [ ] integração semântica
- [ ] geeraçao de código

<b>ANALISADOR LEXICO > TA FUNCIOONANDO PARA TODOS OS ARQUIVOSS</b>

## gramática
- [x] prog
- [x] decl_list_var
- [x] decl_def_prot
- [x] decl_var
- [x] tipo
- [x] prot
- [x] def
- [x] cmd
    - [x] do
    - [x] while
    - [x] if, elif, else
    - [x] var
    - [x] gets
    - [x] puts
    - [x] getout
- [x] atrib
- [x] expr
- [x] expr_simp
- [x] termo
- [x] fator
- [x] op_rel


## tabela de símbolos
- [ ] iniciar tabela - base ok
- [ ] inserir tabela - base ok
- [ ] buscar redeclaração tabela - base ok
- [ ] buscar posicao tabela
- [ ] remover tabela - base ok
- [ ] printar tabela - base ok
- [ ] questão lá do valor da constante: union, enum, char... > e ver como fazer p printar
- [ ] chamadas no anasint.c


## outros
- [ ] limpar prints e comentarios
- [ ] tabela de simbolos
- [ ] talvez tirar a função `printa_valor_token()`
- [x] ajeitar dimensao array
- [x] ajeitar coisa do bool
- [x] limitar dimensao de array no parametro com def e prot

## dúvidas
- por enquanto dboa, só confirmar como tirar stringcon
- a coisa la do valor da constante p a tabela de simbolos

## resolver/pensar sobre
- tirar stringcon
- aplicar tabela de simbolos
- tratamentos chatos