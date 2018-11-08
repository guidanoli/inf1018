#include "gera_codigo.h"

int main(int argc, char *argv[]) {
  FILE *fp;
  void *code;
  funcp funcSBF;
  int res;
  
  if( argc != 2 )
  {
  	printf("Numero invalido de parametros\n");
    return 0;
  }
  
  /* Abre o arquivo para leitura */
  fp = fopen(argv[1], "r");
  
  if( fp == NULL )
  {
  	printf("Erro ao abrir o arquivo\n");
    return 0;
  }
  
  /* Gera o codigo */
  gera_codigo(fp, &code, &funcSBF);
  if ((code == NULL) || (funcSBF == NULL)) {
    printf("Erro na geracao\n");
    return 0;
  }
  
  /* Chama a função gerada */
  //res = (*funcSBF)(argv[2]);
  
  /* Libera a memória utilizada */
  //libera_codigo(code);
  
  return 0;  
}
