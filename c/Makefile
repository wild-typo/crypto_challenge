CC=clang
CFLAGS=-Iinclude -std=c99 -g -O0

SRCS = $(wildcard src/*.c)
OBJS = $(patsubst src/%.c, obj/%.o, $(SRCS))

.PHONY: all
all: bin/crypto_challenge

bin/crypto_challenge: $(OBJS)
	$(CC) -o $@ $^

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm $(OBJS) bin/crypto_challenge

