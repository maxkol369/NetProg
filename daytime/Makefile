CC = gcc
CFLAGS = -Wall
TARGETS = daytime daytime.cpp

all: $(TARGETS)

daytime_client: daytime.cpp
	$(CC) $(CFLAGS) -o $@ $<

daytime_client_tcp: daytime_client_tcp.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS)

test: all
	@echo "Testing connection to 172.16.40.1:13"
	@echo "=== UDP Version ==="
	./daytime_client
	@echo ""
	@echo "=== TCP Version ==="
	./daytime_client_tcp

.PHONY: all clean test
