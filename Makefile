CXX = g++
CXXFLAGS = -lGL -lGLU -lglut 

IPATH1 = ./dependencies/freeglut/include
IPATH2 = ./dependencies
LPATH1 = ./dependencies/freeglut.lib

all: WarAndPeace

WarAndPeace: main.o war.o peace.o stb_image.o 
	$(CXX) "-I$(IPATH1)" "-I$(IPATH2)" "-L$(LPATH1)" -o ./bin/WarAndPeace ./bin/main.o ./bin/war.o ./bin/peace.o ./bin/stb/stb_image.o $(CXXFLAGS)

main.o: ./src/main.cpp
	$(CXX) "-I$(IPATH1)" "-I$(IPATH2)" -c -o ./bin/main.o  ./src/main.cpp

war.o: ./src/war.cpp
	$(CXX) "-I$(IPATH1)" "-I$(IPATH2)" -c -o ./bin/war.o ./src/war.cpp
	
peace.o: ./src/peace.cpp
	$(CXX) "-I$(IPATH1)" "-I$(IPATH2)" -c -o ./bin/peace.o ./src/peace.cpp
	
stb_image.o: ./dependencies/stb/stb_image.cpp
	$(CXX) "-I$(IPATH1)" "-I$(IPATH2)" -c -o ./bin/stb/stb_image.o ./dependencies/stb/stb_image.cpp

clean:
	rm ./bin/* ./bin/stb/*.o
