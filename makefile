
all : bin/canny

bin/canny : executables/main.c src/pgm-image.c src/canny.c src/gauss.c
	gcc -W -Wall -O3 -L headers -lm -o $@ $^

clean :
	rm -rf bin/*

valgrind : bin/canny
	valgrind --leak-check=full --track-origins=yes ./bin/canny inputs/racoon2.pgm outputs/racoon-new.pgm

run : bin/canny
	./bin/canny inputs/8k.pgm outputs/8k.pgm
