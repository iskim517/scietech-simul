CXX = g++
CXXFLAGS = -std=gnu++11 -O2 -g

all : simul

simul : main.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

clean :
	$(RM) simul
