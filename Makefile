CXX ?= g++

CXXFLAGS ?= -std=c++17 -O2

triangles.o: triangles/triangles.cpp triangles/triangles.hpp
	$(CXX) $(CXXFLAGS) triangles/triangles.cpp -o $@

.PHONY: clean
clean:
	rm -rf *.o
