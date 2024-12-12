# compiladorProc
compilador feito em C para a linguagem Proc - 2024.2 Fundamentos de Compiladores
- [x] analisador léxico
- [x] analisador sintático
- [x] tabela de símbolos 
- [ ] integração semântica ~
- [ ] geraçao de código

<b>ANALISADOR LEXICO + SINTATICO > TA FUNCIOONANDO PARA TODOS OS ARQUIVOSS</b>

quando coloca identificador no tamanho do array - nao vai pq por enquanto só ta um numero - ATTA ?

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
- [x] tirar obrigatoriedade de parametro em def em `anasint.c`
- [x] ajustes no stringcon


## tabela de símbolos
- [x] iniciar tabela
- [x] inserir tabela
- [x] buscar redeclaração (var_local || var_global || procedimento || prototipo) 
- [x] limpar/resetar o info_token p n acumular a cada insercao
- [x] remover tabela 
- [x] printar tabela 
- [x] questão lá do valor da constante: `union`, `enum`, `char`... > 
- [x] chamadas no `anasint.c`
- [x] resetar as dimensoes de array do `info_token` 
- [x] tratar dimensoes dos arrays como vetor 
- [x] colocar opcoes que nao aplica no inicio dos enums ? - bom p zerar tabela
- [ ] ver melhor onde usar `confere_atrib_constante()`
- [x] quando tem virgula ele nao guarda atribuicoes do `info_token` - <b>IMPORTANTE</b>
- [x] printar o tipo certo do valor da constante
- [x] inserir nos parametros de procedimento e de prototipo - <b>IMPORTANTE</b>
    - [x] pegar e jogar na tabela
    - [x] pegar os dois dps da virgula
- [x] bug da variavel vir como parametro em vez de var_local
- [x] deixar os parametros de def vivos
- [x] transformar os parametros de def em zumbis
- [x] parametros: se o primeiro for `id_vetor` o outro pega tb
- [x] ver quando aplicar o var_simples
- [x] o valor de array nos parametros de procedimento tao continuando para os que nao sao
- [x] o remover_var_locais ainda deixa uma na tabela (aa primeira normalmente)


## outros
- [ ] limpar prints e comentarios
- [x] tabela de simbolos
- [ ] talvez tirar a função `printa_valor_token()`
- [x] ajeitar dimensao array
- [x] ajeitar coisa do bool
- [x] limitar dimensao de array no parametro com def e prot
- [x] no final apagar a funcao debug
- [ ] tratamentos chatos

## dúvidas
- a coisa la do valor da constante p a tabela de simbolos
- verificacao da variavel em atrib? como lidar?


## prox etapa > semantica
- [ ] verificacoes de repeticao de prototipo e procedimento em `def()` e `prot()` de `anasint.c` - base ok
- [ ] buscar declaracao (constante || prototipo > p o procedimento || variavel > analisar)
- [ ] ele nao vendo quando é constante p passar no tamanho do array
- [ ] tratar identificador no tamanho do array