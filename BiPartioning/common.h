#ifndef common_h
#define common_h

#include <vector>
#include <map>
#include <queue>
#include <algorithm>
#include <unordered_map>

typedef struct node node_t;
extern std::vector<int> allCells;
extern std::vector<int> netList;
extern std::unordered_map<int,std::vector<int> > mapOfBlockNet;

enum direction {
	LEFT = 1, MIDDLE, RIGHT
};

struct node {
	int leftCount;
	int rightCount;
	int middleCount;
	int direction;
	bool isLeftConditionMet;
	bool isRightConditionMet;
	bool isMiddleConditionMet;
	int blockNumber;
	int lowerBoundValue;
	node_t* leftNode;
	node_t* rightNode;
	node_t* MiddleNode;
	node_t* parentNode;
};


void dataParser(const char* filename);
#endif
