CXX := g++
CXXWARNINGS := -Wall -Wextra -Werror
CXXSTD := -std=c++17
CXXOPT := -O3
INCLUDES := -I include -I external/include -I /opt/apps/root/include

CXXFLAGS = $(CXXWARNINGS) $(CXXSTD) $(CXXOPT) $(INCLUDES)

LDFLAGS = $(shell root-config --libs) -Wl,-rpath,$(shell root-config --libdir)


# Exclude corupt_data.cxx because it fails to compile
SRC := $(filter-out src/corupt_data.cxx src/temp_analysis.cxx src/main_minmax.cxx src/test.cxx, $(wildcard src/*.cxx))
OBJ := $(SRC:.cxx=.o)


all: main

main: $(OBJ) main.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f src/*.o main.o main

.PHONY: all clean
