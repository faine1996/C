# Compiler and Flags
CC = gcc
CFLAGS = -ansi -pedantic -Wall -Wextra -I./inc

# Target Names
APP_TARGET = shell_app
TEST_TARGET = shell_test
PINGPONG_TARGET = pingpong_test

# Object Files for Shell
OBJ_FILES = src/shell.o src/main.o
TEST_OBJ_FILES = src/shell.o test/test_shell.o

# Object Files for Ping-Pong
PINGPONG_OBJ_FILES = src/pingpong.o test/test_pingpong.o

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

# Pattern rule for object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Cleanup
clean:
	rm -f src/*.o test/*.o $(APP_TARGET) $(TEST_TARGET) $(PINGPONG_TARGET)