CXX		  := LANG=EN g++
CXX_FLAGS := -Wall -Wextra -std=c++17 -ggdb
# -Wall -Wextra
BIN		:= bin
SRC		:= src
BASE	:= src/base
INTERPRETER := src/interpreter
BYTECODE	:= src/bytecode
INCLUDE	:= include
LIB		:= lib

LIBRARIES	:=
EXECUTABLE	:= main


all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	# ./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp $(BASE)/*.cpp $(BYTECODE)/*.cpp #$(INTERPRETER)/*.cpp
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -L$(LIB) $^ -o $@ $(LIBRARIES)

clean:
	-rm $(BIN)/*
