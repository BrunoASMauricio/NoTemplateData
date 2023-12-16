CC := gcc

SRC_DIR := ./src
OBJ_DIR := ./obj
EXE_DIR := ./exe

SOURCES := $(wildcard $(SRC_DIR)/*.c)
# get obj/<>.o from src/<>.c
TARGETS := $(subst $(SRC_DIR),$(OBJ_DIR),$(SOURCES) )
TARGETS := $(subst .c,.o,$(TARGETS) )

LDFLAGS := -g3
CFLAGS  := -I$(SRC_DIR) -static -g -O0

.SUFFIXES:
.PHONY: clean all

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

TARGET := $(EXE_DIR)/no_template.exe

build: clean $(TARGETS)
	$(CC) $(LDFLAGS) $(TARGETS) -o $(TARGET)

run: build $(TARGET)
	$(TARGET)

all: build

clean:
	find $(OBJ_DIR) -name "*.o" -exec rm "{}" \;
	find $(EXE_DIR) -name "*.exe" -exec rm "{}" \;

debug: build $(TARGET)
	gdb $(TARGET)

memory: build $(TARGET)
	valgrind -s --leak-check=full --track-origins=yes $(TARGET)

