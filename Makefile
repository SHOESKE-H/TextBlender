CC := g++
CFLAGS := -Wall -Wextra -g
LDFLAGS := -lsodium
BUILD_DIR := build
EXECUTABLE := $(BUILD_DIR)/TextBlender

SOURCES := $(shell find src -name '*.cpp')
OBJECTS := $(patsubst src/%,$(BUILD_DIR)/%,$(SOURCES:.cpp=.o))
DIRS := $(dir $(OBJECTS))
VPATH := $(shell find src -type d)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LDFLAGS) -o $@

$(BUILD_DIR)/%.o: src/%.cpp | $(BUILD_DIR)
	mkdir -p $(dir $@)
	$(CC) -I src $(CFLAGS) -c $< -o $@

build: $(EXECUTABLE)

clean:
	rm -rf $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)

rebuild: 
	make clean
	make

run:
	./$(EXECUTABLE)

rebrun:
	make rebuild
	make run

.PHONY: clean
