NAME    = f1
CC        = gcc 
FLAGS    = -Wall -Wextra -Werror -fsanitize=address -g

SRCS = libs/csv.c libs/display.c libs/sort.c libs/logic.c main.c

OBJS = $(SRCS:.c=.o)

all: $(OBJS)
	$(CC) $(FLAGS) -o $(NAME) $(OBJS) -lncurses

%.o: %.c
	$(CC) $(FLAGS) -o $@ -c $<

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re