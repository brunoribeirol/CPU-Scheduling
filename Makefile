CC = gcc
CFLAGS = -Wall -Wextra

all: real

real: real.c
	$(CC) $(CFLAGS) -o $@ $^

rate: real.c
	$(CC) $(CFLAGS) -D RATE_MONOTONIC -o $@ $^

edf: real.c
	$(CC) $(CFLAGS) -D EARLIEST_DEADLINE_FIRST -o $@ $^

clean:
	rm -f real rate edf
