#include "mapmeasure.hpp"
#include <cmath>

MapMeasure::MapMeasure() {
	width = 0;
	height = 0;
}

MapMeasure::MapMeasure(unsigned int w, unsigned int h) {
	width = w;
	height = h;
}

inline float distance(int x1, int y1, int x2, int y2) {
	return (float)sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
}

inline float toRadians(double degrees) {
	return (degrees * 180)/PI;
}

int getBiggestNodeWidth(Window *window, MapNode *base) {
	int maxWidth = 0;
	int childCount = base->children.size();
	for (unsigned int i = 0; i < childCount; ++i) {
		int size = measureMap(window, base->children.at(i)).width;
		if (size > maxWidth) {
			maxWidth = size;
		}
	}
	return maxWidth;
}

static const float BRANCH_PADDING = 10;

float getBranchLength(Window *window, MapNode *base) {
	int childCount = base->children.size();
	float divisionAngle = toRadians(360/childCount);
	int maxWidth = getBiggestNodeWidth(window, base);
	float branchLength = ((BRANCH_PADDING + maxWidth/2) / 
			sin(divisionAngle))*sin(180 - divisionAngle);
	return branchLength;
}

MapMeasure measureMap(Window *window, MapNode *base) {
	int rootX, rootY;
	int childCount = base->children.size();
	unsigned int complexity = getNodeComplexity(base);
	window->getBubbleSize(base->text, complexity, &rootX, &rootY);
	int x, y;

	if (childCount != 0) {
		int biggestNodeWidth = getBiggestNodeWidth(window, base);
		x = getBranchLength(window, base) + biggestNodeWidth/2 + rootX/2;
		x = x > biggestNodeWidth ? x : biggestNodeWidth;
		y = x;
	} else {
		x = rootX;
		y = rootY;
	}

	return MapMeasure(x, y);
}
