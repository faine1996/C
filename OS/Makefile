# Compiler and Flags
CC = gcc
CFLAGS = -ansi -pedantic -Wall -Wextra -I./inc

# Target Names
APP_TARGET = shell_app
TEST_TARGET = shell_test

# Source Files
SRC_FILES = src/shell.c
MAIN_FILE = src/main.c
TEST_FILE = test/test_shell.c

# Object Files
OBJ_FILES = src/shell.o src/main.o
TEST_OBJ_FILES = src/shell.o test/test_shell.o

# Default target: builds the actual shell
all: $(APP_TARGET)

# Build the Production Shell
$(APP_TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $(APP_TARGET) $(OBJ_FILES)

# Build the Test Suite
test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJ_FILES)
	$(CC) $(CFLAGS) -o $(TEST_TARGET) $(TEST_OBJ_FILES)

# Pattern rule for object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Cleanup
clean:
	rm -f src/*.o test/*.o $(APP_TARGET) $(TEST_TARGET)