CC=g++
CFLAGS=
LFLAGS=-static -lgdi32

all: bin/drowsymapper.exe

run: bin/drowsymapper.exe
	./bin/drowsymapper.exe

bin/drowsymapper.exe: bin/main.o bin/reader.o bin/mapmeasure.o bin/window.o
	$(CC) -L lib/SFML/bin bin/main.o bin/mapmeasure.o bin/reader.o bin/window.o -o bin/drowsymapper.exe $(LFLAGS)

bin:
	mkdir bin

bin/main.o: main.cpp reader/reader.hpp bin
	$(CC) -c main.cpp -o bin/main.o $(CFLAGS)

bin/window.o: graphics/window.hpp graphics/window.cpp bin
	$(CC) -c graphics/window.cpp -o bin/window.o $(CFLAGS)

bin/mapmeasure.o: graphics/mapmeasure.hpp graphics/mapmeasure.cpp bin
	$(CC) -c graphics/mapmeasure.cpp -o bin/mapmeasure.o $(CFLAGS)

bin/reader.o: reader/reader.cpp reader/reader.hpp bin
	$(CC) -c reader/reader.cpp -o bin/reader.o $(CFLAGS)
