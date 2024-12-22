CC=clang
CP=cp
CFLAGS=-Iinclude -std=c99 -g -O0

SRCS = $(wildcard src/*.c)
OBJS = $(patsubst src/%.c, obj/%.o, $(SRCS))
RES = res/

.PHONY: all
all: bin/crypto_challenge bin/res

bin/crypto_challenge: $(OBJS)
	$(CC) -o $@ $^

bin/res: $(RES)
	$(CP) -r $^ $@

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm $(OBJS) bin/crypto_challenge
	rm -r bin/res

