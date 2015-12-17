#include "graphics/window.hpp"
#include "graphics/mapmeasure.hpp"
#include "utils/nodecounter.hpp"
#include "reader/reader.hpp"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <time.h>

MapReader *mapReader;
bool beenResized = false;

float mdf(float a, float b) {
	return a - floor(a / b) * b;
}

void paintTree(Window *window, MapNode *node, unsigned int complexity, 
		int x=-1, int y=-1, int offsX=0, int offsY=0) {
	if (x == -1 && y == -1) {
		MapMeasure mapSize(measureMap(window, node));
		x = mapSize.width*2.5;
		y = mapSize.height*2.5;

		if (!beenResized) {
			beenResized = true;
			window->resize(mapSize.width*5, mapSize.height*5);
		}
	}

	int sizeX, sizeY;
	window->getBubbleSize(node->text, complexity, &sizeX, &sizeY);

	int childCount = node->children.size();
	if (childCount > 0) {
		float angle = ((360.f/childCount)*PI)/180.f;
		float dist = getBranchLength(window, node) + sizeX;
		int childX, childY;
		float off = mdf(((float)rand()), PI*2);
		for (unsigned int i = 0; i < childCount; ++i) {
			childX = sin(angle*i + off)*dist + x;
			childY = cos(angle*i + off)*dist + y;
			window->drawLine(childX - offsX, childY - offsY, x - offsX, y - offsY);
			paintTree(window, node->children.at(i), complexity - 1, childX, childY, offsX, offsY);
		}
	}

	window->drawBubble(node->text, complexity, x - sizeX/2 - offsX, y - sizeY/2 - offsY, sizeX, sizeY);
}

static int seed;

void paint(Window *window) {
	srand(seed);
	MapNode *node = mapReader->bases.at(0);
	int complexity = getNodeComplexity(node);
	paintTree(window, node, complexity, -1, -1, window->offsetX, window->offsetY);
}

int main(int argc, char **argv) {
	seed = time(0);

	if (argc != 2) {
		std::cout << "syntax: drowsymapper {file}" << std::endl;
		return 0;
	}

	mapReader = MapReader::openFile(std::string(*(argv + 1)));
	if (mapReader == 0UL) {
		std::cout << "Failed to load map." << std::endl;
		return -1;
	}

	Window *window = new Window(std::string("Drowsy Mind Map Generator"));
	window->setPaintCallback(paint);

	while (window->process())
	{
		if (window->mouse1Down)
			window->repaint();
	}

	return 0;
}
