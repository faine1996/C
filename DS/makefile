CC = gcc
CFLAGS = -ansi -pedantic -Wall -Wextra -g -I./inc
SRC_DIR = src
TEST_DIR = test
OBJ_DIR = obj

# 1. Define our output executables
TARGET_EXPR = test_expr_runner
TARGET_STACK = test_stack_runner
TARGET_PERSON = test_person_runner   # <--- Added new target

# 2. Define specific object files for each executable
OBJS_EXPR = $(OBJ_DIR)/expression_parser.o $(OBJ_DIR)/adt_stack.o $(OBJ_DIR)/test_expression_parser.o
OBJS_STACK = $(OBJ_DIR)/adt_stack.o $(OBJ_DIR)/test_adt_stack.o
OBJS_PERSON = $(OBJ_DIR)/person_list.o $(OBJ_DIR)/test_person_list.o  # <--- Added new objects

# 'all' builds everything by depending on all targets
all: $(TARGET_EXPR) $(TARGET_STACK) $(TARGET_PERSON)

# Link the Person List test runner
$(TARGET_PERSON): $(OBJS_PERSON)
	$(CC) $(CFLAGS) -o $@ $^

# Link the Expression Parser test runner
$(TARGET_EXPR): $(OBJS_EXPR)
	$(CC) $(CFLAGS) -o $@ $^

# Link the Stack test runner
$(TARGET_STACK): $(OBJS_STACK)
	$(CC) $(CFLAGS) -o $@ $^

# Pattern rule: Compile any .c in src/ to a .o in obj/
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Pattern rule: Compile any .c in test/ to a .o in obj/
$(OBJ_DIR)/%.o: $(TEST_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up all generated files
clean:
	rm -rf $(OBJ_DIR) $(TARGET_EXPR) $(TARGET_STACK) $(TARGET_PERSON)