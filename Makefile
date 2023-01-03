
LIB_NAME := RRasterLib

CXX_FLAGS := -std=c++20 -lSDL2main -lSDL2

SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin
LIB_DIR := lib

OBJ_FILES := \
$(OBJ_DIR)/NE_Main.o \
$(OBJ_DIR)/NE_Levels.o \
$(OBJ_DIR)/NE_Graphics.o \
$(OBJ_DIR)/NE_GUI.o \
$(OBJ_DIR)/NE_GUI_Functions.o

build:
	g++ -c $(CXX_FLAGS) $(SRC_DIR)/NE_Main.cpp -o $(OBJ_DIR)/NE_Main.o
	g++ -c $(CXX_FLAGS) $(SRC_DIR)/NE_Levels.cpp -o $(OBJ_DIR)/NE_Levels.o
	g++ -c $(CXX_FLAGS) $(SRC_DIR)/NE_Graphics.cpp -o $(OBJ_DIR)/NE_Graphics.o
	g++ -c $(CXX_FLAGS) $(SRC_DIR)/NE_GUI.cpp -o $(OBJ_DIR)/NE_GUI.o
	g++ -c $(CXX_FLAGS) $(SRC_DIR)/NE_GUI_Functions.cpp -o $(OBJ_DIR)/NE_GUI_Functions.o
	g++ -o $(BIN_DIR)/NukeEdit $(OBJ_FILES) $(CXX_FLAGS)

clean:
	rm -f $(BIN_DIR)/test_*
	rm -f $(LIB_DIR)/LIB_NAME
	rm -f $(OBJ_DIR)/*.o
