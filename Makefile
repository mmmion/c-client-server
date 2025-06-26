CC = gcc
CFLAGS = -Wall -Wextra -g

SRC_DIR = src
BUILD_DIR = build

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES))

# Final executables (change or add as needed)
SERVER_EXE = $(BUILD_DIR)/server
CLIENT_EXE = $(BUILD_DIR)/client

.PHONY: all clean

all: clean $(BUILD_DIR) $(SERVER_EXE) $(CLIENT_EXE)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Link server executable from server.o (add other needed .o files)
$(SERVER_EXE): $(BUILD_DIR)/server.o $(BUILD_DIR)/log.o
	$(CC) $(CFLAGS) $^ -o $@

# Link client executable from client.o (add other needed .o files)
$(CLIENT_EXE): $(BUILD_DIR)/client.o $(BUILD_DIR)/log.o
	$(CC) $(CFLAGS) $^ -o $@

# Generic rule to compile .c to .o inside build/
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)
