/* Guilherme Dantas  1713155  3WB */
/* Rafael Damazio    1712990  3WB */

/* grava_structs.c */

#include <stdio.h>
#include <stdlib.h>

/* ------------------------------------------------
	variáveis encapsuladas
   ------------------------------------------------ */

static unsigned char id_campos[4] = {sizeof(char),sizeof(short int),sizeof(int),sizeof(long int)};

/* ------------------------------------------------
	protótipo das funções encapsuladas 
   ------------------------------------------------ */

static int get_size_struct(char* campos);

/* ------------------------------------------------
	código das funções exportadas pelo módulo 
   ------------------------------------------------ */

/* ------------------------------------------------
	FUNÇÃO:	grava_structs
	DESC:	armazena no arquivo bin. os dados do vetor
			valores de nstructs estruturas com dados
			campos, e dada ordenação ('L'/'B')
	RET:	0	houve sucesso
			-1	não houve sucesso
   ------------------------------------------------ */

int grava_structs (int nstructs, void *valores, char *campos, char ord, char *arquivo){
	/* Abre o arquivo em binário para escrita */
	
	FILE *fname = fopen(arquivo,"wb");
	unsigned char byte = 0;
	int ncampos = 0, i = 0, j = 0, k =0;
	int pAux = 0;
	int size_struct = get_size_struct(campos);

	fwrite(&nstructs,sizeof(char),1,fname);

	while( campos[ncampos] != '\0' )
	{
		ncampos += 1;
	} /* while */
	
	if( ord == 'L' )
	{
		byte = (1 << 7) | (0x7F & (char) ncampos);
	} /* if */
	else
	{
		byte = (0x7F & (char) ncampos);
	} /* else */

	fwrite(&byte,sizeof(char),1,fname);
	byte = 0;
	
	for(i = 0 ; i < ncampos ; i++ )
	{
		unsigned char campo_bits;
		
		switch( campos[i] ){
			case 'c':
				campo_bits = 0x0;
				break;
			case 's':
				campo_bits = 0x1;
				break;
			case 'i':
				campo_bits = 0x2;
				break;
			case 'l':
				campo_bits = 0x3;
				break;
		} /* switch */
		
		byte <<= 2;
		byte = byte | campo_bits;

		if( i%4 == 3 )
		{
			fwrite(&byte,sizeof(char),1,fname);
			byte = 0;
		} /* if */
		
	} /* for */
	
	if( ncampos %4 != 0 )
	{
		byte <<= 2*(4 - ncampos%4);
		fwrite(&byte,sizeof(char),1,fname);
	} /* if */

	for(i=0;i<nstructs;i++)
	{
		
		j = 0; /* contagem dos campos de cada struct */
		pAux = 0; /* pAux é um inteiro que varia de 0 até o tamanho da struct */
		while(j < ncampos)
		{
			int size_valor; /* size_valor é o tamanho em bytes do campo atual */
			switch(campos[j])
			{
				case 'c':
					size_valor = sizeof(char);
					break;
				case 's':
					size_valor = sizeof(short int);
					break;
				case 'i':
					size_valor = sizeof(int);
					break;
				case 'l':
					size_valor = sizeof(long int);
					break;
			} /* switch */
			
			if(pAux != 0)
			{
				pAux = ((pAux-1)/size_valor + 1)*size_valor; /* Ajuste do endereço ao campo */
			} /* if */
			
			if (ord=='L')
			{
				fwrite((valores+i*size_struct+pAux),size_valor,1,fname); /* Escrita */
			} /* if */
			else
			{
				for( k = size_valor-1 ; k >= 0 ; k-- )
				{
					fwrite((valores+i*size_struct+pAux)+k,sizeof(char),1,fname); /* Escrita */
				} /* for */
			} /* else */
			
			pAux += size_valor; /* Incremento do ponteiro auxiliar pelo tamanho do campo */
			
			j += 1;
			
		} /* while */
		
	} /* for */

	fclose(fname);

   return 0;
} /* Término da função grava_structs */

/* ------------------------------------------------
	FUNÇÃO:	dump_structs
	DESC:	lê um arquivo binário e exibe na tela
			os valores de cada instância de vetor
			de struct
   ------------------------------------------------ */

void dump_structs (char *arquivo){

	FILE *fname = fopen(arquivo,"rb");
	
	unsigned char 	nstructs,
						endian,
						ncampos,
						nbytes_campos,
						i, j, k,
						buffer,
						*campos;
	
	if( fname == NULL )
	{
		printf("ERRO: Arquivo nao encontrado");
		return;
	} /* if */
	
	fread(&nstructs,sizeof(char),1,fname);
	fread(&endian,sizeof(char),1,fname);
	
	ncampos = 0x7F & endian;
	endian >>= 7;
	
	campos = (unsigned char *) malloc(sizeof(unsigned char)*ncampos);
	
	if( campos == NULL )
	{
		printf("ERRO: Nao foi possivel alocar um vetor");
		return;
	} /* if */
	
	nbytes_campos = (ncampos-1)/4 + 1;
	
	for( i = 0 ; i < nbytes_campos ; i++ )
	{
		fread(&buffer,sizeof(char),1,fname);
		for( j = 0 ; j < 4 ; j++ )
		{
			unsigned char id;
			if(i*4 + j >= ncampos)
			{
				break;
			} /* if */
				
			id = 0xC0 & buffer;
			id >>= 6;
			
			campos[i*4+j] = id_campos[id];
			
			buffer <<= 2;
		} /* for */
	} /* for */
	
	printf("%c\n",endian?'L':'B');
	printf("%d\n",nstructs);
	
	for( i = 0 ; i < nstructs ; i++ )
	{
		printf("*\n");
		for( j = 0 ; j < ncampos ; j++ )
		{
			for( k = 0 ; k < campos[j] ; k++ )
			{
				fread(&buffer,sizeof(char),1,fname);
				printf("%02x ",buffer);
			} /* for */
			printf("\n");
		} /* for */
	} /* for */
	
	fclose(fname);

} /* Término da função dump_structs */

/* ------------------------------------------------
	código das funções encapsuladas 
   ------------------------------------------------ */

int get_size_struct(char* campos){

	int i,				/* iterador */
		ncampos = 0,	/* número de campos no vetor campos */
		end = 0,		/* iterador de endereço */
		maior = 1;		/* maior campo do vetor em bytes */

	for( i = 0 ; campos[i] != '\0' ; i++ )
	{
		ncampos += 1;	/* conta a quantidade de campos */
	} /* for */

	for( i = 0 ; i < ncampos ; i++ )
	{
		/* para cada campo obtem-se o tamanho deste na memória */
		int tamanho_var = 1;
		
		switch( campos[i] )
		{
		case 'c':
			tamanho_var = 1;
			break;
		case 's':
			tamanho_var = 2;
			break;
		case 'i':
			tamanho_var = 4;
			break;
		case 'l':
			tamanho_var = 8;
			break;
		default:
			/* Caso um dos caracteres seja inválido, termina a função */
			printf("Caractere invalido! (\'%c\')\n", campos[i] );
			return -1;
		} /* switch */
		
		if( tamanho_var > maior )
		{
			maior = tamanho_var;  /* obtem-se o maior campo em bytes */
		} /* if */
		
		if( i != 0 )
		{
			end = ((end-1)/tamanho_var + 2)*tamanho_var; /* ajuste de endereço de memória pelo campo */
		}
		
	} /* for */
	return ((end-1)/maior + 1)*maior; /* ajuste de endereço de memória pelo maior campo */

}/* fim da função get_size_struct */
