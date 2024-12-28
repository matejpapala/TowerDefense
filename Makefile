SRC_DIR = src
BUILD_DIR = build/debug
CC = gcc
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC_FILES))
OBJ_NAME = main
INCLUDE_PATH = -I include/SDL2 -I include/SDL2_image -I include/SDL2_ttf
LIBRARY_PATH = -L lib/SDL2 -L lib/SDL2_image -L lib/SDL2_ttf
COMPILER_FLAGS = -std=c99 -pedantic -Wall -g -fsanitize=address
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf

all:	$(BUILD_DIR) $(BUILD_DIR)/$(OBJ_NAME)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/$(OBJ_NAME): $(OBJ_FILES)
	$(CC) $(OBJ_FILES) $(LIBRARY_PATH) $(LINKER_FLAGS) -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(COMPILER_FLAGS) $(INCLUDE_PATH) -c $< -o $@

clean:
	rm -f $(BUILD_DIR)/*.o $(BUILD_DIR)/$(OBJ_NAME)