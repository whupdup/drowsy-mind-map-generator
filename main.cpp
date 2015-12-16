#include <iostream>
#include "reader/reader.hpp"

void debugTree(MapNode *startNode) {
	for (unsigned int i = 0UL; i < startNode->level; ++i) {
		std::cout.put('\t');
	}
	std::cout << startNode->text << std::endl;

	for (unsigned int i = 0UL; i < startNode->children.size(); ++i) {
		debugTree(startNode->children.at(i));
	}
}

int main(int argc, char **argv) {
	MapReader *mapReader = MapReader::openFile(std::string("demo map.txt"));

	for (unsigned int i = 0; i < mapReader->bases.size(); ++i) {
		debugTree(mapReader->bases.at(i));
	}

	delete mapReader;
}
