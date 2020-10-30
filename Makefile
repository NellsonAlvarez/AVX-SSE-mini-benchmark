all: dot_prod.asm
	nasm -g -f elf64 -o objeto.o dot_prod.asm
	g++ -c -g -no-pie -o otro.o main.cpp  
	g++ -g -no-pie -o casm objeto.o otro.o 


	