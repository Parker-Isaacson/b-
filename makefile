CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -g

TARGET   := main
BUILD    := build
SRC      := src

SOURCES  := $(SRC)/main.cpp $(SRC)/utils/chess.cpp
OBJECTS  := $(SOURCES:$(SRC)/%.cpp=$(BUILD)/%.o)

# Default target
all: $(TARGET)

# Link the executable (placed in project root)
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile source files into build/, mirroring src/ structure
$(BUILD)/%.o: $(SRC)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD) $(TARGET)

run:
	./$(TARGET)

rebuild: clean all

.PHONY: all clean rebuild
