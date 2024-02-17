CFLAG = -Wall -Wextra -g
INCLUDE = -lSDL2 -lSDL2_ttf -lSDL2_mixer -lSDL2_image -I/usr/include/SDL2 -lm
LIB = AffichageLabo.o matrix.o common.o

compil : $(LIB)
	

AffichageLabo.o : AffichageLabo.c
	gcc $(CFLAG) -c AffichageLabo.c $(INCLUDE)

matrix.o : matrix.c
	gcc $(CFLAG) -c matrix.c $(INCLUDE)

common.o : common.c
	gcc $(CFLAG) -c common.c $(INCLUDE)



clean : 
	- rm *.o *~ test