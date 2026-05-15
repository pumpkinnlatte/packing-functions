# Makefile - packing functions
#
# Targets principales:
#   make              compila todo
#   make test         corre todos los tests con resumen
#   make test_arith   corre solo test_arith
#   make test_gtp     corre solo test_gtp
#   make test_dp      corre solo test_dp
#   make test_bp      corre solo test_bp
#   make demo         ejecuta examples/demo
#   make demo_gtp     ejecuta examples/demo_gtp
#   make demo_dp      ejecuta examples/demo_dp
#   make demo_bp      ejecuta examples/demo_bp
#   make cli ARGS=    ejecuta demo_cli (ej: make cli ARGS="dp encode 2 3")
#   make debug        compila con sanitizers
#   make clean        elimina build/

CC      ?= cc
CFLAGS  ?= -std=c11 -Wall -Wextra -Wpedantic -Wshadow -O2
CPPFLAGS = -Iinclude
LDFLAGS  = -lm

ifeq ($(MODE),debug)
    CFLAGS  += -g -O0 -fsanitize=address,undefined
    LDFLAGS += -fsanitize=address,undefined
endif

SRC_DIR     = src
TEST_DIR    = tests
EXAMPLE_DIR = examples
BUILD_DIR   = build

LIB_SRC     = $(wildcard $(SRC_DIR)/*.c)
LIB_OBJ     = $(LIB_SRC:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

TEST_SRC    = $(wildcard $(TEST_DIR)/*.c)
TEST_BIN    = $(TEST_SRC:$(TEST_DIR)/%.c=$(BUILD_DIR)/%)

EXAMPLE_SRC = $(wildcard $(EXAMPLE_DIR)/*.c)
EXAMPLE_BIN = $(EXAMPLE_SRC:$(EXAMPLE_DIR)/%.c=$(BUILD_DIR)/%)

.PHONY: all test clean debug
.PHONY: test_arith test_gtp test_dp test_bp
.PHONY: demo demo_gtp demo_dp demo_bp cli

all: $(TEST_BIN) $(EXAMPLE_BIN)

# -- Compilacion --

$(BUILD_DIR)/%: $(TEST_DIR)/%.c $(LIB_OBJ) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) $< $(LIB_OBJ) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%: $(EXAMPLE_DIR)/%.c $(LIB_OBJ) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) $< $(LIB_OBJ) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# -- Tests --

# Corre todos los tests con resumen final.
test: $(TEST_BIN)
	@echo ""
	@echo "============================================================"
	@echo "  Ejecutando tests"
	@echo "============================================================"
	@total=0; passed=0; failed=0; \
	for t in $(TEST_BIN); do \
		name=$$(basename $$t); \
		total=$$((total + 1)); \
		printf "  %-20s ... " "$$name"; \
		if $$t > /tmp/pf_test_out 2>&1; then \
			passed=$$((passed + 1)); \
			echo "OK"; \
		else \
			failed=$$((failed + 1)); \
			echo "FAIL"; \
			cat /tmp/pf_test_out | sed 's/^/    /'; \
		fi; \
	done; \
	echo "============================================================"; \
	echo "  Resumen: $$passed/$$total OK, $$failed FAIL"; \
	echo "============================================================"; \
	test $$failed -eq 0

# Tests individuales.
test_arith: $(BUILD_DIR)/test_arith
	@$(BUILD_DIR)/test_arith

test_gtp: $(BUILD_DIR)/test_gtp
	@$(BUILD_DIR)/test_gtp

test_dp: $(BUILD_DIR)/test_dp
	@$(BUILD_DIR)/test_dp

test_bp: $(BUILD_DIR)/test_bp
	@$(BUILD_DIR)/test_bp

# -- Demos --

demo: $(BUILD_DIR)/demo
	@$(BUILD_DIR)/demo

demo_gtp: $(BUILD_DIR)/demo_gtp
	@$(BUILD_DIR)/demo_gtp

demo_dp: $(BUILD_DIR)/demo_dp
	@$(BUILD_DIR)/demo_dp

demo_bp: $(BUILD_DIR)/demo_bp
	@$(BUILD_DIR)/demo_bp

# make cli ARGS="dp encode 2 3"
cli: $(BUILD_DIR)/p-functions
	@$(BUILD_DIR)/p-functions $(ARGS)

# -- Utiles --

debug:
	$(MAKE) MODE=debug

clean:
	rm -rf $(BUILD_DIR)
	@rm -f /tmp/pf_test_out
