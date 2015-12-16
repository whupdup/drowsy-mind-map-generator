#include "reader.hpp"

#include <iostream>
#include <fstream>

static unsigned int getLevel(std::string line) {
	int level = 0;
	int lineLength = line.length();
	for(unsigned long i = 0UL; i < lineLength && line.at(i) == '\t'; 
			++i, ++level);
	return level;
}

static bool isCharWhitespace(char data) {
	return data == '\n' || data == '\t' || data == ' ' || data == '\r';
}

static bool isEmpty(std::string line) {
	unsigned long len = line.length();
	for (unsigned long i = 0UL; i < len; ++i) {
		char marker = line.at(i);
		if (!isCharWhitespace(marker))
			return false;
	}
	return true;
}

static std::string trim(std::string line) {
	unsigned int frontMarker = 0UL;
	unsigned int backMarker = 0UL;
	unsigned int lineLength = line.length();

	for (unsigned int i = 0UL; i < lineLength; ++i) {
		if (!isCharWhitespace(line.at(i))) {
			frontMarker = i;
			break;
		}
	}

	for (unsigned int i = lineLength - 1; i >= 0; --i) {
		if (!isCharWhitespace(line.at(i))) {
			backMarker = i;
			break;
		}
	}

	return line.substr(frontMarker, backMarker - frontMarker + 1);
}

MapReader::MapReader() { }

static void destroyBase(MapNode *node) {
	if (node->children.size() != 0) {
		for (unsigned int i = 0UL; i < node->children.size(); ++i) {
			destroyBase(node->children.at(i));
		}
	}

	delete node;
}

MapReader::~MapReader() {
	for (unsigned int i = 0; i < bases.size(); ++i) {
		destroyBase(bases.at(i));
	}
}

MapReader *MapReader::openFile(std::string fileName) {
	std::ifstream stream(fileName.c_str());

	if (stream.is_open()) {
		MapReader *mapReader = new MapReader();
		mapReader->file = fileName;

		std::string line;
		unsigned int currentLevel = 0;
		MapNode *parentNode = (MapNode*)0UL;
		MapNode *lastNode = (MapNode*)0UL;

		while (stream.good()) {
			std::getline(stream, line);

			if (!isEmpty(line)) {
				unsigned int nodeLevel = getLevel(line);
				if (nodeLevel > currentLevel) {
					currentLevel = nodeLevel;
					parentNode = lastNode;
				} else if (currentLevel > nodeLevel) {
					do {
						parentNode = parentNode->parent;
						--currentLevel;
					} while (currentLevel > nodeLevel);
				}

				MapNode *mapNode = new MapNode();
				mapNode->level = nodeLevel;
				mapNode->text = trim(line);

				lastNode = mapNode;

				if (nodeLevel != 0) {
					mapNode->parent = parentNode;
					parentNode->children.push_back(mapNode);
				} else {
					mapReader->bases.push_back(mapNode);
				}
			}
		}

		stream.close();
		return mapReader;
	} else {
		return (MapReader*)0L;
	}
}
