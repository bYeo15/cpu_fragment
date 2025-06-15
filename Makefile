CC := gcc
CFLAGS := -Wall -Wextra -lm

SRC_DIR := src
LIB_DIR := lib
OBJ_DIR := obj

FULL_SRC := $(notdir $(wildcard $(SRC_DIR)/*.c))
FULL_OBJ := $(addprefix $(OBJ_DIR)/,$(patsubst %.c,%.o,$(FULL_SRC)))

TEST_DIR := test
TEST_OUT_DIR := $(TEST_DIR)/bin
UNIT_DIR := $(TEST_DIR)/unit

FULL_UNIT := $(addprefix $(TEST_OUT_DIR)/,$(basename $(notdir $(wildcard $(UNIT_DIR)/*.c))))

TEST_FLAGS := -lcmocka -fsanitize=address,leak -g -Og


.PHONY: all
all: $(FULL_OBJ)


.PHONY: unit
unit: $(FULL_UNIT)


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@ 


$(TEST_OUT_DIR)/%: $(UNIT_DIR)/%.c $(TEST_OUT_DIR) all
	$(CC) $(CFLAGS) $(TEST_FLAGS) $< $(FULL_OBJ) -o $@


$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)


$(TEST_OUT_DIR):
	@mkdir -p $(TEST_OUT_DIR)


.PHONY: clean
clean:
	@echo "Cleaning up object and executable files"
	@if [[ -d "$(OBJ_DIR)" ]]; then rm -r $(OBJ_DIR); fi
	@if [[ -d "$(TEST_OUT_DIR)" ]]; then rm -r $(TEST_OUT_DIR); fi
