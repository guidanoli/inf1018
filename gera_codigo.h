/*
Aluno: Guilherme Dantas	Matrícula: 1713155
Aluno: Rafael Damazio		Matrícula: 1712990
*/

#include <stdio.h>

typedef int (*funcp) (int x);
void gera_codigo (FILE *f, void **code, funcp *entry);
void libera_codigo (void *p);
