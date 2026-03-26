CXX = g++
CXXFLAGS = -O2 -fopenmp

TARGET = program

all: $(TARGET)

$(TARGET): main.o correlate.o
	$(CXX) $(CXXFLAGS) -o $(TARGET) main.o correlate.o

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

correlate.o: correlate.cpp
	$(CXX) $(CXXFLAGS) -c correlate.cpp

clean:
	rm -f *.o $(TARGET)
