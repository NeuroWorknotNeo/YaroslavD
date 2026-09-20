CXX = g++
CXXFLAGS = -O2 -Wall -Wextra

all: a.out

a.out: main.o matrix.o inverse.o
	$(CXX) main.o matrix.o inverse.o -o a.out

main.o: main.cpp matrix.h inverse.h
	$(CXX) $(CXXFLAGS) -c main.cpp

matrix.o: matrix.cpp matrix.h
	$(CXX) $(CXXFLAGS) -c matrix.cpp

inverse.o: inverse.cpp inverse.h
	$(CXX) $(CXXFLAGS) -c inverse.cpp

clean:
	rm -f *.o a.out
