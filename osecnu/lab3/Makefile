CC = clang
CFLAGS = -Wall -Wextra

.PHONY: all buildram test clean

all: buildram test

buildram: buildmyram

minix_test: test

buildmyram: buildmyram.c
	$(CC) $(CFLAGS) -o buildmyram buildmyram.c

test: test.c
	$(CC) $(CFLAGS) -o test test.c

clean:
	rm -f buildmyram test *.txt