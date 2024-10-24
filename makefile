#Make file for projects quick start utility

# Compiler variable
CC = g++
CFLAGS = -Wall -g -std=c++23
LDFLAGS = 

# Source files (all .cpp files in the directory)
SRCS = $(wildcard *.cpp */*.cpp)

# Object files (change .cpp to .o)
OBJS = $(SRCS:.cpp=.o)

# Executable name
EXEC = project.bin

# All target
all: deepclean $(EXEC) memclean 

# Compile source
compile: memclean $(EXEC)

# Link the object files to create the executable
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

# Compile each .cpp file to .o
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up object files and executable
memclean:
	rm -rf */*.o

deepclean: memclean
	rm -rf $(EXEC)

install: 
	cp ./${EXEC} /usr/bin/project

# Phony targets
.PHONY: all compile memclean deepclean install

