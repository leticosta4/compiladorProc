# "assembly" - geração de código, MP

## WHILE
while(\<expr>)
{ \<cmd>}
endw

    LABEL Lx
    [cod expr]
    GOFALSE Ly
    [cod cmd]
    GOTO Lx
    LABEL Ly

## IF - ELIF - ELSE 
if(\<expr1>)
{ \<cmd1>}
elif(\<expr2>)
{ \<cmd2>}
else
{ \<cmd3>}
endi

    [cod expr1]
    GOFALSE Lx
    [cod cmd1]
    GOTO La
    LABEL Lx
    [cod expr2]
    GOFALSE Ly
    [cod cmd2]
    GOTO La
    LABEL Ly
    [cod expr3]
    GOFALSE Lw
    [cod cmd3]
    GOTO La
    LABEL Lw
    [cod cmd4]
    LABEL La

## VAR
### com TO (expr 1 < expr 2)d
var a from \<expr1> to \<expr2> [by intcon | idconst] \<cmd>

    [cod expr1]
    STOR A
    [cod expr2]
    LABEL Lx
    COPY 
    LOAD A
    SUB            //expr2 - expr1
    GOTRUE Ly
    GOTO Lz       //deu zero, expressoes iguais 
    LABEL Ly
    [codigo cmd]
    LOAD A
    PUSH [intcon] | LOAD [idconst]
    ADD
    STOR A        //precisa?
    POP           //não esquecer se n dá lixo
    GOTO Lx       //para repetir o loop for
    LABEL Lz      //saindo e não executa o loop
    POP

### com DT (expr 1 > expr 2)
var a from \<expr1> dt \<expr2> [by intcon | idconst] \<cmd>

    [cod expr1]
    STOR A 
    LABEL Lx
    [cod expr2]  //depois do LABEL Lx p n perder o valor de exp2 e ver se é maior q exp1
    LOAD A
    SUB
    COPY
    GOTRUE Ly    //expressao falsa pq exp2 > exp1
    GOFALSE Lz   //se der 0 é falsa tb, sai logo
    [cod cmd]
    LOAD A 
    PUSH [intcon] | LOAD [idconst]
    SUB
    STOR A
    GOTO LX     //continuacao do loop
    LABEL Ly
    POP
    LABEL Lz


## OR lógico
? ( \<expr1> || \<expr2>)

    [cod expr1]
    COPY
    GOTRUE Lx
    POP
    [cod expr2]
    LABEL Lx

## AND lógico
? ( \<expr1> && \<expr2>)

    [cod expr1]
    COPY
    GOFALSE Ly
    POP
    [cod expr2]
    LABEL Ly

## NEGACAO
? (! \<expr1>)

    [cod expr1]
    GOTRUE Lz
    PUSH 1
    GOTO Lw
    LABEL Lz
    PUSH 0
    LABEL Lw

## IGUALDADE ==  e  DIFERENTE !=
A == B  e  A  != B

    LOAD A
    LOAD B
    SUB
    GOFALSE Ly //se for 0 a é igual a b > condição verdadeira
    PUSH 0     //se for maior q 0 a > b > condição falsa (0 no topo da pilha) 
    GOTO Lz
    LABEL Ly
    PUSH 1
    LABEL Lz

para <b>DIFERENTE</b>  é só trocar os `PUSH`es

## MENOR IGUAL 
? (a <= b) 

    LOAD A
    LOAD B
    SUB
    GOTRUE L1 //aq se der maior doq 0 a subtração a > b, condição falsa
    PUSH 1    //se for 0 ou menos > condição verdadeira (1 no topo da pilha)
    GOTO L2
    LABEL L1
    PUSH 0   // 0 no topo da pilha pq a condição é falsa
    LABEL L2
    ...      //resto do codigo p testar a condição

## MAIOR IGUAL 
? (a >= b) 

    LOAD A
    LOAD B
    SUB
    COPY
    GOTRUE L1   //se der maior doq 0 a subtração a > b, condição verdadeira
    GOFALSE L2  //se der 0 a subtração a == a, condição verdadeira
    PUSH 0
    GOTO L3
    LABEL L1
    POP      //pop na primeira p n ter push 1 e duplicado na pilha
    LABEL L2
    PUSH 1
    LABEL L3
    ...

## MAIOR QUE
? (a > b)

    LOAD A 
    LOAD B
    SUB 
    GOTRUE L1  //maior que 0 > condição verdadeira
    PUSH 0     //se for falsa zero no topo da pilha
    GOTO L2
    LABEL L1
    PUSH 1
    LABEL L2
    ...

## MENOR QUE
? (a < b)

    LOAD A 
    LOAD B
    SUB
    COPY
    GOTRUE L1   //se for maior que 0, a > b, condição falsa
    GOFALSE L2  //se 0, a == b, condição falsa
    PUSH 1      //nenhum desses 2 casos, condição verdadeira 
    GOTO L3
    LABEL L1
    POP
    LABEL L2
    PUSH 0
    LABEL L3
    ...

## PONTAS DO PROGRAMA

    INIP
    AMEM [qtdVarGlobais]
    ...
    DMEM [qtdVarGlobais]
    HALT

## PONTAS DO PROCEDIMENTO
    
    LABEL Lx
    INIPR 1
    AMEM [qtdVarLocais]
    ...
    DMEM [qtdVarLocais]
    RET 1, [qtdParametros]

## informações gerais importantes
- GOTO é específico para LABEL, mas o CALL Lx para procedimento
- parametros de procedimento possuem endereçõ negativo, sendo que o ultimo deles é o -3

## exemplo A = B + 1

    LOAD B
    PUSH 1
    ADD
    STOR A

## exemplo:
<b>while ( <span style="color: red;">(a <= b)</span>  <span style="color: yellow;">&&</span> ( <span style="color: blue;">(c > d)</span>  <span style="color: orange;">||</span>  <span style="color: green;">(e == f)</span> )) {
    
    
<span style="color: cyan;">a = a + c;</span>

<span style="color: pink;">c = a - b + 2;</span>

<span style="color: magenta;">e = d / 5 * c;</span>

}</b>

<i>INIP</i>

LABEL L1

<span style="color: red;">
LOAD A<br>LOAD B<br>SUB<br>GOTRUE L2<BR>PUSH 1<BR>GOTO L3<BR>LABEL L2<BR>PUSH 0
</span>

<span style="color: yellow;">
LABEL L3<BR>COPY<BR>GOFALSE L4 
</span>
<span style="color: gray;">//como é um and, se a primeira pt ja for falsa toda a expr é falsa > sai logo</span>
<BR><span style="color: yellow;"> POP </span>

<span style="color: blue;">
LOAD C<br>LOAD D<br>SUB<BR>GOTRUE L5<br>PUSH 0<BR>GOTO L6<BR>LABEL L5<BR>PUSH 1<BR>
</span>

<span style="color: orange;">
LABEL L6<BR>COPY<BR>GOTRUE L7
</span>
<span style="color: gray;">//como é um or, se a primeira pt ja for verdadeira, ja vale p execução</span>
<BR><span style="color: orange;"> POP </span>


<span style="color: green;">
LOAD E<BR>LOAD 7<BR>SUB<BR>GOFALSE L8<BR>PUSH 0<BR>GOTO L9<BR>LABEL L8<BR>PUSH 1
</span>

<br><span style="color: GREEN;">LABEL L9</span>        
<span style="color: orange;">LABEL L7</span>        <span style="color: gray;">//fim da segunda pt da expressão (t: ||)</span>
<br><span style="color: yellow;">LABEL L4</span>        <span style="color: gray;">//fim da expressão (t: &&)</span>
<br>GOFALSE L10      <span style="color: gray;"> //expressao falsa > não executa bloco de comando e vai p o fim do while </span>
    
<span style="color: cyan;">
LOAD A<br>LOAD C<br>ADD<br>STOR A
</span>


<span style="color: pink;">
LOAD A<br>LOAD B<br>SUB<br>PUSH 2<br>ADD<br>STOR C
</span>


<span style="color: magenta;">
LOAD D<br>PUSH 5<br>DIV<br>LOAD C<br>MULT<br>STOR E
</span>

GOTO L1  //continuação do loop while
<br>LABEL L10 //fim do while

<i>HALT</i>