# "assembly" - geração de código, MP

## WHILE
while(\<expr>)
{ \<cmd>}
endw

    LABEL *Lx*
    [cod expr]
    GOFALSE *Ly*
    [cod cmd]
    GOTO *Lx*
    LABEL *Ly*

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
    ... continuar

## OR lógico
? ( \<expr1> || \<expr2>)

    [cod expr1]
    COPY
    GOTRUE *Lx*
    POP
    [cod expr2]
    LABEL *Lx*

## AND lógico
? ( \<expr1> && \<expr2>)

    [cod expr1]
    COPY
    GOFALSE *Ly*
    POP
    [cod expr2]
    LABEL *Ly*

## NEGACAO
? (! \<expr1>)

    [cod expr1]
    GOTRUE *Lz*
    PUSH 1
    GOTO *Lw*
    LABEL *Lz*
    PUSH 0
    LABEL *Lw*
