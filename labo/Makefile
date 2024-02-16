CFLAG = -Wall -Wextra
INCLUDE = -lSDL2 -lSDL2_ttf -lSDL2_mixer -lSDL2_image -I/usr/include/SDL2 -lm
LIB = AffichageLabo.o matrix.o test.o

compil : $(LIB)
	

AffichageLabo.o : AffichageLabo.c
	gcc $(CFLAG) -c AffichageLabo.c $(INCLUDE)

matrix.o : matrix.c
	gcc $(CFLAG) -c matrix.c $(INCLUDE)

test.o : test.c
	gcc $(CFLAG) -c test.c $(INCLUDE)

debug : compil
	gcc $(CFLAGS) -o test $(LIB) $(INCLUDE)

clean : 
	- rm *.o *~ test