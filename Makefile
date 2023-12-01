CC := g++
CFLAGS := -Wall -Wextra -Wpedantic -g $(shell find src -type d -printf '-I%p ')
BUILD_DIR := build
EXECUTABLE := $(BUILD_DIR)/TextBlender

SOURCES := $(shell find src -name '*.cpp')
OBJECTS := $(patsubst src/%,$(BUILD_DIR)/%,$(SOURCES:.cpp=.o))

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

$(BUILD_DIR)/%.o: src/%.cpp | $(BUILD_DIR)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

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
	make clean
	make rebuild
	make run

.PHONY: clean
