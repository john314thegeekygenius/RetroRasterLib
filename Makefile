
LIB_NAME := libRRasterLib

CXX_FLAGS := -Wall -O3 -std=c++20 -Iinclude -I/usr/include/SDL2
CXX_LIBS := -lSDL2main -lSDL2 -lSDL2_image
LINK_FLAGS := -Wl

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
SHARED_RRLIB_FLAGS := -L$(LIB_DIR)/shared -l:$(LIB_NAME).so

OBJ_FILES := $(OBJ_DIR)/RR_Lib.o $(OBJ_DIR)/RR_Logger.o $(OBJ_DIR)/RR_Graphics.o $(OBJ_DIR)/RR_Window.o 

static: lib_static tests_static

shared: lib_shared tests_shared

all: dirs clean lib_static lib_shared tests_static tests_shared

# More info on library making
# https://renenyffenegger.ch/notes/development/languages/C-C-plus-plus/GCC/create-libraries/index
dirs:
	mkdir -p $(LIB_DIR)/static
	mkdir -p $(LIB_DIR)/shared
	mkdir -p $(OBJ_DIR)
	mkdir -p $(BIN_DIR)/tests/static
	mkdir -p $(BIN_DIR)/tests/shared

lib_static:
	g++ -c $(CXX_FLAGS) $(SRC_DIR)/RR_Logger.cpp -o $(OBJ_DIR)/RR_Logger.o $(CXX_LIBS)
	g++ -c $(CXX_FLAGS) $(SRC_DIR)/RR_Graphics.cpp -o $(OBJ_DIR)/RR_Graphics.o $(CXX_LIBS)
	g++ -c $(CXX_FLAGS) $(SRC_DIR)/RR_Window.cpp -o $(OBJ_DIR)/RR_Window.o $(CXX_LIBS)
	g++ -c $(CXX_FLAGS) $(SRC_DIR)/RR_Lib.cpp -o $(OBJ_DIR)/RR_Lib.o $(CXX_LIBS)
	ar rcs $(LIB_DIR)/static/$(LIB_NAME).a $(OBJ_FILES)

lib_shared:
	g++ -c -fPIC $(CXX_FLAGS) $(SRC_DIR)/RR_Logger.cpp -o $(OBJ_DIR)/RR_Logger.o $(CXX_LIBS)
	g++ -c -fPIC $(CXX_FLAGS) $(SRC_DIR)/RR_Graphics.cpp -o $(OBJ_DIR)/RR_Graphics.o $(CXX_LIBS)
	g++ -c -fPIC $(CXX_FLAGS) $(SRC_DIR)/RR_Window.cpp -o $(OBJ_DIR)/RR_Window.o $(CXX_LIBS)
	g++ -c -fPIC $(CXX_FLAGS) $(SRC_DIR)/RR_Lib.cpp -o $(OBJ_DIR)/RR_Lib.o $(CXX_LIBS)
	g++ -shared $(LINK_FLAGS),-soname,$(LIB_NAME).so -o $(LIB_DIR)/shared/$(LIB_NAME).so $(OBJ_FILES)

tests_static:
	g++ $(CXX_FLAGS) $(TEST_SRC_DIR)/test_log.cpp -o $(BIN_DIR)/tests/static/test_log $(STATIC_RRLIB_FLAGS) $(CXX_LIBS)
	g++ $(CXX_FLAGS) $(TEST_SRC_DIR)/test_shapes.cpp -o $(BIN_DIR)/tests/static/test_shapes $(STATIC_RRLIB_FLAGS) $(CXX_LIBS)
	g++ $(CXX_FLAGS) $(TEST_SRC_DIR)/test_image.cpp -o $(BIN_DIR)/tests/static/test_image $(STATIC_RRLIB_FLAGS) $(CXX_LIBS)
	g++ $(CXX_FLAGS) $(TEST_SRC_DIR)/test_window.cpp -o $(BIN_DIR)/tests/static/test_window $(STATIC_RRLIB_FLAGS) $(CXX_LIBS)
	g++ $(CXX_FLAGS) $(TEST_SRC_DIR)/test_keyboard.cpp -o $(BIN_DIR)/tests/static/test_keyboard $(STATIC_RRLIB_FLAGS) $(CXX_LIBS)
	g++ $(CXX_FLAGS) $(TEST_SRC_DIR)/test_mouse.cpp -o $(BIN_DIR)/tests/static/test_mouse $(STATIC_RRLIB_FLAGS) $(CXX_LIBS)
	g++ $(CXX_FLAGS) $(TEST_SRC_DIR)/test_multi_window.cpp -o $(BIN_DIR)/tests/static/test_multi_window $(STATIC_RRLIB_FLAGS) $(CXX_LIBS)
	g++ $(CXX_FLAGS) $(TEST_SRC_DIR)/test_joystick.cpp -o $(BIN_DIR)/tests/static/test_joystick $(STATIC_RRLIB_FLAGS) $(CXX_LIBS)
	g++ $(CXX_FLAGS) $(TEST_SRC_DIR)/test_graphics.cpp -o $(BIN_DIR)/tests/static/test_graphics $(STATIC_RRLIB_FLAGS) $(CXX_LIBS)

tests_shared:
	g++ $(CXX_FLAGS) $(TEST_SRC_DIR)/test_log.cpp -o $(BIN_DIR)/tests/shared/test_log $(SHARED_RRLIB_FLAGS) $(CXX_LIBS)
	g++ $(CXX_FLAGS) $(TEST_SRC_DIR)/test_shapes.cpp -o $(BIN_DIR)/tests/shared/test_shapes $(SHARED_RRLIB_FLAGS) $(CXX_LIBS)
	g++ $(CXX_FLAGS) $(TEST_SRC_DIR)/test_image.cpp -o $(BIN_DIR)/tests/shared/test_image $(SHARED_RRLIB_FLAGS) $(CXX_LIBS)
	g++ $(CXX_FLAGS) $(TEST_SRC_DIR)/test_window.cpp -o $(BIN_DIR)/tests/shared/test_window $(SHARED_RRLIB_FLAGS) $(CXX_LIBS)
	g++ $(CXX_FLAGS) $(TEST_SRC_DIR)/test_keyboard.cpp -o $(BIN_DIR)/tests/shared/test_keyboard $(SHARED_RRLIB_FLAGS) $(CXX_LIBS)
	g++ $(CXX_FLAGS) $(TEST_SRC_DIR)/test_mouse.cpp -o $(BIN_DIR)/tests/shared/test_mouse $(SHARED_RRLIB_FLAGS) $(CXX_LIBS)
	g++ $(CXX_FLAGS) $(TEST_SRC_DIR)/test_multi_window.cpp -o $(BIN_DIR)/tests/shared/test_multi_window $(SHARED_RRLIB_FLAGS) $(CXX_LIBS)
	g++ $(CXX_FLAGS) $(TEST_SRC_DIR)/test_joystick.cpp -o $(BIN_DIR)/tests/shared/test_joystick $(SHARED_RRLIB_FLAGS) $(CXX_LIBS)
	g++ $(CXX_FLAGS) $(TEST_SRC_DIR)/test_graphics.cpp -o $(BIN_DIR)/tests/shared/test_graphics $(SHARED_RRLIB_FLAGS) $(CXX_LIBS)

clean:
	rm -f $(BIN_DIR)/test/shared/test_*
	rm -f $(BIN_DIR)/test/static/test_*
	rm -f $(LIB_DIR)/shared/*.so
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
