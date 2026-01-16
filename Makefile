CXX := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -O2 -Iinclude
LIBS := -lvulkan -lglfw

SRC := main.cpp $(shell find src -name "*.cpp")
OBJ := $(patsubst %.cpp,build/%.o,$(SRC))

TARGET := build/app

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(LIBS)

build/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf build

rebuild: clean all
