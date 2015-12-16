#ifndef READER_HPP
#define READER_HPP

#include <vector>
#include <string>

class MapNode {
	public:
		std::string text;
		MapNode *parent;
		unsigned int level;
		std::vector<MapNode*> children;
};

unsigned int getNodeComplexity(MapNode*);

class MapReader {
	public:
		static MapReader *openFile(std::string);

		std::vector<MapNode*> bases;
		std::string file;
		~MapReader();

	private:
		MapReader();
};

#endif
