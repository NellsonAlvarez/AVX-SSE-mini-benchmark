all: dot_prod.asm
	nasm -g -f elf64 -o objeto.o dot_prod.asm
	g++ -g -no-pie main.cpp objeto.o 


	
