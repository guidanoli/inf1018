#include <stdio.h>

typedef int (*funcp) (int x);
void gera_codigo (FILE *f, void **code, funcp *entry);
void libera_codigo (void *p);
