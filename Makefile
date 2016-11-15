CXX = g++
CXXFLAGS = -std=gnu++11 -O2 -g
LDFLAGS = 

all : simul

simul : main.o dijkstra.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

main.o : main.cpp dijkstra.h
	$(CXX) -c $(CXXFLAGS) $(LDFLAGS) -o $@ main.cpp

dijkstra.o : dijkstra.cpp dijkstra.h
	$(CXX) -c $(CXXFLAGS) $(LDFLAGS) -o $@ dijkstra.cpp

clean :
	$(RM) simul main.o dijkstra.o
