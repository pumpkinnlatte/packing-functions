# Makefile - packing functions
#
# Targets:
#   make            compila biblioteca, tests y ejemplos
#   make test       corre todos los tests
#   make demo       compila y ejecuta examples/demo
#   make debug      compila con sanitizers
#   make clean      elimina build/

CC      ?= cc
CFLAGS  ?= -std=c11 -Wall -Wextra -Wpedantic -Wshadow -O2
CPPFLAGS = -Iinclude
LDFLAGS  =

ifeq ($(MODE),debug)
    CFLAGS  += -g -O0 -fsanitize=address,undefined
    LDFLAGS += -fsanitize=address,undefined
endif

SRC_DIR     = src
TEST_DIR    = tests
EXAMPLE_DIR = examples
BUILD_DIR   = build

LIB_SRC      = $(wildcard $(SRC_DIR)/*.c)
LIB_OBJ      = $(LIB_SRC:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

TEST_SRC     = $(wildcard $(TEST_DIR)/*.c)
TEST_BIN     = $(TEST_SRC:$(TEST_DIR)/%.c=$(BUILD_DIR)/%)

EXAMPLE_SRC  = $(wildcard $(EXAMPLE_DIR)/*.c)
EXAMPLE_BIN  = $(EXAMPLE_SRC:$(EXAMPLE_DIR)/%.c=$(BUILD_DIR)/%)

.PHONY: all test demo clean debug

all: $(TEST_BIN) $(EXAMPLE_BIN)

$(BUILD_DIR)/%: $(TEST_DIR)/%.c $(LIB_OBJ) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) $< $(LIB_OBJ) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%: $(EXAMPLE_DIR)/%.c $(LIB_OBJ) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) $< $(LIB_OBJ) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

test: $(TEST_BIN)
	@for t in $(TEST_BIN); do echo "=== $$t ==="; $$t || exit 1; done

demo: $(BUILD_DIR)/demo
	@$(BUILD_DIR)/demo

debug:
	$(MAKE) MODE=debug

clean:
	rm -rf $(BUILD_DIR)