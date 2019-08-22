SHELL = /bin/bash

R = \033[0;31m
G = \033[0;32m
B = \033[0;34m
Y = \033[1;33m
RC = \033[0m
ECHO_OK = echo -e "> $(G)OK$(RC)."

ROOT = .
PYTHON = python3
CXX = clang++
BUILD_DIR = $(ROOT)/build
INC_DIR = $(ROOT)/inc
SRC_DIR = $(ROOT)/src
TEST_DIR = $(ROOT)/test
BIN_DIR = $(BUILD_DIR)/binary
OBJ_DIR = $(BUILD_DIR)/object
SHARED_LIB = $(BIN_DIR)/libalg.so
TEST_EXE = $(BIN_DIR)/test

MK_BIN_DIR = mkdir -p $(BIN_DIR)
MK_OBJ_DIR = mkdir -p $(OBJ_DIR)

TEST_FILES = $(shell ls -d $(TEST_DIR)/*_test.cpp)
SRC_FILES = $(shell ls -d $(SRC_DIR)/*.cpp)
OBJ_FILES = $(SRC_FILES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

CXXFLAGS = -std=c++1z -xc++ -Wall -Werror -I$(INC_DIR)
TEST_FLAGS = $(CXXFLAGS) \
			 -Wl,-rpath,'$$ORIGIN' \
			 -L$(BIN_DIR) \
			 -l$(SHARED_LIB:$(BIN_DIR)/lib%.so=%) \
			 -lgtest\
			 -lgtest_main

TEST_TARGET ?= all

$(SHARED_LIB): $(OBJ_FILES)
	@echo -e "$(B)Linking: $(G)$^$(RC) -> $(G)$@$(RC)..."
	@$(MK_BIN_DIR)
	@$(CXX) -shared -fPIC $^ -o $@
	@$(ECHO_OK)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo -e "$(B)Building: $(G)$<$(RC) -> $(G)$@$(RC)..."
	@$(MK_OBJ_DIR)
	@mkdir -p $(OBJ_DIR)
	@$(CXX) $(CXXFLAGS) -fPIC -c $< -o $@
	@$(ECHO_OK)

$(TEST_EXE): $(SHARED_LIB) $(TEST_FILES)
	@$(MK_BIN_DIR)
ifeq ($(TEST_TARGET),all)
	@echo -e "$(B)Building: $(G)$(TEST_FILES)$(RC) -> $(G)$(TEST_EXE)$(RC)..."
	@$(CXX) $(TEST_FLAGS) $(TEST_DIR)/*_test.cpp -o $(TEST_EXE)
else
	@echo -e "$(B)Building: $(G)$(TEST_TARGET)_test.cpp$(RC) -> $(G)$(TEST_EXE)$(RC)..."
	@$(CXX) $(TEST_FLAGS) $(TEST_DIR)/$(TEST_TARGET)_test.cpp -o $(TEST_EXE)
endif

.PHONY: lib
lib: $(SHARED_LIB)

.PHONY: test
test: $(TEST_EXE)

.PHONY: runtest
runtest: $(TEST_EXE)
	@$(TEST_EXE)

.PHONY: clean
clean:
	@echo -e "$(B)Cleaning...$(RC)"
	@rm -r $(BUILD_DIR)
	@$(ECHO_OK)
