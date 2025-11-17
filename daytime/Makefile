CXX = g++
CXXFLAGS = -std=c++11
TARGET = daytime
SOURCES = daytime.cpp

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)

test:
	./$(TARGET) 172.16.40.1

.PHONY: clean test
