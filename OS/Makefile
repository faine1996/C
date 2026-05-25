# Compiler and Flags
CC = gcc
CFLAGS = -ansi -pedantic -Wall -Wextra -I./inc

# Target Names
APP_TARGET = shell_app
TEST_TARGET = shell_test
PINGPONG_TARGET = pingpong_test

# --- NEW NETWORK TARGETS ---
SERVER_TARGET = tcp_server_nonblock
SERVER_SELECT_TARGET = tcp_server_select
CLIENT_TARGET = tcp_client
STOC_TEST_TARGET = test_tcp_pingpong
ENDIAN_TARGET = endianness

# Object Files for Shell
OBJ_FILES = src/shell.o src/main.o
TEST_OBJ_FILES = src/shell.o test/test_shell.o

# Object Files for Ping-Pong
PINGPONG_OBJ_FILES = src/pingpong.o test/test_pingpong.o

# --- NEW NETWORK OBJECT FILES ---
SERVER_OBJ = src/tcp_server_nonblock.o
SERVER_SELECT_OBJ = src/tcp_server_select.o
CLIENT_OBJ = src/tcp_client.o
STOC_TEST_OBJ = test/test_tcp_pingpong.o
ENDIAN_OBJ = src/endianness.o

# Default target: builds the actual shell
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

# Build the Select Server
server_select: $(SERVER_SELECT_TARGET)

$(SERVER_SELECT_TARGET): $(SERVER_SELECT_OBJ)
	$(CC) $(CFLAGS) -o $(SERVER_SELECT_TARGET) $(SERVER_SELECT_OBJ)

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

$(ENDIAN_TARGET): $(ENDIAN_OBJ)
	$(CC) $(CFLAGS) -o $(ENDIAN_TARGET) $(ENDIAN_OBJ)

# Cleanup
clean:
	rm -f src/*.o test/*.o $(APP_TARGET) $(TEST_TARGET) $(PINGPONG_TARGET) $(SERVER_TARGET) $(SERVER_SELECT_TARGET) $(CLIENT_TARGET) $(STOC_TEST_TARGET) $(ENDIAN_TARGET)