all: drowsymapper.exe

run: drowsymapper.exe
	./drowsymapper.exe

drowsymapper.exe: main.o reader.o
	g++ main.o reader.o -o drowsymapper.exe

main.o: main.cpp reader.hpp
	g++ -c main.cpp

reader.o: reader.cpp reader.hpp
	g++ -c reader.cpp
