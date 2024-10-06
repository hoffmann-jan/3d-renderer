CC = gcc
CFLAGS = -Wall -std=c99 -lm -I/opt/homebrew/include -I/opt/homebrew/include/SDL2
LDFLAGS = -L/opt/homebrew/lib -lSDL2

SRC = ./src/*.c ./src/app/*.c ./src/data-structures/*.c
TARGET = renderer

build:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

run:
	./$(TARGET)

clean:
	rm -f $(TARGET)
