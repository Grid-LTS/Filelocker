# TO windows check ifeq ($(OS),Windows_NT)
OS := $(shell uname)
ifeq ($(OS),Darwin)
IMPL_OS := darwin
else
IMPL_OS := linux
endif

CC := g++
CFLAGS := -Wall -std=c++17
TARGET := bin/filelocker
SRC_DIR := ${CURDIR}/src
IMPL_DIR := $(SRC_DIR)/$(IMPL_OS)
OBJ_DIR := ${CURDIR}/bin
IMPL_FILES := $(wildcard $(IMPL_DIR)/*.cpp)
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
OBJ_FILES += $(patsubst $(IMPL_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(IMPL_FILES))

all: $(TARGET)
$(TARGET): $(OBJ_FILES) 
	$(CC) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o: $(IMPL_DIR)/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(TARGET) *.o
	
.PHONY: all clean