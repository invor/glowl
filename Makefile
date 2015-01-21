
# choose your favorite compiler
#CXX=clang++
CXX=g++

# compile with all warnings and optimization
CPPFLAGS= -Wall -g -Wextra -O2 -Isrc -std=c++11 -Iinclude/glowl
LDFLAGS=
# example for LDFLAGS:
# LDFLAGS= -ldl -lglfw -lGLEW -lGL -lm -lrt -Lexternal/lib/ -Wl,-Rexternal/lib/

# find project source files (.cpp) in src/ directory
SOURCES=$(wildcard src/**/*.cpp src/*.cpp)
# make list of needed object files (.o) from source files
OBJECTS=$(patsubst %.cpp,%.o,$(SOURCES))

# name of static library file
# note: when using glowl, use linker flag '-lglowl', 
# and don't forget to include the header file from 'include/glowl/glowl.h'
TARGET=libglowl.a

all: $(OBJECTS) $(TARGET)

# build library file: compile all needed object files,
# put them in an archive (ar) and generate an index for the archive (ranlib)
$(TARGET): CPPFLAGS += -fPIC
$(TARGET): $(OBJECTS)
	ar rcs $@ $(OBJECTS)
	ranlib $@

.PHONY: clean

# clean up all currently visible objects and the lib file
clean:
	rm -rf $(OBJECTS) $(TARGET) 

# build everything from scratch by first deleting and then rebuilding the project
new: clean all

