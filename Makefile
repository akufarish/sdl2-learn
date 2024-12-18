all:
	gcc -Isrc/Include -Lsrc/lib -o main app.c -lmingw32 -lSDL2main  -lSDL2_ttf -lSDL2_image -lSDL2

