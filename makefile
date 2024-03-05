CC = gcc
CFLAGS = -Wall -Wextra -std=c11
SDL_FLAGS = -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer -lm

# répertoires
SRC_DIR = src
INCLUDE_DIR = include
BIN_DIR = bin

# fichier source
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(BIN_DIR)/%.o, $(SRC_FILES))

TARGET = $(BIN_DIR)/game

all: $(TARGET)
	rm -f $(BIN_DIR)/*.o

$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -o $@ $^ $(SDL_FLAGS)

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c -o $@ $<

clean:
	rm -f $(BIN_DIR)/*.o $(TARGET)

.PHONY: all clean
