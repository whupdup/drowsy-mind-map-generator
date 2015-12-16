CC=g++
CFLAGS=-DSFML_STATIC -I lib\SFML
LFLAGS=-static -L lib/SFML/bin -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lsfml-main -lwinmm -lopengl32 -lgdi32

all: bin/drowsymapper.exe

run: bin/drowsymapper.exe
	./bin/drowsymapper.exe

bin/drowsymapper.exe: bin/main.o bin/reader.o
	$(CC) -L lib/SFML/bin bin/main.o bin/reader.o -o bin/drowsymapper.exe $(LFLAGS)

bin:
	mkdir bin

bin/main.o: main.cpp reader/reader.hpp bin
	$(CC) -c main.cpp -o bin/main.o $(CFLAGS)

bin/reader.o: reader/reader.cpp reader/reader.hpp bin
	$(CC) -c reader/reader.cpp -o bin/reader.o $(CFLAGS)
