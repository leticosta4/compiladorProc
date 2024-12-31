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
elif(\<expr3>)
{ \<cmd3>}
else
{ \<cmd4>}
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
    LAOD B
    SUB
    COPY
    GOTRUE L1   //se for maior que 0, a > b, condição falsa
    GOFALSE L2  //se 0, a == b, condição falsa
    PUSH 1      //nenhum desses 2 casos, condição verdadeira 
    GOTO L2
    LABEL L1
    POP
    LABEL L2
    PUSH 0
    LABEL 3
    ...

## exemplo A = B + 1

    LOAD B
    PUSH 1
    ADD
    STOR A

## exemplo:
<b>while((a <= b) && ((c > d) || (e == f))){
    
a = a + c;
c = a - b + 2;
e = d / 5 * c;

}</b>
<font color=\"red\">Texto com cor vermelha</font>