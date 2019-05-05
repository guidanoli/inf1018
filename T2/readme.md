# Trabalho 2 de SOFTWARE BÁSICO (PUC-Rio) 

# Como executar o programa
Basta rodar os seguintes comandos no terminal...
~~~~
gcc -Wall -Wa,--execstack -o prog main.c gera_codigo.h gera_codigo.c
./prog sbf.txt
~~~~

# Como depurar um disassembly
Basta rodar os seguintes comandos no terminal...
~~~~
gcc -c teste.s
objdump -d teste.o
~~~~

Att,
Guilherme Dantas e
Rafael Damazio
