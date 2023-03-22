CC=clang
CFLAGS=-Wall -Wextra -Werror -Iutils

OBJS=main.o utils/src/exec.o utils/src/history.o utils/src/builtin.o utils/src/mytop.o utils/src/node.o

all: main

main: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

debug: CFLAGS+=-DSHDEBUG
debug: main

clean:
	rm -f $(OBJS) main