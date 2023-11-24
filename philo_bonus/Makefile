MAKEFLAGS += --silent
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
SRCS = main.c process.c utils.c free.c
OBJS_EXEC = ${SRCS:.c=.o}
EXEC = philo_bonus

all: ${EXEC}
	echo "\033[7;32m~| Tous les fichiers sont à jour ! |~\033[0m"\

${EXEC}: ${OBJS_EXEC}
	${CC} ${CFLAGS} -I ./philo.h ${OBJS_EXEC} -o ${EXEC} -lpthread\
		&& echo "\033[1;32m~| Compilation de $@ : OK |~\033[0m"\
		|| echo "\033[1;31m~| Compilation de $@ : Erreur |~\033[0m"

clean:
	rm -f ${OBJS_BONUS}
	rm -f ${OBJS_EXEC}\
		&& echo "\033[1;36m~| Nettoyage des .o : OK |~\033[0m"\
		|| echo "\033[0;31m~| Nettoyage des $@ : Erreur |~\033[0m"

fclean: clean
	rm -f ${BONUS}
	rm -f ${EXEC} \
		&& echo "\033[1;36m~| Nettoyage des executables : OK |~\033[0m"\
		|| echo "\033[0;31m~| Nettoyage des $@ : Erreur |~\033[0m"

re: fclean all
