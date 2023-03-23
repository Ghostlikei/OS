CC=clang
CFLAGS=-Wall -Wextra -Werror

OBJS=./main.o ./utils/src/exec.o ./utils/src/history.o ./utils/src/builtin.o ./utils/src/mytop.o ./utils/src/node.o

MOBJS=./main.o ./exec.o ./history.o ./builtin.o ./mytop.o ./node.o

all: main

main: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# This is used for minix
mshell:
	$(CC) $(CFLAGS) -c -o ./utils/src/exec.o ./utils/src/exec.c
	$(CC) $(CFLAGS) -c -o ./utils/src/history.o ./utils/src/history.c
	$(CC) $(CFLAGS) -c -o ./utils/src/builtin.o ./utils/src/builtin.c
	$(CC) $(CFLAGS) -c -o ./utils/src/mytop.o ./utils/src/mytop.c
	$(CC) $(CFLAGS) -c -o ./utils/src/node.o ./utils/src/node.c
	$(CC) $(CFLAGS) -c -o ./main.o ./main.c
	$(CC) $(CFLAGS) -o main $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

debug: CFLAGS+=-DSHDEBUG
debug: main

clean:
	rm -f $(OBJS) main
	rm -f $(MOBJS) mshell