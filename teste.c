/*
   Módulo de testes
*/

#include <stdio.h>
#include "grava_structs.h"

typedef struct a{
	int i;
	long l;
	short s;
	char c;
	short s2;
} structs;

int main(void)
{
   structs a[3]={{1,90,4,14,34},{2,91,5,15,35},{3,93,6,16,36}};
   int retorno = grava_structs(3,a,"ilscs",'B',"relatorio.txt");
   
   return 0;
}
