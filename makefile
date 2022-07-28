CXX		:=	clang++
FLAGS	:=	-Wall -Wextra -std=c++17 -fdelayed-template-parsing

INCLUDE	:=	include
SRC		:=	src
BIN		:=	bin

LIBS	:=	-lglfw -lGLEW -lGLU -lGL
EXE		:=	VisionGL

DEPS	:=	$(wildcard $(SRC)/*.cpp)
OBJ		:=	$(patsubst $(SRC)/%.cpp, $(BIN)/%.o, $(DEPS))

$(BIN)/%.o: $(SRC)/%.cpp 
	mkdir -p $(BIN)
	$(CXX) -c -o $@ $< $(FLAGS) -I$(INCLUDE)

$(BIN)/$(EXE): $(OBJ)
	$(CXX) $(FLAGS) -I$(INCLUDE) -o $@ $^ $(LIBS)


run: $(BIN)/$(EXE)
		./$(BIN)/$(EXE)


start:
		clear
		./$(BIN)/$(EXE)

clean:
		rm -rf $(BIN)/*

setup:
	mkdir -p $(BIN)
	mkdir -p $(SRC)
	mkdir -p $(INCLUDE)
	touch $(SRC)/main.cpp