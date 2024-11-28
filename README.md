# compiladorProc
compilador feito em C para a linguagem Proc - 2024.2 Fundamentos de Compiladores


- VER COMO VAI APLICAR MATRIZ OU VETOR EM DECL_VAR 
- esse enum: `typedef enum{INT, REAL, CHAR, BOOL, NAO_APLICA}TIPO;` ta dando erro de redefinicao
- no caso de funcao ou procedimento, ja consumo o token em `prog` ou so em `decl_def_proc`, ja que preciso fazer essa verificação la em baixo tb?
- resolver
    - `int oi = 2, tchau`
    - `int oi[3] = {2, 4, 5}` (so pega ate o array)
