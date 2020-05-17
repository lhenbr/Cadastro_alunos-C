all:
	gcc -o arquivo+indices main.c arquivo+indices.c -I. -Wall -Wextra 
run:
	./arquivo+indices
