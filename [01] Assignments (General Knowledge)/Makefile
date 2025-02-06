CUR_DIR = .
BIN_DIR = $(CUR_DIR)/bin
INC_DIR = $(CUR_DIR)/inc
OBJ_DIR = $(CUR_DIR)/obj
LIB_DIR = $(CUR_DIR)/lib
SRC_DIR = $(CUR_DIR)/src
LIB_STATIC_DIR = $(LIB_DIR)/static
LIB_SHARED_DIR = $(LIB_DIR)/shared

# Compilation flags
C_FLAGS = -I $(INC_DIR) -fPIC
LDFLAGS = -L$(LIB_STATIC_DIR) -L$(LIB_SHARED_DIR)

# Create object files
create_objs:
	gcc $(C_FLAGS) -c $(SRC_DIR)/helloThinh.c -o $(OBJ_DIR)/helloThinh.o
	gcc $(C_FLAGS) -c $(SRC_DIR)/helloWorld.c -o $(OBJ_DIR)/helloWorld.o
	gcc $(C_FLAGS) -c $(SRC_DIR)/main.c -o $(OBJ_DIR)/main.o

# Create static library
create_static_lib:
	ar rcs $(LIB_STATIC_DIR)/libhello.a $(OBJ_DIR)/helloThinh.o $(OBJ_DIR)/helloWorld.o

# Create shared library
create_shared_lib:
	gcc -shared -o $(LIB_SHARED_DIR)/libhello.so $(OBJ_DIR)/helloThinh.o $(OBJ_DIR)/helloWorld.o

# Build targets with static and shared libraries
static: create_objs create_static_lib
	gcc $(OBJ_DIR)/main.o -L$(LIB_STATIC_DIR) -lhello -o $(BIN_DIR)/use-static-library

shared: create_objs create_shared_lib
	gcc $(OBJ_DIR)/main.o -L$(LIB_SHARED_DIR) -lhello -o $(BIN_DIR)/use-shared-library

# Build both library types
all: static shared

# Clean up
clean:
	rm -rf $(OBJ_DIR)/*.o
	rm -rf $(BIN_DIR)/use-static-library
	rm -rf $(BIN_DIR)/use-shared-library
	rm -rf $(LIB_STATIC_DIR)/libhello.a
	rm -rf $(LIB_SHARED_DIR)/libhello.so

.PHONY: all static shared create_objs create_static_lib create_shared_lib clean