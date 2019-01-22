CC=clang
CPPCHECK=C:\Program Files\Cppcheck\cppcheck.exe
CFLAGS=-Wall -Iinclude -Iinclude/glad -Wno-pragma-pack
LDFLAGS= -L. -lSDL2
BINARY_DRAW=draw
BINARY_TESTS=opengl_tests

ifeq ($(OS),Windows_NT)	
	BINARY_DRAW:=$(BINARY_DRAW).exe
	BINARY_TESTS:=$(BINARY_TESTS).exe
endif

all: draw

draw: draw.o glad.o parser.o rasterizer.o aiv_math.o
	$(CC) -o $(BINARY_DRAW) $(LDFLAGS) $^

draw.o: draw.c
	$(CC) -c -o $@ $(CFLAGS) $^
	$(CPPCHECK) $^

glad.o: glad.c
	$(CC) -c -o $@ $(CFLAGS) $^
	$(CPPCHECK) $^

parser.o: parser.c parser.h
	$(CC) -c -o $@ $(CFLAGS) $<
	$(CPPCHECK) $^

rasterizer.o: rasterizer.c rasterizer.h
	$(CC) -c -o $@ $(CFLAGS) $<
	$(CPPCHECK) $^

aiv_math.o: aiv_math.c aiv_math.h
	$(CC) -c -o $@ $(CFLAGS) $<
	$(CPPCHECK) $^

tests.o: tests.c
	$(CC) -c -o $@ $(CFLAGS) $^
	$(CPPCHECK) $^
