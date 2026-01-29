##
## EPITECH PROJECT, 2025
## B-OOP-400-TLS-4-1-raytracer-antonin.veau
## File description:
## Makefile
##

SRC = 	$(wildcard src/Math/*.cpp) \
		$(wildcard src/Primitives/*.cpp) \
		$(wildcard src/Raytracer/*.cpp) \
		$(wildcard src/Raytracer/Color/*.cpp) \
		$(wildcard src/Raytracer/Light/*.cpp) \
		$(wildcard src/Raytracer/BVH/*.cpp) \
		$(wildcard src/Rendering/*.cpp) \
		$(wildcard src/*.cpp) \

OBJ = $(SRC:.cpp=.o)

NAME = raytracer

all: $(NAME)

FLAGS = -lsfml-graphics -lsfml-window -lsfml-system

FASTFLAGS = -march=native -ftree-vectorize -O3 -freciprocal-math -flto

$(NAME): $(OBJ)
		g++ -o $(NAME) $(OBJ) $(FLAGS)

debug:
	g++ -g -o $(NAME) $(SRC) $(FLAGS) $(FASTFLAGS)

clean:
	rm -f $(OBJ)
	rm -f *~
	rm -f vgcore*

fclean: clean
		rm -f raytracer

re: fclean all

.PHONY: all clean fclean re