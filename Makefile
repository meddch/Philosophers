RED = \033[0;31m
GREEN = \033[0;32m
BLUE = \033[0;34m
CC = cc
CFLAGS = -Wall -Wextra -Werror 
RM = rm -f
NAME = philo
FILE_M = mandatoty/ft_atoi mandatoty/main mandatoty/thread mandatoty/utils
HEADER = mandatoty/philosopher.h

SRCS_M = $(addsuffix .c, $(FILE_M))
OBJS_M = $(addsuffix .o, $(FILE_M))


%.o : %.c $(HEADER)
	@echo "$(BLUE)Building project ⏳.."
	@$(CC) $(CFLAGS) -c -o $@ $<
	
all : $(NAME)
	@echo  "$(GREEN)Building done successfully ✅"

$(NAME): $(OBJS_M) 
	@$(CC) $(OBJS_M) -o $(NAME)

clean:
	@$(RM) $(OBJS_M)
	@echo  "$(RED)ALL DELETED ☠ "


fclean: clean
	@$(RM) $(NAME)

re: fclean all 


.PHONY: all clean fclean re 