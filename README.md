# compiladorProc
compilador feito em C para a linguagem Proc - 2024.2 Fundamentos de Compiladores
- [x] analisador léxico
- [x] analisador sintático
- [x] tabela de símbolos 
- [ ] integração semântica - WIP
- [ ] geraçao de código
- [x] bugfix:
    - [x] anasint
    - [x] tabsimb
- exemplo de <b>fragilidade</b>:
    
    se colocar um procedimento novo embaixo do procedimento soma no arquivo `teste3.proc` ele buga a tabela de simbolos e passa a deixar var_local ainda

## analise sintatica - gramática
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
- [x] erro em do > fator ? (investigar linha 80 do novo arquivo)


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
- [x] ver melhor onde usar `confere_atrib_constante()`
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
- [x] alterar como eu trato prototipo de procedimento e os seus parametros


## outros
- [ ] limpar prints
- [ ] limpei os comentarios
- [x] tabela de simbolos
- [x] talvez tirar a função `printa_valor_token()`
- [x] ajeitar dimensao array
- [x] ajeitar coisa do bool
- [x] limitar dimensao de array no parametro com def e prot
- [x] no final apagar a funcao debug
- [ ] tratamentos chatos

## dúvidas

## analise semantica    
### bugs
- [x] endereço reiniciando nos parametros
- [x] nomes de procedimentos sendo reescritos
- [x] tabela resetando
- [x] coisa de array com {}
- [x] variavel escalar/vetor n sendo reconhecido mesmo sendo igual => ta dando incopatibilidade EM DEF
- [x] refinar a busca da declaracao de identificador
- [x] bug `while(!(b > c))` : para no c e não reconhece (bug do sintatico) > `else if NEGACAO` do `case SNL` em `fator()`
- [ ] aql ngcio la do contlinha
- [x] manipulacao de var locais na tabela de simbolos > gambiarra
    - [x] em `procura_existencia_identificador_em_proced` : reconhecimento identificadores
    - [x] em `apagar_var_locais` : deixava smp a primeira var local declarada
- [x] quantidade de parametros na chamada do do
- [ ] camuflar bug da primeira variavel no print da tabela

### regras
- [x] buscar REDECLARACAO
    - [x] prototipo
    - [x] procedimento
    - [x] variavel global
    - [x] variavel local - dentro do mesmo procedimento
    - [x] parametro - dentro do mesmo procedimento
- [x] constante
    - [x] redeclaração
    - [x] deve ser inicializada
    - [x] passar no array e atribuir o valor da const na dimensao
    - [x] compatibilidade tipo
- [x] não pode prototipo sem procedimento
- [x] verificação do tipo da veriavel com o correspondente na DECLARAÇÃO 
    - [x] inteiro com ele mesmo e char, e vice-versa
    - [x] bool com int e char
    - [x] resto com resto
- [x] verificação do tipo da variavel com o correspondente na ATRIBUIÇÃO 
- [x] variaveis locais e globais
- [x] buscar DECLARACAO 
    - [x] constante > quando usada na dimensao do array
    - [x] prototipo > p o procedimento
    - [x] de identificador ao ser usado em expr
- [x] ele nao vendo quando é constante p passar no tamanho do array ?
- [x] declaração de var globais antes de procedimento
- [x] declaração de var locais antes de comandos
- [x] quando chamar o do verificar se o procedimento tem um prototipo (é obrigatorio)
- [x] não pode deixar definição de procedimento interno
- [x] escopo local para os parametros de um procedimento
- [x] variavel sem ser array n pode ser inicializada com {}
- [x] quando o procedimento nao tiver prototipo, a declaração dele deve ser antes do init
- [x] possivel funcao de associaçao de compatibilidade dos tipos int, char e bool
    - [x] p atrib
    - [x] adaptar p condicional em expr
- [x] parametros compatíveis no procedimento
    - [x] na definicao quando ele tem prototipo
        - [x] em tipo 
        - [x] em quantidade 
        - [x] em ordem
        - [x] array ou var escalar
    - [x] na chamada com o do quando ele nao tem prototipo 
        - [x] em tipo
        - [x] em quantidade 
        - [x] em ordem
        - [x] array ou var escalar
- [x] não pode ter & na chama do procedimento com o do
- [x] expr 
    - [x] se for um id, o tipo dessa expr é o tipo do id 
    - [x] se for uma constante int, o tipo dessa expr é o tipo do id 
    - [x] nao pode comparação de variaveis entre tipos nao compativeis
    - [x] se expr tiver 2 lados e ops relacionais ou logicos o tipo é bool 
    - [x] se expr é aritmetica o tipo dela é o resultado da operação
    - [x] uma expressao entre colchetes deve ser do tipo int
    - [x] tipo boolean para a expr condicional de if, elif e while
- [ ] se n tiver o init ele deve ser só COMPILADO mas n EXECUTADO (?)
- [ ] var (DEPENDE DA MAQ DE PILHA)  >> 291 @ `anasiint.c`
    - [ ] usou TO: expr1 < expr2
    - [ ] usou DT: expr1 > expr2

## geração de codigo - MP
- [x] INIP e HALT
- [ ] INIPR 1 E RET 1, <qtdParams>
    - [x] randomly
    - [ ] proper way
- [ ] AMEM E DMEM
    - [x] global
    - [ ] local (começa a dar problema nos parametros de procedimento wtf?)
- [ ] enderecos corretamente (me fudi)
- [ ] while
- [ ] if
- [ ] elif
- [ ] else
- [ ] var
- [ ] comp igualdade
- [ ] comp diferença
- [ ] maior que
- [ ] menor que
- [ ] and logico
- [ ] or logico