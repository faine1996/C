# Compiler and Flags
CC = gcc
CFLAGS = -ansi -pedantic -Wall -Wextra -I./inc

# Target Names
APP_TARGET = shell_app
TEST_TARGET = shell_test
PINGPONG_TARGET = pingpong_test

# --- NEW NETWORK TARGETS ---
SERVER_TARGET = tcp_server_nonblock
CLIENT_TARGET = tcp_client
STOC_TEST_TARGET = test_tcp_pingpong

# Object Files for Shell
OBJ_FILES = src/shell.o src/main.o
TEST_OBJ_FILES = src/shell.o test/test_shell.o

# Object Files for Ping-Pong
PINGPONG_OBJ_FILES = src/pingpong.o test/test_pingpong.o

# --- NEW NETWORK OBJECT FILES ---
SERVER_OBJ = src/tcp_server_nonblock.o
CLIENT_OBJ = src/tcp_client.o
STOC_TEST_OBJ = test/test_tcp_pingpong.o

# Default target
all: $(APP_TARGET)

# Build the Production Shell
$(APP_TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $(APP_TARGET) $(OBJ_FILES)

# Build and run the Shell Test Suite
test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJ_FILES)
	$(CC) $(CFLAGS) -o $(TEST_TARGET) $(TEST_OBJ_FILES)

# Build and run the Ping-Pong Test Suite
test_pingpong: $(PINGPONG_TARGET)
	./$(PINGPONG_TARGET)

$(PINGPONG_TARGET): $(PINGPONG_OBJ_FILES)
	$(CC) $(CFLAGS) -o $(PINGPONG_TARGET) $(PINGPONG_OBJ_FILES)

# --- NEW NETWORK BUILD RULES ---

# Build the Non-Blocking Server
server: $(SERVER_TARGET)

$(SERVER_TARGET): $(SERVER_OBJ)
	$(CC) $(CFLAGS) -o $(SERVER_TARGET) $(SERVER_OBJ)

# Build the Standard Client
client: $(CLIENT_TARGET)

$(CLIENT_TARGET): $(CLIENT_OBJ)
	$(CC) $(CFLAGS) -o $(CLIENT_TARGET) $(CLIENT_OBJ)

# Build the Stochastic Test Client
test_stochastic: $(STOC_TEST_TARGET)

$(STOC_TEST_TARGET): $(STOC_TEST_OBJ)
	$(CC) $(CFLAGS) -o $(STOC_TEST_TARGET) $(STOC_TEST_OBJ)

# Pattern rule for object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Cleanup
clean:
	rm -f src/*.o test/*.o $(APP_TARGET) $(TEST_TARGET) $(PINGPONG_TARGET) $(SERVER_TARGET) $(CLIENT_TARGET) $(STOC_TEST_TARGET)