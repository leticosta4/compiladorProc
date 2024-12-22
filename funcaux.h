#ifndef FUNC_AUX
#define FUNC_AUX

#include <stdlib.h> 
#include <stdbool.h> 

extern int contLinha; //extern para garantir que a mesma variavel global sendo usada em arquivos diferentes

void error(char msg[]);
int verifica_plv_rsvd(const char lexema[]);
void consome_fim_exp();
void debug(char onde[]);
void busca_erro_decl_var_dps_decl_prot_proc_ou_cmd(TOKEN sus_token, int escopo_var);
#endif