CC = gcc
CFLAGS = -ansi -pedantic -Wall -Wextra -g -I./inc
SRC_DIR = src
TEST_DIR = test
OBJ_DIR = obj

# Executable Targets
TARGET_EXPR   = test_expr_runner
TARGET_STACK  = test_stack_runner
TARGET_PERSON = test_person_runner
TARGET_DLIST  = test_dlist_runner
TARGET_BST    = test_bst_runner

# Object File Definitions
OBJS_EXPR   = $(OBJ_DIR)/expression_parser.o $(OBJ_DIR)/adt_stack.o $(OBJ_DIR)/test_expression_parser.o
OBJS_STACK  = $(OBJ_DIR)/adt_stack.o $(OBJ_DIR)/test_adt_stack.o
OBJS_PERSON = $(OBJ_DIR)/person_list.o $(OBJ_DIR)/test_person_list.o
OBJS_DLIST  = $(OBJ_DIR)/new_gen_dlist.o $(OBJ_DIR)/test_new_gen_dlist.o

# FIXED: Matches the actual file test/test_BSTree.c
OBJS_BST    = $(OBJ_DIR)/BSTree.o $(OBJ_DIR)/test_BSTree.o

all: $(TARGET_EXPR) $(TARGET_STACK) $(TARGET_PERSON) $(TARGET_DLIST) $(TARGET_BST)

# Link rules
$(TARGET_BST): $(OBJS_BST)
	$(CC) $(CFLAGS) -o $@ $^

$(TARGET_DLIST): $(OBJS_DLIST)
	$(CC) $(CFLAGS) -o $@ $^

$(TARGET_EXPR): $(OBJS_EXPR)
	$(CC) $(CFLAGS) -o $@ $^

$(TARGET_STACK): $(OBJS_STACK)
	$(CC) $(CFLAGS) -o $@ $^

$(TARGET_PERSON): $(OBJS_PERSON)
	$(CC) $(CFLAGS) -o $@ $^

# Pattern rule for objects from src directory
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Pattern rule for objects from test directory
$(OBJ_DIR)/%.o: $(TEST_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(TARGET_EXPR) $(TARGET_STACK) $(TARGET_PERSON) $(TARGET_DLIST) $(TARGET_BST)