
all:main.bin

main.bin: pontua.o inimigo.o nave.o tiro.o bloco.o main.o
	gcc -o main.bin pontua.o inimigo.o nave.o tiro.o bloco.o main.o $(shell pkg-config allegro-5 allegro_font-5 allegro_image-5 allegro_image-5 allegro_ttf-5 allegro_primitives-5 --libs --cflags)

pontua.o: pontua.c 
	gcc $(shell pkg-config allegro-5 allegro_font-5 allegro_image-5 allegro_image-5 allegro_ttf-5 allegro_primitives-5 --libs --cflags) -c pontua.c

inimigo.o: inimigo.c 
	gcc $(shell pkg-config allegro-5 allegro_font-5 allegro_image-5 allegro_image-5 allegro_ttf-5 allegro_primitives-5 --libs --cflags) -c inimigo.c
	
nave.o: nave.c 
	gcc $(shell pkg-config allegro-5 allegro_font-5 allegro_image-5 allegro_image-5 allegro_ttf-5 allegro_primitives-5 --libs --cflags) -c nave.c
	
tiro.o: tiro.c 
	gcc $(shell pkg-config allegro-5 allegro_font-5 allegro_image-5 allegro_image-5 allegro_ttf-5 allegro_primitives-5 --libs --cflags) -c tiro.c

bloco.o: bloco.c 
	gcc $(shell pkg-config allegro-5 allegro_font-5 allegro_image-5 allegro_image-5 allegro_ttf-5 allegro_primitives-5 --libs --cflags) -c bloco.c

main.o: main.c
	gcc main.c $(shell pkg-config allegro-5 allegro_font-5 allegro_image-5 allegro_image-5 allegro_ttf-5 allegro_primitives-5 --libs --cflags) -c main.c
	
clean:
	rm *.o 


