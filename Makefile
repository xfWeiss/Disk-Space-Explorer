TARGET = dse
TARGET_TEST = test

CC = gcc
CFLAGS = -Wall -MMD -MP -I src
NCURSES_FLAGS = -lncursesw
CTEST_FLAGS = -I thirdparty

ROOT_PATH = /usr/local/bin

SRC_DIR = src
OBJ_DIR = obj/src
BIN_DIR = bin
TEST_DIR = test
TEST_OBJ_DIR = obj/test
LIB_PATH = $(OBJ_DIR)/lib.a

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

TEST_SRC = $(wildcard $(TEST_DIR)/*.c)
TEST_OBJ = $(patsubst $(TEST_DIR)/%.c, $(TEST_OBJ_DIR)/%.o, $(TEST_SRC))

DEPS = $(OBJ:.o=.d)

all: $(BIN_DIR)/$(TARGET)

-include $(DEPS)

$(BIN_DIR)/$(TARGET): $(OBJ) $(LIB_PATH)
	$(CC) $(CFLAGS) $(OBJ) -o $@ $(NCURSES_FLAGS) 

$(LIB_PATH): $(OBJ)
	ar rcs $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@ 

install : $(BIN_DIR)/$(TARGET)
	sudo install -D -m 755 $(BIN_DIR)/$(TARGET) $(ROOT_PATH)

test: $(BIN_DIR)/$(TARGET_TEST)
	@echo "Running tests..."
	./$(BIN_DIR)/$(TARGET_TEST)

$(TEST_OBJ_DIR)/%.o: $(TEST_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(CTEST_FLAGS) -c $< -o $@

$(BIN_DIR)/$(TARGET_TEST): $(TEST_OBJ) $(LIB_PATH)
	$(CC) $(CFLAGS) $(CTEST_FLAGS) $^ -o $@ $(NCURSES_FLAGS) -lm

uninstall: 
	sudo rm $(ROOT_PATH)/$(TARGET)

clean:
	rm -rf $(OBJ_DIR)/* $(TEST_OBJ_DIR)/* $(BIN_DIR)/$(TARGET) $(BIN_DIR)/$(TARGET_TEST)

.PHONY: all clean test install
