all: bin/drowsymapper.exe

run: bin/drowsymapper.exe
	./bin/drowsymapper.exe

bin/drowsymapper.exe: bin/main.o bin/reader.o
	g++ bin/main.o bin/reader.o -static -o bin/drowsymapper.exe

main.o: main.cpp reader/reader.hpp
	g++ -c main.cpp -o bin/main.o

reader.o: reader/reader.cpp reader/reader.hpp
	g++ -c reader/reader.cpp -o bin/reader.o
