#ifndef graphics_mapmeasure_hpp
#define graphics_mapmeasure_hpp

#include "../reader/reader.hpp"
#include "window.hpp"

#ifndef PI
#define PI 3.14159265
#endif

class MapMeasure {
	public:
		unsigned int width;
		unsigned int height;
		MapMeasure();
		MapMeasure(unsigned int, unsigned int);
};

MapMeasure measureMap(Window*, MapNode*);
float getBranchLength(Window*, MapNode*);
int getBiggestNodeWidth(Window*, MapNode*);

#endif
