SRC_DIR = src
BIN_DIR = bin
BUILD_DIR = build
OUTPUT_DIR = output

CC = gcc
WARNINGS = -Wall -Wextra -Wshadow
CFLAGS = -std=c2x $(WARNINGS) -O3 -march=native -fopenmp -flto -Iinclude -MMD -MP -g
LDLIBS = -lm

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))
DEPS = $(OBJS:.o=.d)

TARGET = $(BIN_DIR)/fractals

.PHONY: all clean

all: $(TARGET) $(OUTPUT_DIR)

$(TARGET): $(OBJS) | $(BIN_DIR)
	@$(CC) $(CFLAGS) $^ $(LDLIBS) -o $@
	$(info Linked $@.)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@
	$(info Compiled $@.)

$(BIN_DIR) $(BUILD_DIR) $(OUTPUT_DIR):
	@mkdir -p $@
	$(info Created output directories.)

clean:
	@rm -rf $(BIN_DIR) $(BUILD_DIR) $(OUTPUT_DIR)
	$(info Removed output directories.)

-include $(DEPS)