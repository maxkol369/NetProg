CC = gcc
CFLAGS = -Wall -std=c99
TARGET = daytime
SOURCES = daytime.cpp

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)

test: $(TARGET)
	./$(TARGET) time.nist.gov

.PHONY: all clean test
