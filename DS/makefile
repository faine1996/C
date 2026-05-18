CC = gcc
CFLAGS = -ansi -pedantic -Wall -Wextra -g -fPIC -I./inc
LDFLAGS = -shared

SRC_DIR = src
TEST_DIR = test
OBJ_DIR = obj
LIB_DIR = lib

TARGET_LIB = $(LIB_DIR)/libds.so

LIB_OBJS = $(OBJ_DIR)/genvector.o \
           $(OBJ_DIR)/genqueue.o \
           $(OBJ_DIR)/new_gen_dlist.o \
           $(OBJ_DIR)/genheap.o \
           $(OBJ_DIR)/HashMap.o \
           $(OBJ_DIR)/BSTree.o \
           $(OBJ_DIR)/stack.o \

TARGET_EXPR   = test_expr_runner
TARGET_STACK  = test_stack_runner
TARGET_PERSON = test_person_runner
TARGET_DLIST  = test_dlist_runner
TARGET_BST    = test_bst_runner
TARGET_HEAP   = test_heap_runner
TARGET_HASHMAP = test_hashmap_runner

OBJS_EXPR   = $(OBJ_DIR)/expression_parser.o $(OBJ_DIR)/adt_stack.o $(OBJ_DIR)/test_expression_parser.o
OBJS_STACK  = $(OBJ_DIR)/adt_stack.o $(OBJ_DIR)/test_adt_stack.o
OBJS_PERSON = $(OBJ_DIR)/person_list.o $(OBJ_DIR)/test_person_list.o
OBJS_DLIST  = $(OBJ_DIR)/new_gen_dlist.o $(OBJ_DIR)/test_new_gen_dlist.o
OBJS_BST    = $(OBJ_DIR)/BSTree.o $(OBJ_DIR)/test_BSTree.o
OBJS_HEAP   = $(OBJ_DIR)/genvector.o $(OBJ_DIR)/genheap.o $(OBJ_DIR)/test_genheap.o
OBJS_HASHMAP = $(OBJ_DIR)/new_gen_dlist.o $(OBJ_DIR)/HashMap.o $(OBJ_DIR)/test_HashMap.o

all: $(TARGET_LIB) $(TARGET_EXPR) $(TARGET_STACK) $(TARGET_PERSON) $(TARGET_DLIST) $(TARGET_BST) $(TARGET_HEAP) $(TARGET_HASHMAP)

$(TARGET_LIB): $(LIB_OBJS)
	@mkdir -p $(LIB_DIR)
	$(CC) $(LDFLAGS) -o $@ $^

$(TARGET_HASHMAP): $(OBJS_HASHMAP)
	$(CC) $(CFLAGS) -o $@ $^

$(TARGET_HEAP): $(OBJS_HEAP)
	$(CC) $(CFLAGS) -o $@ $^

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

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(TEST_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(LIB_DIR) $(TARGET_EXPR) $(TARGET_STACK) $(TARGET_PERSON) $(TARGET_DLIST) $(TARGET_BST) $(TARGET_HEAP) $(TARGET_HASHMAP)