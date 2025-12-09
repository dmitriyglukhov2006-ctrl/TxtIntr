CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -lm
TARGET = calculator
SOURCES = main.cpp

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)

.PHONY: clean