CXX := g++ #Uses g++ as compilar
CXXWARNINGS := -Wall -Wextra -Werror #Does so it gives warning flags and treats them as errors
CXXSTD := -std=c++17
CXXOPT := -O3 #chosese optimization level
INCLUDES := -I include -I /opt/apps/root/include #So it has the proper includes

CXXFLAGS = $(CXXWARNINGS) $(CXXSTD) $(CXXOPT) $(INCLUDES) #Writing all above into one variable 

LDFLAGS = $(shell root-config --libs) -Wl,-rpath,$(shell root-config --libdir) #Flags for the linker


# Exclude corupt_data.cxx because it fails to compile
SRC := $(filter-out src/corupt_data.cxx, $(wildcard src/*.cxx)) #Builds list of the source files to compile
OBJ := $(SRC:.cxx=.o) #This will be the object file names


all: main #Default target is all, which depends on main

main: $(OBJ) main.o #Here define main, depends on all object files and main.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cxx #compiles the .cxx files without linking it yet
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

clean: #removs the .o files
	rm -f src/*.o main.o main

.PHONY: all clean #does so all and clean are phony targets
