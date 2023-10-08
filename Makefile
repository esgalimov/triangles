CXX ?= g++

CXXFLAGS ?= -std=c++17 -O2

triangles.o: source/triangles.cpp include/triangles.hpp include/geometry.hpp include/double_funcs.hpp
	$(CXX) $(CXXFLAGS) source/triangles.cpp -o $@

.PHONY: clean
clean:
	rm -rf *.o
