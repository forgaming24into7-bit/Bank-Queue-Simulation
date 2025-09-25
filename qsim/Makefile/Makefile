CC = gcc
CFLAGS = -Iinclude -Wall -g
SRC = src/main.c src/event.c src/customer.c src/teller.c src/queue.c
OBJ = $(SRC:.c=.o)
BIN = bin/qSim

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $(BIN) -lm

clean:
	rm -f src/*.o $(BIN)
