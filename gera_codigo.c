/*
Aluno: Guilherme Dantas Matrícula: 1713155
Aluno: Rafael Damazio Matrícula: 1712990
*/

/*****************************************************************
*
*	gera_codigo.c
*
*	Descrição:
*		Este módulo compila o código da linguagem SBF para
*		linguagem de máquina um por vez.
*
*	Autores:
*		gui	-	Guilherme Dantas
*		raf -	Rafael Damazio
*
*	Versionamento:
*		Versão     Data			Autores		Descrição
*		   1		06/11/2018	gui,raf		Início do desenvolvimento
*		 1.1    08/11/2018  gui,raf		Estrutura da função gera_codigo
*		 1.2    13/11/2018  gui				Funções write_commands e print_commands
*		 1.3		14/11/2018	gui				Funções cmd_function, cmd_ret, cmd_end, num_lendian
*
******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gera_codigo.h"

#define _DEBUG

#define DIM_VT_CODIGO 1024
#define PRINT_HEX_PER_LINE 8

#define SZ_FUNCTION 11
#define SZ_END 2
#define SZ_RET_CTE 5
#define SZ_RET_PARM 3
#define SZ_RET_VAR 3

/********* Protótipos das funções encapsuladas pelo módulo *********/

static void error (const char *msg, int line);
static void print_commands ( void );
static void print_headers ( void );
static int write_commands ( unsigned char *commands, size_t bytes );
static void num_lendian ( unsigned char *commands, size_t pos, size_t bytes, int number );
static int cmd_function ( void );
static int cmd_end ( void );
static int cmd_ret ( char var, int idx );

/***************** Código de máquina das variáveis *****************/

static unsigned char cod_function[SZ_FUNCTION] = {0x55,0x48,0x89,0xe5,0x48,0x83,0xec,0x20,0x89,0x7d,0xe4};
// Inicia a pilha e armazena %edi

static unsigned char cod_mov_var_reg[] = {0x44,0x8b,0x55,0x00}; // move para %r10d o valor da  variavel 00-indice
static unsigned char cod_mov_cte_reg[] = {0x41,0xba,0x00,0x00,0x00,0x00}; // move para %r10d o valor da constante 00 00 00 00 
static unsigned char cod_mov_reg_var[] = {0x44,0x89,0x55,0x00}; // move para 00-indice da variavel, o valor de %r10d     

static unsigned char cod_mov_cte_parm[] = {0xbf,0x00,0x00,0x00,0x00}; // move para %edi, a constante 00 00 00 00 (little)
static unsigned char cod_mov_var_parm[] = {0x8b,0x7d,0x00}; // move para %edi, a variavel 00-indice da memoria
static unsigned char cod_mov_parm_pilha[] = {0x89,0x7d,0xe4}; // move %edi para a posição de memoria -28(%rbp)

static unsigned char cod_opr_add_cte_reg[] = {0x41,0x81,0xc2,0x00,0x00,0x00,0x00}; // adiciona a constante 00 00 00 00 em %r10d
static unsigned char cod_opr_add_var_reg[] = {0x44,0x03,0x55,0x00}; // adiciona em %r10d o valor da variavel 00-indice

static unsigned char cod_opr_sub_cte_reg[] = {0x41,0x81,0xea,0x00,0x00,0x00,0x00}; // subtrai o valor da constante 00 00 00 00 em %r10d
static unsigned char cod_opr_sub_var_reg[] = {0x44,0x2b,0x55,0x00}; // subtrai o valor da variavel 00-indice em %r10d

static unsigned char cod_opr_mult_cte_reg[] = {0x45,0x69,0xd2,0x00,0x00,0x00,0x00}; // multiplica o valor de %r10d pela constante 00 00 00 00 
static unsigned char cod_opr_mult_var_reg[] = {0x44,0x0f,0xaf,0x55,0x00}; // multiplica o valor de %r10d pela variavel 00-indice

static unsigned char cod_call[] = {0xe8,0x00,0x00,0x00,0x00,0x89,0x45,0x00}; /*MOVER PARAMETRO ANTES!! COD_MOV_CTE_PARM ou COD_MOV_VAR_PARM / gera o call, e move %eax para a variavel 00-indice*/
static unsigned char cod_ret_cte[SZ_RET_CTE] = {0xb8,0x00,0x00,0x00,0x00}; // move constante 00 00 00 00 para %eax
static unsigned char cod_ret_var[] = {0x8b,0x45,0x00}; // move constante variavel 00-indice para %eax
static unsigned char cod_ret_parm[] = {0x8b,0x45,0xe4};// move -28(%edi) para %eax (este caso provavelmente não será usado)

static unsigned char cod_zret_reg[] = {0x41,0x83,0xfa,0x00,0x75,0x05}; // sempre trabalha com %r10d e com o mesmo label
static unsigned char cod_zret_pil[] = {0x83,0x7d,0xCC,0x00,0x75,0x05}; // CC -- subtrai o valor da variavel 100-indice em %r10d

static unsigned char cod_end[SZ_END] = {0xc9,0xc3}; // Desfaz a pilha e retorna ao ultimo endereço

static unsigned char end_func[50] = {};
static unsigned int  qtd_func     = 0 ;

static unsigned int  byte_corr    = 0 ;
/* code[byte_corr] é o byte corrente */

static unsigned char * pCode = NULL;
/* ponteiro global que aponta para o código
   Como nosso módulo opera apenas com um código
   por vez, esta implementação é o suficiente. */

void gera_codigo (FILE *f, void **code, funcp *entry) {
	
  int line = 1;
  int  c;
  
  if( code == NULL ) {
  	error("Vetor nulo fornecido a funcao que gera codigo",0);
  }
  
  pCode = ( unsigned char * ) *code;
  
	pCode = (unsigned char *) malloc(DIM_VT_CODIGO);
	/* O valor 1024 foi estimado através do comando mais
		 custo (em código de máquina), que é a soma (SBF).
		 Custa 21 bytes. Com o máximo de 50 linhas...
		 1050, que é bem próximo de 1024 (o arredondamento
		 pode ser feito pois é absurdo o programa só ter
		 operações aritméticas) */

	if( pCode == NULL ) {
		error("Memoria insuficiente para alocar vetor de comandos!",0);
	}

  while ((c = fgetc(f)) != EOF) {
    switch (c) {
      case 'f': { /* function */
        char c0;
        if (fscanf(f, "unction%c", &c0) != 1)
          error("comando invalido", line);
        	printf("function\n");
        	if( cmd_function() )
        		error("Memoria insuficiente!", line);
        		
        	#ifdef _DEBUG
        	printf("\n>>> Cabeçalhos:\n");
        	print_headers();
        	printf("\n>>> Comandos:\n");
        	print_commands();
        	printf("\n\n");
        	#endif
        	
        break;
      }
      case 'e': { /* end */
        char c0;
        if (fscanf(f, "nd%c", &c0) != 1)
          error("comando invalido", line);
        printf("end\n");
        if( cmd_end() )
        	error("Memoria insuficiente!", line);
        	
        #ifdef _DEBUG
      	printf("\n>>> Comandos:\n");
      	print_commands();
      	printf("\n\n");
      	#endif
        	
        break;
      }
      case 'r': {  /* retorno incondicional */
        int idx0, idx1, ret;
        char var0, var1;
        if (fscanf(f, "et %c%d", &var0, &idx0) != 2) 
          error("comando invalido", line);
        printf("ret %c%d\n", var0, idx0);
        ret = cmd_ret( var0, idx0 );
        if( ret == 1 )
        	error("Memoria insuficiente!", line);
        else if( ret == -1 )
        	error("Parametros invalidos!", line);
        
        #ifdef _DEBUG
      	printf("\n>>> Comandos:\n");
      	print_commands();
      	printf("\n\n");
      	#endif
        
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

/*************** Funções encapsuladas pelo módulo ***************/

/***************************************************************
*
*	error	- Exibe erro
*
*	Descrição:
*		Escreve uma mensagem de erro customizada, indicando
*		a linha em que ele ocorreu, e para a execução do pgm.
*
*	Parâmetros:
*		msg	  -	string da mensagem de erro
*		line  -	linha em que erro ocorreu
*
****************************************************************/

void error (const char *msg, int line) {

  fprintf(stderr, "erro %s na linha %d\n", msg, line);
  exit(EXIT_FAILURE);
  
} /* fim da função error */

/***************************************************************
*
*	num_lendian - Converte número em little endian
*
*	Descrição:
*		Altera um campo numérico de um comando escrito como
*		um número inteiro, em little endian.
*
*	Assertivas de Entrada:
*		Assume que pos+bytes<len(commands).
*
*	Parâmetros:
*		commands	- comandos a serem alterados
*		pos				- posicao no vetor commands a partir do qual
*								o número será impresso
*		bytes			- quantidade de bytes do número a ser armazenado
*		number		- o número a ser impresso
*
****************************************************************/

void num_lendian ( unsigned char *commands, size_t pos, size_t bytes, int number ) {

	int i = 0;
	char byte_nulo;

	if( number < 0 ) {
		number = ~(-(number+1)); /* inv(-(x+1)) */
		byte_nulo = 0xff;
	} /* if */
	else {
		byte_nulo = 0x00;
	} /* else */

	while( bytes-- ) {
		char num_byte = number & 0xff;
		commands[pos+i] = number ? num_byte : byte_nulo;
		number = number >> 8;
		i++;
	} /* while */
	
} /* fim da função num_lendian */

/***************************************************************
*
*	print_commands	- Imprime o código
*
*	Descrição:
*		Imprime no terminal o código em hexadecimal.
*		Serve somente para debug.
*
****************************************************************/

void print_commands( void ) {

	int i = 0;
	
	while( i < byte_corr ) {
	
		printf("%.02x ",pCode[i]);
		
		if( i % PRINT_HEX_PER_LINE == PRINT_HEX_PER_LINE - 1 &&
				i != qtd_func-1  ) {
			printf("\n");
		} /* if */
		
		i++;
		
	} /* while */
	
} /* fim da função print_commands */

/***************************************************************
*
*	print_headers	- Imprime cabeçalhos das funções
*
*	Descrição:
*		Imprime no terminal o endereço dos cabeçalhos das funções
*		em hexadecimal.
*		Serve somente para debug.
*
****************************************************************/

void print_headers( void ) {
	
	int i = 0;
	
	while( i < qtd_func ) {
	
		printf("%.02x ",end_func[i]);
		
		if( i % PRINT_HEX_PER_LINE == PRINT_HEX_PER_LINE - 1 &&
				i != qtd_func-1 ) {
			printf("\n");
		} /* if */
		
		i++;
		
	} /* while */
	
} /* fim da função print_headers */

/***************************************************************
*
*	write_commands	- Escreve comandos
*
*	Descrição:
*		Escreve os bytes de comandos em código de máquina
*		no vetor code, atualizando o contador de bytes byte_corr.
*
*	Parâmetros:
*		commands  -	vetor com os comandos em código de máquina
*								que serão escritos
*		bytes	  	-	tamanho do vetor commands
*
*	Retorno:
*		0 se escreveu comandos com sucesso
*		1 se extrapolaria limite do vetor code (não escreve)
*
****************************************************************/

int write_commands ( unsigned char *commands, size_t bytes ) {

	int i = 0;
	
	if( pCode == NULL ) {
		error("Ponteiro nulo!",0);
	}
	
	if( byte_corr + bytes > DIM_VT_CODIGO ) {
		return 1;
	} /* if */
	
	while( bytes-- ) {
		pCode[byte_corr] = commands[i];
		byte_corr++;
		i++;
	} /* while */
	
	return 0;
} /* fim da função write_commands */

/***************************************************************
*
*	cmd_function	- Compilar lexema "function"
*
*	Descrição:
*		Cria a pilha da chamada da função, e move os valores
*		dos registradores para esta mesma pilha.
*
*	Retorno:
*		0 se escreveu comandos com sucesso
*		1 se extrapolaria limite do vetor code (não escreve)
*
****************************************************************/

int cmd_function ( void ) {
	
	/*************************************************************
	* "function" em asm			|	"function" em bytes
	*												|
	*	pushq %rbp						|	55
	*	movq %rsp, %rbp				| 48 89 e5
	*	subq $32, %rsp				| 48 83 ec 20
	*	movl %edi, -28(%rbp)	| 89 7d e4
	*												|
	**************************************************************/
	
	end_func[qtd_func] = byte_corr;
	qtd_func++;
	return write_commands( cod_function , SZ_FUNCTION );
	
} /* fim da função cmd_function */

/***************************************************************
*
*	cmd_end	- Compilar lexema "end"
*
*	Descrição:
*		Desfaz a pilha, e volta para a função anterior.
*
*	Retorno:
*		0 se escreveu comandos com sucesso
*		1 se extrapolaria limite do vetor code (não escreve)
*
****************************************************************/

int cmd_end ( void ) {

	/*************************************************************
	* "end" em asm					|	"end" em bytes
	*												|
	*	leave									|	c9
	*	ret										| c3
	*												|
	**************************************************************/

	return write_commands( cod_end , SZ_END );

} /* fim da função cmd_end */

/***************************************************************
*
*	cmd_ret	- Compilar lexema "ret"
*
*	Descrição:
*		Armazena a variável a ser retornada em %eax.
*
*	Assertivas de Entrada:
*		Assume que os parâmetros são válidos, isto é,
*		para var = 'p', idx = 0 e para var = 'v',
*		idx pertence ao conjunto {0,1,2,3,4} ,
*		para var = '$', idx deve estar dentro dos limtes de INT.
*		Caso esta AE seja descumprida, -1 será retornado.
*
*	Retorno:
*		0 se escreveu comandos com sucesso
*		-1 se os parâmetros são inválidos.
*		1 se extrapolaria limite do vetor code (não escreve)
*
****************************************************************/

int cmd_ret ( char var, int idx ) {

	switch( var ) {
	
		case '$':
		
	/*************************************************************
	* "ret $cte" em asm			|	"ret $cte" em bytes
	*												|
	*	movl $cte, %eax				|	b8 00 00 00 00
	*												|    ----cte----
	**************************************************************/
		
		num_lendian( cod_ret_cte , 1 , 4 , idx );
		return write_commands( cod_ret_cte , SZ_RET_CTE );
	
		case 'p':
		
	/*************************************************************
	* "ret p0" em asm				|	"ret p0" em bytes
	*												|
	*	movl -28(%rbp), %eax	|	8b 45 e4
	*												| 
	**************************************************************/
		
		return write_commands( cod_ret_parm , SZ_RET_PARM );
		
		case 'v':
		
	/*************************************************************
	* "ret vi" em asm				|	"ret vi" em bytes
	*												|
	*	movl off(%rbp), %eax	|	8b 45 00
	*												| 			off = -4*(idx+1)
	**************************************************************/
		
		num_lendian( cod_ret_var , 2 , 1 , -4*(idx+1) );
		return write_commands( cod_ret_var , SZ_RET_VAR );
		
		default:
		
			return -1;
			
	} /*switch*/

} /* fim da função cmd_ret */




