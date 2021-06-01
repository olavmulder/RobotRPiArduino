CC = g++ #g++ compiler
CFLAGS = -Wall -Wconversion -Wextra  -ansi -std=c++11 -Wno-psabi -g #set all flags and use c++11
SOURCE= main.cpp 
HEADER =$(shell find  header/ -type f -iwholename 'header/*.h')#get all header files
SOURCE =$(shell find src/ -type f -iwholename 'src/*.cpp')#get all header files

ALLFILES = $(HEADER) $(SOURCE)
TARGET= main #set name for executable

#$(TARGET): $(SOURCES) $(HEADERCAR) $(SOURCECAR) $(SOURCESTRAFFICLIGHT) $(HEADERTRAFFICLIGHT) 	#for the target get all source and header fiels
	#compile with g++ and flags all de source and header file than output is target = main, lm flag
#	$(CC) $(CFLAGS) $^  -o $@ -lm 
$(TARGET): main.cpp $(ALLFILES)
	#compile with g++ and flags all de source and header file than output is target = main, lm flag
	$(CC) $(CFLAGS) $^  -o $@ -lwiringPi

clean:	 #remove target file, the executable
	rm -f $(TARGET)