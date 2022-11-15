INCLUDES=-I includes/
CXXFLAGS=-std=c++20 -g $(INCLUDES)
CXX=clang++

clean:
	rm -f bin/*

main: src/main.cpp src/network.cpp
	$(CXX) $(CXXFLAGS) $^ -o bin/main

# tests: tests/tests.cc src/board.cc
# 	$(CXX) $(CXXFLAGS) $^ -o bin/tests

# src/%.cc: includes/%.hpp
# 	touch $@

.DEFAULT_GOAL := main
# .PHONY: tests clean exec