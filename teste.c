/*
   Módulo de testes
*/

#include <stdio.h>
#include "grava_structs.h"

#define FNAME "testes.txt"

typedef struct teste_1{
	int i1;
	short s1;
	long l1;
	char c1;
} struct1;

typedef struct teste_2{
	int i1;
	int i2;
	long l1;
	long l2;
} struct2;

typedef struct teste_3{
	int i1;
	short s1;
	long l1;
	char c1;
	int i2;
	int i3;
	long l2;
	long l3;
} struct3;

typedef struct teste_4{
	char c1;
	char c2;
	char c3;
	char c4;
	int i1;
} struct4;

typedef struct teste_5{
	int i;
	short s;
	long l;
	char c;
} struct5;

int main(void)
{
   struct1 t1[3] = {{2,6,4,6},{2,5,7,8},{1,5,8,9}};
   struct2 t2[5] = {{2,7,8,9},{2,5,6,8},{8,6,5,3},{6,8,2,4},{8,7,6,5}};
   struct3 t3[2] = {{1,2,3,4,5,6,7,8},{8,7,6,5,4,3,2,1}};
   struct4 t4[4] = {{7,7,6,6,4},{2,7,5,2,5},{4,7,8,6,1},{2,2,3,8,4}};
   struct5 t5[3] = {{1,90,4,1},{2,21,5,15},{3,93,6,3}};
   
   int retorno;
   
   retorno = grava_structs(3,t1,"islc",'L',FNAME);
   dump_structs(FNAME);
   
   retorno = grava_structs(5,t2,"iill",'L',FNAME);
   dump_structs(FNAME);
   
   retorno = grava_structs(2,t3,"islciill",'B',FNAME);
   dump_structs(FNAME);
   
   retorno = grava_structs(4,t4,"cccci",'B',FNAME);
   dump_structs(FNAME);
   
   retorno = grava_structs(3,t5,"islc",'L',FNAME);
   dump_structs(FNAME);
   
   return 0;
}

