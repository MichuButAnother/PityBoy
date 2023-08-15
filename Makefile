CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++20

SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

HEADER_DIR := $(SRC_DIR)/headers
SHARED_DIR := $(SRC_DIR)/shared
PLAYER_DIR := $(SRC_DIR)/player
EDITOR_DIR := $(SRC_DIR)/editor

SRC_PLAYER := $(wildcard $(PLAYER_DIR)/*.cpp)
SRC_EDITOR := $(wildcard $(EDITOR_DIR)/*.cpp)
SRC_SHARED := $(wildcard $(SHARED_DIR)/*.cpp)

OBJ_PLAYER := $(patsubst $(PLAYER_DIR)/%.cpp,$(OBJ_DIR)/player/%.o,$(SRC_PLAYER))
OBJ_EDITOR := $(patsubst $(EDITOR_DIR)/%.cpp,$(OBJ_DIR)/editor/%.o,$(SRC_EDITOR))
OBJ_SHARED := $(patsubst $(SHARED_DIR)/%.cpp,$(OBJ_DIR)/shared/%.o,$(SRC_SHARED))

HEADERS :=  $(wildcard $(HEADER_DIR)/*.hpp)

# cross platform idk to do
ifeq ($(OS),Windows_NT)
    LIBS :=
else 
    LIBS := -lsfml-system -lsfml-graphics -lsfml-window -llua5.4
endif


TARGET_PLAYER := $(BIN_DIR)/PityBoy
TARGET_EDITOR := $(BIN_DIR)/PityBoy_editor

.PHONY: all player editor clean

all: player editor

player: $(TARGET_PLAYER)
editor: $(TARGET_EDITOR)


$(TARGET_PLAYER): $(OBJ_PLAYER) $(OBJ_SHARED)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $^ $(LIBS) -o $@

$(TARGET_EDITOR): $(OBJ_EDITOR) $(OBJ_SHARED)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $^ $(LIBS) -o $@

$(OBJ_DIR)/player/%.o: $(PLAYER_DIR)/%.cpp $(HEADERS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/editor/%.o: $(EDITOR_DIR)/%.cpp $(HEADERS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/shared/%.o: $(SHARED_DIR)/%.cpp $(HEADERS)
	@mkdir -p $(@D)
	-$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)/ $(BIN_DIR)/