SRCS = falsh.c commands.c

OBJS = $(SRCS:.c=.o)

OPTS = -Wall -O

clean: falsh
	rm falsh

falsh:
	gcc $(OPTS) -o falsh $(SRCS)
