CXX := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -I. -IEngine -ICore -INetwork
BUILD_DIR := build

SRCS := example.cpp \
        Core/DummyCore.cpp \
        Engine/SimObject.cpp \
        Engine/Simulator.cpp \
        Network/src/Router.cpp \
        Network/src/TrafficGen.cpp

OBJS := $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(SRCS))
TARGET := $(BUILD_DIR)/sim

# Default target: compile binary into build/sim
all: $(TARGET)

# Run target: build (if needed) and execute simulation binary
run: $(TARGET)
	@./$(TARGET)

# Link executable
$(TARGET): $(OBJS)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@

# Compile C++ source files to object files inside build/
$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all run clean
