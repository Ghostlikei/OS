CC = clang
CFLAGS = -Wall -Wextra -Werror

.PHONY: all buildram test clean

all: buildram test

buildram: buildmyram

test: test

buildmyram: buildmyram.c
	$(CC) $(CFLAGS) -o buildmyram buildmyram.c

test: test.c
	$(CC) $(CFLAGS) -o test test.c

clean:
	rm -f buildmyram test *.txt