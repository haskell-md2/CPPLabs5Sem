CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra
LDFLAGS := 

SRC_DIR := ./src
BUILD_DIR := ./build
OBJ_DIR := $(BUILD_DIR)/obj
PLUGINS_DIR := $(BUILD_DIR)/plugins
TARGET := $(BUILD_DIR)/calс

SRCS := $(shell find $(SRC_DIR) -name "*.cpp" | grep -v "/plugins/")
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

PLUGIN_SRCS := $(shell find $(SRC_DIR)/plugins -name "*.cpp" 2>/dev/null || true)

PLUGIN_TARGETS := $(PLUGIN_SRCS:$(SRC_DIR)/plugins/%.cpp=$(PLUGINS_DIR)/%.so)
ifeq ($(OS),Windows_NT)
    PLUGIN_TARGETS := $(PLUGIN_SRCS:$(SRC_DIR)/plugins/%.cpp=$(PLUGINS_DIR)/%.dll)
else
    PLUGIN_TARGETS := $(PLUGIN_SRCS:$(SRC_DIR)/plugins/%.cpp=$(PLUGINS_DIR)/%.so)
endif

all: $(TARGET) $(PLUGIN_TARGETS)

$(TARGET): $(OBJS) | $(BUILD_DIR)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(PLUGINS_DIR)/%.so: $(SRC_DIR)/plugins/%.cpp | $(PLUGINS_DIR)
	@echo "Компиляция плагина: $*"
	$(CXX) -shared -fPIC $(CXXFLAGS) $< -o $@
	@echo "Собран плагин: $@"

$(PLUGINS_DIR)/%.dll: $(SRC_DIR)/plugins/%.cpp | $(PLUGINS_DIR)
	@echo "Компиляция плагина: $*"
	$(CXX) -shared $(CXXFLAGS) $< -o $@
	@echo "Собран плагин: $@"

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(PLUGINS_DIR):
	mkdir -p $(PLUGINS_DIR)

plugins: $(PLUGIN_TARGETS)

clean:
	rm -rf $(BUILD_DIR)

rebuild: clean all

.PHONY: all clean rebuild plugins

run: $(TARGET) $(PLUGIN_TARGETS)
	cd $(BUILD_DIR) && ./calculator