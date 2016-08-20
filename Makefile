.PHONY: clean run

CXXFLAGS = -W -Wall -std=c++14
LDFLAGS = -lcpr -lcurl -ljsoncpp -ltermbox

SRCS = main.cpp odeon.cpp cli.cpp
OBJS = $(addprefix build/, $(SRCS:.cpp=.o))

TARGET = odeon

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) 

build/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

clean:
	$(RM) $(TARGET) $(OBJS)

run: $(TARGET)
	./$(TARGET)
