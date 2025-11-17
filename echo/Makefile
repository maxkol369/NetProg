CC = gcc
CFLAGS = -Wall -std=c99
TARGETS = echo echo_client

all: $(TARGETS)

echo_client: echo.cpp
	$(CC) $(CFLAGS) -o $@ $<

echo_client_simple: echo_client.cpp
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS)

test: echo_client_simple
	./echo_client_simple

.PHONY: all clean test
