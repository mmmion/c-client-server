CC = gcc
CFLAGS = -Wall -Wextra -g

SRC_DIR = src
BUILD_DIR = build

CLIENT_SOURCES = $(filter $(SRC_DIR)/client.c $(SRC_DIR)/client_cli.c, $(wildcard $(SRC_DIR)/*.c))
CLIENT_OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(CLIENT_SOURCES))

SERVER_SOURCES = $(filter-out $(SRC_DIR)/client.c $(SRC_DIR)/client_cli.c, $(wildcard $(SRC_DIR)/*.c))
SERVER_OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SERVER_SOURCES))

SERVER_EXE = $(BUILD_DIR)/server
CLIENT_EXE = $(BUILD_DIR)/client

.PHONY: all clean

all: clean $(BUILD_DIR) $(SERVER_EXE) $(CLIENT_EXE)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(SERVER_EXE): $(SERVER_OBJECTS) $(BUILD_DIR)/log.o
	$(CC) $(CFLAGS) $^ -o $@

$(CLIENT_EXE): $(CLIENT_OBJECTS) $(BUILD_DIR)/log.o
	$(CC) $(CFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)
