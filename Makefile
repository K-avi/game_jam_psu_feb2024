INCLUDE = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -I/usr/include/SDL2 -lm
FILES = main.c intro/intro.c labo/*.c timer/timer.c point_and_click/*.c

test_matrix : 
	gcc test_matrix.c labo/matrix.c -g -Og -o test_matrix

dev :
	gcc $(FILES) -o main -Wall -Wextra $(INCLUDE)

clang :
	clang $(FILES) -o main -Wall -Wextra $(INCLUDE)

release :
	gcc $(FILES) -o main -O3 $(INCLUDE)

debug :
	gcc main.c -o main -g -Wall -Wextra -fsanitize=address $(INCLUDE)

intro :
	gcc intro/intro.c -o intro_exec -g -Wall -Wextra $(INCLUDE)

try :
	gcc $(FILES) -o exec -g -Wall -Wextra $(INCLUDE)
