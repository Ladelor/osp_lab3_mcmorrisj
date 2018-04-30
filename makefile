TARG = falsh

OBJS = $(SRCS:.c=.o)

OPTS = -Wall -O

clean: falsh
	rm falsh

false:
	gcc $(OPTS) -o falsh falsh.c
