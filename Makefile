# The Compiler
CC = g++

# Compiler Flags (C++17, Optimization level 3 for speed)
CFLAGS = -std=c++17 -O3

# Libraries (Link SDL2)
LDFLAGS = -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL2

# The Target
TARGET = blackhole

all: $(TARGET)

$(TARGET): main.cpp
	$(CC) $(CFLAGS) main.cpp -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)
