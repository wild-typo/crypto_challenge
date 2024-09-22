CC=clang
CFLAGS=-Iinclude -std=c99 -g -O0

SRCS = $(wildcard src/*.c)
OBJS = $(patsubst src/%.c, obj/%.o, $(SRCS))

.PHONY: all
all: bin/hex2base64

bin/hex2base64: $(OBJS)
	$(CC) -o $@ $^

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm $(OBJS) bin/hex2base64

