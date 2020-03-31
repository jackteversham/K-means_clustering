
PROGRAM=clusterer

#by default CC = gcc, this will give 'undefined std::xxx errors
CC=g++  
COMPILE=-c -o
LINK=-o
#      c++11     all warnings on            
#      |         |     debug info (allows debugging with gdb)
#      |         |     |  check for out-of-bounds errors
#      V         V     V  V                  /=extra info for runtime errors
FLAGS=-std=c++11 -Wall -g -fsanitize=address -fno-omit-frame-pointer
#        fast code
#        V   /=c++11    /=still tell me about all warnings
OPTIMIZE=-O3 -std=c++11 -Wall #but do not add error checking at runtime

#the default target is debug code
debug:
	$(CC) $(FLAGS) $(COMPILE) kMeansClusterer.o kMeansClusterer.cpp #compile
	$(CC) $(FLAGS) $(COMPILE) $(PROGRAM).o $(PROGRAM).cpp #compile
	$(CC) $(FLAGS) $(LINK) $(PROGRAM) $(PROGRAM).o #link
	
#and fast executable is an additional target
release:
	$(CC) $(OPTIMIZE) $(COMPILE) $(PROGRAM).o $(PROGRAM).cpp #compile for speed
	$(CC) $(OPTIMIZE) $(LINK) $(PROGRAM) $(PROGRAM).o #link

clean:
	rm -f *.o #clear out object files
	rm -f $(PROGRAM) #and the final executable
