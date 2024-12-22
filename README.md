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
- verificacao da variavel em atrib? como lidar?


## analise semantica    
### bugs
- [x] endereço reiniciando nos parametros
- [x] nomes de procedimentos sendo reescritos
- [x] tabela resetando
- [x] coisa de array com {}

### regras
- [ ] buscar REDECLARACAO
    - [x] prototipo
    - [x] procedimento
    - [ ] constante
    - [x] variavel global
    - [ ] variavel local - dentro do mesmo procedimento
    - [x] parametro - dentro do mesmo procedimento
- [ ] verificação do tipo da veriavel com o correspondente da declaração 
    - [ ] inteiro com ele mesmo e char, e vice-versa
    - [ ] bool com int
- [x] variaveis locais e globais
- [ ] buscar DECLARACAO 
    - [ ] constante
    - [x] prototipo > p o procedimento
    - [ ] variavel > analisar
    - [ ] var local no procedimento e parametro daquele procedimento
    - [ ] passar o id dentro do tamanho do array - ver como lidar 
- [ ] ele nao vendo quando é constante p passar no tamanho do array
- [ ] declaração de var globais antes de procedimento
- [ ] declaração de var locais antes de comandos
- [x] quando chamar o do verificar se o procedimento tem um prototipo (é obrigatorio)
- [x] não pode deixar definição de procedimento interno
- [x] escopo local para os parametros de um procedimento
- [x] variavel sem ser array n pode ser inicializada com {}
- [ ] se n tiver o init ele deve ser só COMPILADO mas n EXECUTADO (wtf?)
- [x] quando o procedimento nao tiver prototipo, a declaração dele deve ser antes do init
- [ ] parametros compatíveis no procedimento
    - [ ] em tipo
    - [ ] em quantidade - refinar
    - [ ] em ordem
- [x] não pode ter & na chama do procedimento com o do
- [ ] tipo boolean para a expr condicional de if, elif e while
- [ ] var
    - [ ] usou TO: expr1 < expr2
    - [ ] usou DT: expr1 > expr2
- [ ] basically td de expr...
- [ ] mais coisas que envolvem boooleans e resultados