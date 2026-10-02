CXX ?= g++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -m32
CPPFLAGS ?= -Iinclude
TARGET = ra2cpp.exe
TEST_TARGET = tests/run_tests.exe

SRC_DIR := src
TEST_DIR := tests
INCLUDE_DIR := include

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
TEST_SRCS := $(wildcard $(TEST_DIR)/*.cpp)
CORE_SRCS := $(wildcard $(INCLUDE_DIR)/*.cpp)

OBJS := $(SRCS:%.cpp=%.o) $(CORE_SRCS:%.cpp=%.o)
TEST_OBJS := $(TEST_SRCS:%.cpp=%.o)

all: $(TARGET)

$(TARGET): $(SRCS) $(CORE_SRCS)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(SRCS) $(CORE_SRCS) -o $@

$(TEST_TARGET): $(TEST_SRCS) $(CORE_SRCS)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(TEST_SRCS) $(CORE_SRCS) -o $@

tests: $(TEST_TARGET)

clean:
	rm -f $(TARGET) $(TEST_TARGET) $(SRCS:%.cpp=%.o) $(CORE_SRCS:%.cpp=%.o) $(TEST_SRCS:%.cpp=%.o)

.PHONY: all clean tests
