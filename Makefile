CXX := g++
CXXFLAGS := -Wall -Wextra -Iinclude
LDFLAGS := -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

BUILD_DIR := build
EXECUTABLE := $(BUILD_DIR)/game
SRC_DIR := src
OBJ_DIR := $(BUILD_DIR)/obj

SOURCES := $(SRC_DIR)/main.cpp $(SRC_DIR)/scene.cpp $(SRC_DIR)/scenemanager.cpp
OBJECTS := $(patsubst $(SRC_DIR)/%, $(OBJ_DIR)/%, $(SOURCES:.cpp=.o))

all: $(EXECUTABLE)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(EXECUTABLE): $(OBJ_DIR) $(OBJECTS) | $(BUILD_DIR)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

run: $(EXECUTABLE)
	./$(EXECUTABLE)

.PHONY: all clean run
