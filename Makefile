INCLUDE = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -I/usr/include/SDL2 -lm
FILES = main.c intro.c minijeux/*.c labo/*.c

test_matrix : 
	gcc test_matrix.c labo/matrix.c -g -Og -o test_matrix

dev :
	gcc main.c */*.c -o main -Wall -Wextra $(INCLUDE)

clang :
	clang main.c -o main -Wall -Wextra $(INCLUDE)

release :
	gcc main.c -o main -O3 $(INCLUDE)

debug :
	gcc main.c -o main -g -Wall -Wextra -fsanitize=address $(INCLUDE)

intro :
	gcc intro/intro.c -o intro_exec -g -Wall -Wextra $(INCLUDE)

try :
	gcc */*.c main.c -o exec -g -Wall -Wextra $(INCLUDE)