# I'm really good at making really basic makefiles

$CC=clang

tetris: src/tetris.c
	$(CC) src/tetris.c -o bin/tetris -lncurses

tetris-one: src/tetris-one.c
	$(CC) src/tetris-one.c -o bin/tetris-one -lncurses -lwiiuse
