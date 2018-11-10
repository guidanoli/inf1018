/*
Aluno: Guilherme Dantas	Matrícula: 1713155
Aluno: Rafael Damazio		Matrícula: 1712990
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "gera_codigo.h"

static void error (const char *msg, int line) {
  fprintf(stderr, "erro %s na linha %d\n", msg, line);
  exit(EXIT_FAILURE);
}

unsigned char cod_function[] = {0x55,0x48,0x89,0xe5,0x48,0x83,0xec,0x20,0x4c,0x89,0x55,0xe0};	//Inicia a pilha, e salva %r10 em -32(%rbp) SE HOUVER CHAMADA DE OUTRA FUNÇÃO, UTILIZAR COD_MOV_PARM_PILHA[]

unsigned char cod_mov_var_reg[] = {0x44,0x8b,0x55,0x100};										// move para %r10d o valor da  variavel 100-indice
unsigned char cod_mov_cte_reg[] = {0x41,0xba,0x00,0x00,0x00,0x00} 								// move para %r10d o valor da constante 00 00 00 00 
unsigned char cod_mov_reg_var[] = {0x44,0x89,0x55,0x100}; 										// move para 100-indice da variavel, o valor de %r10d     

unsigned char cod_mov_cte_parm[] = {0xbf,0x00,0x00,0x00,0x00}; 									// move para %edi, a constante 00 00 00 00 (little)
unsigned char cod_mov_var_parm[] = {0x8b,0x7d,0x100}; 											// move para %edi, a variavel 100-indice da memoria
unsigned char cod_mov_parm_pilha[] = {0x89,0x7d,0xe4}; 											// move %edi para a posição de memoria -28(%rbp)

unsigned char cod_opr_add_cte_reg[] = {0x41,0x81,0xc2,0x00,0x00,0x00,0x00}; 					// adiciona a constante 00 00 00 00 em %r10d
unsigned char cod_opr_add_var_reg[] = {0x44,0x03,0x55,0x100}; 									// adiciona em %r10d o valor da variavel 100-indice

unsigned char cod_opr_sub_cte_reg[] = {0x41,0x81,0xea,0x00,0x00,0x00,0x00}; 					// subtrai o valor da constante 00 00 00 00 em %r10d
unsigned char cod_opr_sub_var_reg[] = {0x44,0x2b,0x55,0x100}; 									// subtrai o valor da variavel 100-indice em %r10d

unsigned char cod_opr_mult_cte_reg[] = {0x45,0x69,0xd2,0x00,0x00,0x00,0x00}; 					// multiplica o valor de %r10d pela constante 00 00 00 00 
unsigned char cod_opr_mul_var_reg[] = {0x44,0x0f,0xaf,0x55,0x100}; 								// multiplica o valor de %r10d pela variavel 100-indice

unsigned char cod_call[] = {0xe8,0x00,0x00,0x00,0x00,0x89,0x45,0x100}; 							/*MOVER PARAMETRO ANTES!! COD_MOV_CTE_PARM ou COD_MOV_VAR_PARM // 																									gera o call, e move %eax para a variavel 100-indice*/

unsigned char cod_ret_cte[] = {0xb8,0x00,0x00,0x00,0x00}; 										// move constante 00 00 00 00 para %eax
unsigned char cod_ret_var[] = {0x8b,0x45,0x100}; 												// move constante variavel 100-indice para %eax
unsigned char cod_ret_parm[] = {0x89,0xf8}; 													// move %edi para %eax (este caso provavelmente não será usado)


unsigned char cod_zret[] = {};


unsigned char cod_end[] = {0x4c,0x8b,0x55,0xe0,0xc9,0xc3}; // Desfaz a pilha e restaura %r10 (de -32(%rbp) para %r10

unsigned char end_func[50] = {}

void gera_codigo (FILE *f, void **code, funcp *entry) {

  int line = 1;
  int  c;

	*code = (unsigned char *) malloc(1024);
	/* O valor 1024 foi estimado através do comando mais
		 custo (em código de máquina), que é a soma (SBF).
		 Custa 21 bytes. Com o máximo de 50 linhas...
		 1050, que é bem próximo de 1024 (o arredondamento
		 pode ser feito pois é absurdo o programa só ter
		 operações aritméticas) */

  while ((c = fgetc(f)) != EOF) {
    switch (c) {
      case 'f': { /* function */
        char c0;
        if (fscanf(f, "unction%c", &c0) != 1)
          error("comando invalido", line);
        printf("function\n");
        //
        break;
      }
      case 'e': { /* end */
        char c0;
        if (fscanf(f, "nd%c", &c0) != 1)
          error("comando invalido", line);
        printf("end\n");
        break;
      }
      case 'r': {  /* retorno incondicional */
        int idx0, idx1;
        char var0, var1;
        if (fscanf(f, "et %c%d", &var0, &idx0) != 2) 
          error("comando invalido", line);
        printf("ret %c%d\n", var0, idx0);
        break;
      }
      case 'z': {  /* retorno condicional */
        int idx0, idx1;
        char var0, var1;
        if (fscanf(f, "ret %c%d %c%d", &var0, &idx0, &var1, &idx1) != 4) 
          error("comando invalido", line);
        printf("zret %c%d %c%d\n", var0, idx0, var1, idx1);
        break;
      }
      case 'v': {  /* atribuicao */
        int idx0;
        char var0 = c, c0;
        if (fscanf(f, "%d = %c",&idx0, &c0) != 2)
          error("comando invalido",line);

        if (c0 == 'c') { /* call */
          int func, idx1;
          char var1;
          if (fscanf(f, "all %d %c%d\n", &func, &var1, &idx1) != 3)
            error("comando invalido",line);
          printf("%c%d = call %d %c%d\n",var0, idx0, func, var1, idx1);
        }
        else { /* operação aritmética */
          int idx1, idx2;
          char var1 = c0, var2, op;
          if (fscanf(f, "%d %c %c%d", &idx1, &op, &var2, &idx2) != 4)
            error("comando invalido", line);
          printf("%c%d = %c%d %c %c%d\n",
                var0, idx0, var1, idx1, op, var2, idx2);
        }
        break;
      }
      default: error("comando desconhecido", line);
    }
    line ++;
    fscanf(f, " ");
  }
  return;
}

