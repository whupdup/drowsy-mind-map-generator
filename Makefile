all: bin/drowsymapper.exe

run: bin/drowsymapper.exe
	./bin/drowsymapper.exe

bin/drowsymapper.exe: bin/main.o bin/reader.o
	g++ bin/main.o bin/reader.o -static -o bin/drowsymapper.exe

bin:
	mkdir bin

bin/main.o: main.cpp reader/reader.hpp bin
	g++ -c main.cpp -o bin/main.o

bin/reader.o: reader/reader.cpp reader/reader.hpp bin
	g++ -c reader/reader.cpp -o bin/reader.o
