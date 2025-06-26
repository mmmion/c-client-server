CC = gcc
CFLAGS = -Wall -Wextra -g

SRC_DIR = src
BUILD_DIR = build

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES))

SERVER_EXE = $(BUILD_DIR)/server
CLIENT_EXE = $(BUILD_DIR)/client

.PHONY: all clean

all: clean $(BUILD_DIR) $(SERVER_EXE) $(CLIENT_EXE)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(SERVER_EXE): $(BUILD_DIR)/server.o $(BUILD_DIR)/log.o
	$(CC) $(CFLAGS) $^ -o $@

$(CLIENT_EXE): $(BUILD_DIR)/client.o $(BUILD_DIR)/log.o
	$(CC) $(CFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)
