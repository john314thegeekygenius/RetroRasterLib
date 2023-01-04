
LIB_NAME := libRRasterLib

CXX_FLAGS := -Wall -O3 -std=c++20 -Iinclude -I/usr/include/SDL2
CXX_LIBS := -lSDL2main -lSDL2 

# Where the source code is
SRC_DIR := src
TEST_SRC_DIR := src/tests
# Where the object files will be
OBJ_DIR := obj
# Where the library will be
LIB_DIR := lib
# Where the tests will be
BIN_DIR := bin

# Library Flags
STATIC_RRLIB_FLAGS := -L$(LIB_DIR)/static -l:$(LIB_NAME).a
SHARED_RRLIB_FLAGS := -L$(LIB_DIR)/static -l:$(LIB_NAME).a

OBJ_FILES := $(OBJ_DIR)/RR_Lib.o $(OBJ_DIR)/RR_Logger.o $(OBJ_DIR)/RR_Graphics.o 

all: dirs clean build_lib build_tests

# More info on library making
# https://renenyffenegger.ch/notes/development/languages/C-C-plus-plus/GCC/create-libraries/index
dirs:
	mkdir -p $(LIB_DIR)/static
	mkdir -p $(OBJ_DIR)
	mkdir -p $(BIN_DIR)/tests

build_lib:
	g++ -c $(CXX_FLAGS) $(SRC_DIR)/RR_Logger.cpp -o $(OBJ_DIR)/RR_Logger.o $(CXX_LIBS)
	g++ -c $(CXX_FLAGS) $(SRC_DIR)/RR_Graphics.cpp -o $(OBJ_DIR)/RR_Graphics.o $(CXX_LIBS)
	g++ -c $(CXX_FLAGS) $(SRC_DIR)/RR_Lib.cpp -o $(OBJ_DIR)/RR_Lib.o $(CXX_LIBS)
	ar rcs $(LIB_DIR)/static/$(LIB_NAME).a $(OBJ_FILES)
build_tests:
	g++ $(CXX_FLAGS) $(TEST_SRC_DIR)/test_log.cpp -o $(BIN_DIR)/tests/test_log $(STATIC_RRLIB_FLAGS) $(CXX_LIBS)
	g++ $(CXX_FLAGS) $(TEST_SRC_DIR)/test_shapes.cpp -o $(BIN_DIR)/tests/test_shapes $(STATIC_RRLIB_FLAGS) $(CXX_LIBS)
	g++ $(CXX_FLAGS) $(TEST_SRC_DIR)/test_window.cpp -o $(BIN_DIR)/tests/test_window $(STATIC_RRLIB_FLAGS) $(CXX_LIBS)

clean:
	rm -f $(BIN_DIR)/test/test_*
	rm -f $(LIB_DIR)/static/*.a
	rm -f $(OBJ_DIR)/*.o
git:
	git add src
	git add include
	git add Makefile
	git add RetroRasterLib.code-workspace
	git add .gitignore
	git add *.md
	@read -p "Please enter a commit message:" msg; \
	git commit -m "$$msg"
	git push
