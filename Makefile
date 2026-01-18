CXX := g++

MODE ?= debug
TARGET := build/app

COMMON_FLAGS := -Iinclude -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion

DEBUG_FLAGS := -std=c++20 -Og -g3 \
	-fsanitize=address,undefined \
	-fno-omit-frame-pointer \
	-fstack-protector-strong -D_FORTIFY_SOURCE=3

RELEASE_FLAGS := -std=c++20 -O2 \
	-fstack-protector-strong -D_FORTIFY_SOURCE=3 \
	-Wl,-z,relro,-z,now

ifeq ($(MODE),debug)
	CXXFLAGS := $(COMMON_FLAGS) $(DEBUG_FLAGS)
	LDFLAGS  := -fsanitize=address,undefined
else
	CXXFLAGS := $(COMMON_FLAGS) $(RELEASE_FLAGS)
	LDFLAGS  :=
endif

LIBS := -lvulkan -lglfw

SRC := main.cpp $(shell find src -name "*.cpp")
OBJ := $(patsubst %.cpp,build/%.o,$(SRC))

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) $(LDFLAGS) $(LIBS) -o $@

build/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf build

rebuild: clean all
