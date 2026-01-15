# ====== CONFIG ======
CXX       := g++
CXXSTD    := -std=c++20
WARNINGS  := -Wall -Wextra -Wpedantic
OPT_DEBUG := -O0 -g
OPT_REL   := -O2 -DNDEBUG

TARGET    := app
BUILD_DIR := build

# ====== LIBS ======
GLFW_CFLAGS := $(shell pkg-config --cflags glfw3)
GLFW_LIBS   := $(shell pkg-config --libs glfw3)

VULKAN_LIBS := -lvulkan

INCLUDES := -Iinclude
CXXFLAGS := $(CXXSTD) $(WARNINGS) $(GLFW_CFLAGS) $(INCLUDES)
LDFLAGS  := $(GLFW_LIBS) $(VULKAN_LIBS)

# ====== SOURCES ======
SRC_CPP  := $(shell find src -name "*.cpp")
MAIN_CPP := main.cpp

SRCS := $(MAIN_CPP) $(SRC_CPP)
OBJS := $(SRCS:%.cpp=$(BUILD_DIR)/%.o)

# ====== BUILD MODE ======
ifdef RELEASE
    CXXFLAGS += $(OPT_REL)
else
    CXXFLAGS += $(OPT_DEBUG)
endif

# ====== RULES ======
all: $(TARGET)

$(TARGET): $(OBJS)
	@echo "Linking $@"
	$(CXX) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@echo "Compiling $<"
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

run: all
	./$(TARGET)

.PHONY: all clean run
