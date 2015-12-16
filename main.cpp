#include "graphics/window.hpp"
#include "graphics/mapmeasure.hpp"
#include "reader/reader.hpp"
#include <iostream>

MapReader *mapReader;

void paint(Window *window) {
	MapMeasure mapSize(measureMap(window, mapReader->bases.at(0)));
	// TODO: Draw tree
	window->drawBubble(std::string(mapReader->bases.at(0)->text), 20, 50, 
			mapSize.width, mapSize.height);
}

int main(int argc, char **argv) {
	mapReader = MapReader::openFile(std::string("demo map.txt"));
	if (mapReader == 0UL) {
		std::cout << "Failed to load map." << std::endl;
		return -1;
	}

	Window *window = new Window(std::string("Drowsy Mind Map Generator"));
	window->setPaintCallback(paint);

	while (window->process());

	return 0;
}
