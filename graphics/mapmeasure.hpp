#ifndef graphics_mapmeasure_hpp
#define graphics_mapmeasure_hpp

#include "../reader/reader.hpp"
#include "window.hpp"

class MapMeasure {
	public:
		unsigned int width;
		unsigned int height;
		MapMeasure();
		MapMeasure(unsigned int, unsigned int);
};

MapMeasure measureMap(Window*, MapNode*);

#endif
