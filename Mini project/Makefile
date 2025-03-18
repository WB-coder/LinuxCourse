CC = gcc
CFLAGS = -Wall -Wextra -I./inc -pthread
LDFLAGS = -lreadline -lpthread
SRC = src/main.c src/cli.c src/message.c src/connection.c src/utils.c src/common.c
OBJ = $(SRC:.c=.o)
TARGET = peer_app

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean