INCLUDES=-I includes/ lib/cs225/HSLAPixel.hpp lib/cs225/PNG.hpp lib/cs225/RGB_HSL.hpp lib/lodepng/lodepng.hpp
CXXFLAGS=-std=c++20 -g $(INCLUDES)
CXX=clang++

clean:
	rm -f main

main: src/main.cpp src/network.cpp lib/cs225/HSLAPixel.cpp lib/cs225/PNG.cpp lib/lodepng/lodepng.cpp
	$(CXX) $(CXXFLAGS) $^ 
	rm -f lib/cs225/HSLAPixel.hpp.gch
	rm -f lib/cs225/PNG.hpp.gch
	rm -f lib/cs225/RGB_HSL.hpp.gch
	rm -f lib/lodepng/lodepng.hpp.gch

# tests: tests/tests.cc src/board.cc
# 	$(CXX) $(CXXFLAGS) $^ -o bin/tests


.DEFAULT_GOAL := main
# .PHONY: tests clean exec