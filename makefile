CC = gcc

IDIR = ./include/
SRCDIR = ./src/

CFLAGS = -I$(IDIR)
LDFLAGS = -lncurses

SOURCES = $(SRCDIR)*.c
TARGET = sudoku_solver

all: $(TARGET) run

$(TARGET):
	$(CC) $(SOURCES) $(CFLAGS) -O -o $(TARGET) $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
