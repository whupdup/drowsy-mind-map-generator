#include "mapmeasure.hpp"

MapMeasure::MapMeasure() {
	width = 0;
	height = 0;
}

MapMeasure::MapMeasure(unsigned int w, unsigned int h) {
	width = w;
	height = h;
}

MapMeasure measureMap(Window *window, MapNode *base) {
	int x, y;
	if (base->children.size() == 0) {
		window->getBubbleSize(base->text, &x, &y);
	} else {
		window->getBubbleSize(base->text, &x, &y);
		// TODO: Calculate size including children
	}

	return MapMeasure(x, y);
}
