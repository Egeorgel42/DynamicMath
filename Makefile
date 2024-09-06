CC = c++
SRC = DynamicMath.cpp \
addition.cpp \
DynamicUtils.cpp \
testing.cpp \
multiplication.cpp
OBJ = ${SRC:.cpp=.o}
CFLAGS = -Wall -Werror -Wextra -std=c++17 -g #-fsanitize=address
NAME = DynamicTesting

all: $(NAME)

$(NAME): $(OBJ) Makefile
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

%.o: %.cpp
	$(CC) $(CFLAGS) -o $@ -c $<

.PHONY: all clean fclean re