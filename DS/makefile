CC = gcc
CFLAGS = -ansi -pedantic -Wall -Wextra -g -I./inc
SRC_DIR = src
TEST_DIR = test
OBJ_DIR = obj

# Output executables
TARGET_EXPR = test_expr_runner
TARGET_STACK = test_stack_runner
TARGET_PERSON = test_person_rec_runner  # Renamed for clarity

# Specific object files
OBJS_EXPR = $(OBJ_DIR)/expression_parser.o $(OBJ_DIR)/adt_stack.o $(OBJ_DIR)/test_expression_parser.o
OBJS_STACK = $(OBJ_DIR)/adt_stack.o $(OBJ_DIR)/test_adt_stack.o

# Updated to match your new filename: test_person_list_rec.c
# This assumes your source logic is still in person_list.c 
# or you've renamed that to person_list_rec.c as well.
OBJS_PERSON = $(OBJ_DIR)/person_list_rec.o $(OBJ_DIR)/test_person_list_rec.o

all: $(TARGET_EXPR) $(TARGET_STACK) $(TARGET_PERSON)

$(TARGET_PERSON): $(OBJS_PERSON)
	$(CC) $(CFLAGS) -o $@ $^

$(TARGET_EXPR): $(OBJS_EXPR)
	$(CC) $(CFLAGS) -o $@ $^

$(TARGET_STACK): $(OBJS_STACK)
	$(CC) $(CFLAGS) -o $@ $^

# Pattern rules - These handle the "heavy lifting"
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(TEST_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(TARGET_EXPR) $(TARGET_STACK) $(TARGET_PERSON)