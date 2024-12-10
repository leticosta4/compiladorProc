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
- [ ] tirar obrigatoriedade de parametro em def em `anasint.c`


## tabela de símbolos
- [ ] iniciar tabela - base ok
- [ ] inserir tabela - base ok
- [ ] buscar redeclaração (var_local || var_global || procedimento || prototipo) - base ok
- [ ] buscar declaracao (constante || prototipo > p o procedimento || variavel > analisar)
- [ ] limpar/resetar o info_token p n acumular a cada insercao
- [ ] remover tabela - base ok
- [ ] printar tabela - base ok
- [x] questão lá do valor da constante: `union`, `enum`, `char`... > e ver como fazer p printar
- [ ] chamadas no `anasint.c`
- [ ] resetar o `info_token` antes des ser preenchido com as infos do prox token a ser processadp no `anasint.c`
- [ ] tratar dimensoes dos arrays como vetor - base ok
- [ ] verificacoes de repeticao de prototipo e procedimento em `def()` e `prot()` de `anasint.c` - base ok
- [x] colocar opcoes que nao aplica no inicio dos enums ? - bom p zerar tabela
- [ ] a bagunca da `verifica_declaracao()`
- [ ] ver melhor onde usar `confere_atrib_constante()`


## outros
- [ ] limpar prints e comentarios
- [ ] tabela de simbolos
- [ ] talvez tirar a função `printa_valor_token()`
- [x] ajeitar dimensao array
- [x] ajeitar coisa do bool
- [x] limitar dimensao de array no parametro com def e prot
- [ ] no final apagar a funcao debug

## dúvidas
- por enquanto dboa, só confirmar stringcon
- a coisa la do valor da constante p a tabela de simbolos
- verificacao da variavel em atrib? como lidar?

## resolver/pensar sobre
- tirar stringcon
- aplicar tabela de simbolos
- tratamentos chatos