#include <stdio.h>

int fact( int x );

int main( int argc, char * argv[] )
{
	int num = 0, res;
	char *p = argv[2];
	
	while( *p != '\0' ) {
    num *= 10;
    if( *p < '0' || *p > '9' ) {
      printf("Argumento nao-numerico!\n");
      return 0;
    }
    num += *p - '0';
    p++;
  }
	
	res = fact( num );
	printf("fact(%d) = %d\n",num,res);
	
}
