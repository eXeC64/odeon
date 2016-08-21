.PHONY: all clean

STD = c++14
LIBS = cpr curl jsoncpp

CXXFLAGS = -W -Wall -std=$(STD) -Iinclude
LDFLAGS = $(LIBS:%=-l%)

BINS = $(patsubst src/%.m.cpp,bin/%,$(wildcard src/*.m.cpp))
SRCS = $(filter-out %.m.cpp, $(wildcard src/*.cpp))
OBJS = $(SRCS:src/%.cpp=build/%.o)

RED = \033[31m
GREEN = \033[32m
BLUE = \033[34m
WHITE = \033[37m
DEFAULT = \033[39m

all: $(BINS)

bin/%: build/%.m.o $(OBJS)
	@mkdir -p bin
	@echo -e "[*] $(GREEN)Linking $(WHITE)$@$(DEFAULT)"
	@$(CXX) -o $@ $^ $(LDFLAGS)

build/%.o: src/%.cpp
	@mkdir -p build
	@echo -e "[*] $(BLUE)Compiling $(WHITE)$^$(DEFAULT)"
	@$(CXX) $(CXXFLAGS) -o $@ -c $<

clean:
	@$(RM) bin/* build/*
