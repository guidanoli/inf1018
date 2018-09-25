/* Guilherme Dantas  1713155  3WB */
/* Rafael Damazio    1712990  3WB */

#include <stdio.h>


int grava_structs (int nstructs, void *valores, char *campos, char ord, char *arquivo){
	/* Abre o arquivo em binário para escrita */
	FILE *fname = fopen(arquivo,"wb");
	unsigned char byte = 0;
	int ncampos = 0, i = 0,j;
	int pAux = 0;
	
	fwrite(&nstructs,sizeof(char),1,fname);

	while( campos[ncampos] != '\0' ){
		ncampos += 1;
	}
	
	if( ord == 'L' ){
		byte = (1 << 7) | (0x7F & (char) ncampos);
	}

	fwrite(&byte,sizeof(char),1,fname);
	byte = 0;
	
	for(i = 0 ; i < ncampos ; i++ ){
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
		}
		byte <<= 2;
		byte = byte | campo_bits;

		if( i%4 == 3 ){
			fwrite(&byte,sizeof(char),1,fname);
			byte = 0;
		}
	}
	
	if( ncampos %4 != 0 ){
		byte <<= 2*(4 - ncampos%4);
		fwrite(&byte,sizeof(char),1,fname);
	}
	if (ord=='L'){
		for(i=0;i<nstructs;i++){
			
			j = 0; /* contagem dos campos de cada struct */
			pAux = 0; /* pAux é um inteiro que varia de 0 até o tamanho da struct */
			while(pAux < sizeof(*valores)){
			
				int size_valor; /* size_valor é o tamanho em bytes do campo atual */
				
				switch(campos[j]){
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
				
				pAux = ((pAux-1)/size_valor + 1)*size_valor; /* Ajuste do endereço ao campo */
				fwrite((valores+i*sizeof(*valores)+pAux),size_valor,1,fname); /* Escrita */
				pAux += size_valor; /* Incremento do ponteiro auxiliar pelo tamanho do campo */
				j += 1;
				
			} /* while */
			
		} /* for */
		
	} /* if */


	fclose(fname);

   return 0;
}/* Término da função grava_structs */

void dump_structs (char *arquivo){

}
