CC	    = gcc
CXX		= g++
INC_DIR = ./include
SRC_DIR = ./src
OBJ_DIR = ./obj
BIN_DIR = ./bin
TESTBIN_DIR = ./testbin

INCLUDE         += -I $(INC_DIR)
CFLAGS          += -Wall
TESTLDFLAGS     += -lgtest -lgtest_main  -lpthread
CPPFLAGS        += -std=c++14 


INCLUDE         += -I $(INC_DIR)
PROJ_NAME       = proj3
TESTMAP_NAME   = testmap

MAIN_OBJ        = $(OBJ_DIR)/main.o
TESTMAP_OBJ    = $(OBJ_DIR)/testmap.o
PROJ_OBJS       = $(OBJ_DIR)/KeyValueMap.o


all: directories test $(BIN_DIR)/$(PROJ_NAME) 
directories: $(BIN_DIR) $(OBJ_DIR) $(TESTBIN_DIR)

test: $(TESTBIN_DIR)/$(TESTMAP_NAME)
	$(TESTBIN_DIR)/$(TESTMAP_NAME)


$(BIN_DIR)/$(PROJ_NAME): $(PROJ_OBJS) $(MAIN_OBJ)
	$(CC) $(MAIN_OBJ) $(PROJ_OBJS) -o $(BIN_DIR)/$(PROJ_NAME) 

$(TESTBIN_DIR)/$(TESTMAP_NAME): $(PROJ_OBJS) $(TESTMAP_OBJ) 
	$(CXX) $(PROJ_OBJS) $(TESTMAP_OBJ) -o $(TESTBIN_DIR)/$(TESTMAP_NAME) $(CFLAGS) $(CPPFLAGS) $(DEFINES) $(TESTLDFLAGS) 

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(INCLUDE) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(INCLUDE) -c $< -o $@


$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(TESTBIN_DIR):
	mkdir -p $(TESTBIN_DIR)

clean:
	rm -rf $(BIN_DIR)
	rm -rf $(OBJ_DIR)
	rm -rf $(TESTBIN_DIR)

.PHONY: clean
