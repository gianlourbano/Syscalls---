func =

CXX = gcc
CXXFLAGS = -std=gnu99 -O3 -g -Isrc/ -Iincludes/ -DFUNC_NAME=$(func)

SRC_DIR = src
BUILD_DIR = build
EXEC_DIR = bin
TARGET = $(func)
SCRIPTS_DIR = scripts

ALL_FILES := $(wildcard $(SRC_DIR)/**/**/*.c) $(wildcard $(SRC_DIR)/**/*.c) $(wildcard $(SRC_DIR)/*.c)
ALL_HEADER_FILES := $(wildcard $(SRC_DIR)/**/**/*.h) $(wildcard $(SRC_DIR)/**/*.h) $(wildcard $(SRC_DIR)/*.h)

ALL_OBJ_FILES := $(ALL_FILES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
OBJ_FOLDERS:= $(addprefix $(BUILD_DIR)/, $(dir $(ALL_FILES:$(SRC_DIR)/%=%)))

.PHONY: all clean

all: $(ALL_OBJ_FILES)
	$(CXX) $(CXXFLAGS) -o $(EXEC_DIR)/$(TARGET) $^

$(ALL_OBJ_FILES): $(BUILD_DIR)/%.o : $(SRC_DIR)/%.c | $(OBJ_FOLDERS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_FOLDERS):
	mkdir -p $@

type =
name =
desc =

create:
	python3 create.py "$(type)" "$(name)" "$(desc)"

main.o: src/main.c $(ALL_HEADER_FILES)
	rm -rf build/main.o
	$(CXX) $(CXXFLAGS) -c -o build/main.o src/main.c

options = 
run: main.o all
	./$(EXEC_DIR)/$(TARGET) $(options)

script =

run-script:
	@echo "\n"
	python3 $(SCRIPTS_DIR)/$(script).py $(options)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)
