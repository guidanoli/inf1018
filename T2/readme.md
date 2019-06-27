# Language compiler and interpreter in C

# Execution

``` shell
$ gcc -Wall -Wa,--execstack -o prog main.c gera_codigo.h gera_codigo.c
$ ./prog sbf.txt
```

# Debug

``` shell
$ gcc -c teste.s
$ objdump -d teste.o
```
