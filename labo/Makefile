CFLAG = -Wall -Wextra
INCLUDE = -lSDL2 -lSDL2_ttf -lSDL2_mixer -lSDL2_image -I/usr/include/SDL2 -lm


compil : AffichageLabo.c
	gcc $(CFLAGS) -c AffichageLabo.c $(INCLUDE)

debug : compil test.c
	gcc $(CFLAGS) -o test test.c AffichageLabo.o $(INCLUDE)