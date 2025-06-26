CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude

SRC_DIR = src
BUILD_DIR = build

# Shared source file(s)
COMMON_SOURCES = $(SRC_DIR)/log.c
COMMON_OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(COMMON_SOURCES))

# Client and server sources
CLIENT_SOURCES = $(SRC_DIR)/client.c $(SRC_DIR)/client_cli.c
CLIENT_OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(CLIENT_SOURCES))

SERVER_SOURCES = $(filter-out $(CLIENT_SOURCES), $(wildcard $(SRC_DIR)/*.c))
SERVER_OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SERVER_SOURCES))

SERVER_EXE = $(BUILD_DIR)/server
CLIENT_EXE = $(BUILD_DIR)/client

.PHONY: all clean

all: $(BUILD_DIR) $(SERVER_EXE) $(CLIENT_EXE)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(SERVER_EXE): $(SERVER_OBJECTS) $(COMMON_OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

$(CLIENT_EXE): $(CLIENT_OBJECTS) $(COMMON_OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)
