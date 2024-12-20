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
    GOFALSE Ly
    PUSH 0
    GOTO Lz
    LABEL Ly
    PUSH 1
    LABEL Lz

para <b>DIFERENTE</b>  é só trocar os `PUSH`es

