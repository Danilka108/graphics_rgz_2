.PHONY: all, clean, rebuild, extra

CC = g++
CFLAGS = -c
GLFLAGS = -lglut -lGL -lGLU

BUILD_PATH = ../build/
SOURCES =  main.cpp
OBJECTS = $(SOURCES:.cpp=.o)

EXECUTABLE = MAIN

all: $(OBJECTS)
	$(CC) $(BUILD_PATH)$(OBJECTS) -o $(BUILD_PATH)$(EXECUTABLE) $(GLFLAGS)


rebuild: clean all

re: rebuild

%.o: %.cpp
	$(CC) $(CFLAGS) $< -o $(BUILD_PATH)$@

clean:
	-rm  $(BUILD_PATH)*
