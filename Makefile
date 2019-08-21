# I'm really good at making really basic makefiles

$CC=clang

tetris: tetris.c
	$(CC)  tetris.c -o tetris -lncurses
