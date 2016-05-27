all:
	gcc -ansi  -c 3.0.c -o 3.0.o
	gcc -o snake.exe 3.0.o
	rm 3.0.o
