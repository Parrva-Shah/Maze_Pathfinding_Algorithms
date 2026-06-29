CXX      = g++
CXXFLAGS = -O2 -std=c++17
LDFLAGS  = -lsfml-graphics -lsfml-window -lsfml-system
TARGET   = maze_visualizer
SRCS     = $(wildcard *.cpp)

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all run clean
