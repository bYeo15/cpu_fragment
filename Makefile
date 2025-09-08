CC := gcc
CFLAGS := -Wall -Wextra -lm -O3

SRC_DIR := src
DEMO_DIR := demos
LIB_DIR := lib
OBJ_DIR := obj
FRAG_DIR := frag

CFLAGS += -I$(LIB_DIR)

CORE_DIR := $(SRC_DIR)/core
CORE_SRC := $(notdir $(wildcard $(CORE_DIR)/*.c))

CORE_OBJ_DIR := $(OBJ_DIR)/core
CORE_OBJ := $(addprefix $(CORE_OBJ_DIR)/,$(patsubst %.c,%.o,$(CORE_SRC)))
CORE_SO := libcpufragcore.so

OPT_DIR := $(SRC_DIR)/optional
OPT_OBJ_DIR := $(OBJ_DIR)/optional

TEST_DIR := test
TEST_OUT_DIR := $(TEST_DIR)/bin
UNIT_DIR := $(TEST_DIR)/unit

FULL_UNIT := $(addprefix $(TEST_OUT_DIR)/,$(basename $(notdir $(wildcard $(UNIT_DIR)/*.c))))

TEST_FLAGS := -lcmocka -fsanitize=address,leak -g -Og


# Make core shared obj
.PHONY: core
core: $(CORE_SO)


# Make and run unit tests
.PHONY: unit
unit: $(FULL_UNIT)


# Make a demo
$(FRAG_DIR)/%: $(DEMO_DIR)/%.c $(CORE_SO) $(OPT_OBJ_DIR)/frame_png.o $(FRAG_DIR)
	$(CC) $(CFLAGS) $< $(CORE_SO) $(OPT_OBJ_DIR)/frame_png.o -o $@ -lpng -lm


# Core shared obj rule
$(CORE_SO): $(CORE_OBJ)
	ld -r $(CORE_OBJ) -o $@


# Make a single core obj file
$(CORE_OBJ_DIR)/%.o: $(CORE_DIR)/%.c $(CORE_OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@


# Make a single optional obj file
$(OPT_OBJ_DIR)/%.o: $(OPT_DIR)/%.c $(OPT_OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@


# Make core tests TODO
$(TEST_OUT_DIR)/%: $(UNIT_DIR)/%.c $(TEST_OUT_DIR) $(CORE_SO)
	$(CC) $(CFLAGS) $(TEST_FLAGS) $< $(CORE_OBJ) -o $@


# Directories
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)


$(CORE_OBJ_DIR): $(OBJ_DIR)
	@mkdir -p $(CORE_OBJ_DIR)


$(OPT_OBJ_DIR): $(OBJ_DIR)
	@mkdir -p $(OPT_OBJ_DIR)


$(FRAG_DIR):
	@mkdir -p $(FRAG_DIR)


$(TEST_OUT_DIR):
	@mkdir -p $(TEST_OUT_DIR)


# Specific optional components
png: $(OPT_OBJ_DIR)/frame_png.o


.PHONY: clean
clean:
	@echo "Cleaning up object and executable files"
	@if [[ -d "$(OBJ_DIR)" ]]; then rm -r $(OBJ_DIR); fi
	@if [[ -d "$(TEST_OUT_DIR)" ]]; then rm -r $(TEST_OUT_DIR); fi
	@if [[ -d "$(FRAG_DIR)" ]]; then rm -r $(FRAG_DIR); fi
	@if [[ -e "$(CORE_SO)" ]]; then rm $(CORE_SO); fi
	@rm -f *.png
