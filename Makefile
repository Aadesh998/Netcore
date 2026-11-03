CXX      ?= g++
CXXFLAGS ?= -std=c++20 -Wall -Wextra -O2
CPPFLAGS += -Iinclude

BUILD_DIR := build
BIN_DIR   := $(BUILD_DIR)/bin
OBJ_DIR   := $(BUILD_DIR)/obj

SERVER_SRCS := $(wildcard src/server/*.cpp)
CLIENT_SRCS := $(wildcard src/client/*.cpp)

SERVER_OBJS := $(SERVER_SRCS:src/%.cpp=$(OBJ_DIR)/%.o)
CLIENT_OBJS := $(CLIENT_SRCS:src/%.cpp=$(OBJ_DIR)/%.o)

.PHONY: all server client clean

all: server client

server: $(BIN_DIR)/server
client: $(BIN_DIR)/client

$(BIN_DIR)/server: $(SERVER_OBJS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BIN_DIR)/client: $(CLIENT_OBJS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: src/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

-include $(SERVER_OBJS:.o=.d) $(CLIENT_OBJS:.o=.d)
