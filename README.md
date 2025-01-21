# compiladorProc
compilador feito em C para a linguagem Proc - 2024.2 Fundamentos de Compiladores
- [x] analisador léxico
- [x] analisador sintático
- [x] tabela de símbolos 
- [x] integração semântica 
- [x] geraçao de código

## instrucoes

colocar o nome do arquivo desejado (estão na pasta `files`) para leitura pelo compilador nas segundas linhas de cada uma das funções abaixo (arquivo `proc.c`):

- testaAnalex()
- testaAnasint()

comando para compilar o projeto e rodar o executavel:

    gcc proc.c analex.c anasint.c funcaux.c tabsimb.c -o proclang && ./proclang

## observações:
- já que a minha máquina de pilha não lida com alocação de vetores e matrizes, ao usar uma string (vetor de char), junto com 'getstr' ou 'putstr' é colocado O GET_C UO PUT_C como equivalente

- não localizei comandos equivalentes ao PUSH para valores reais ou literais, ent para o primeiro tipo estou usando PUSHF e para o segundo, assim como para stringcon, usando PUSH mesmo (já que int e char são compatíveis)

- quando é declarado um vetor ou matriz com const, o valor dessa constante não é adicionado na tabela de símbolos


## ficou faltando:
- gerar código de máquina de pilha para manipulação de váriaveis com passagem por referência (LDA, LOADI, STORI), e para vetores e matrizes (LDDLC, LDSTK, STSTK)
- fazer a comparação entre expr1 e expr2 em var para decidir qual cláusula (to ou dt) é a correta - **_não dá para fazer com o quanto foi aprendido na matéria_**