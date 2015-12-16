#ifndef utils_nodecounter_hpp
#define utils_nodecounter_hpp

#include "../reader/reader.hpp"

static unsigned int getNodeComplexity(MapNode *node) {
	if (node->children.size() == 0) {
		return 1;
	} else {
		int maxComplexity = 0;
		int childCount = node->children.size();
		for (unsigned int i = 0; i < childCount; ++i) {
			int nodeComplexity = getNodeComplexity(node->children.at(i));
			if (nodeComplexity > maxComplexity) {
				maxComplexity = nodeComplexity;
			}
		}
		return maxComplexity;
	}
}

#endif
