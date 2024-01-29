CC=gcc
CFLAGS=-Wno-deprecated-declarations -Wno-return-type
SRC_DIR=src
BUILD_DIR=build

# List of source files
SOURCES := $(wildcard $(SRC_DIR)/*.c)
# Replace source directory with build directory and change extension to .o
OBJECTS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SOURCES))

all: simulation run

v:
	valgrind ./simulation

run:
	./simulation

# Change the target name to 'simulation'
simulation: $(OBJECTS)
	$(CC) -o $@ $^ -lpthread

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(BUILD_DIR)/*.o
	rm -f simulation
