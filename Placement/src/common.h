#ifndef common_h
#define common_h

#include <vector>
#include <map>
#include <queue>
#include <algorithm>
#include <unordered_map>

typedef struct cell cell_t;
typedef struct slotMatrix slotMatrix_t;
extern std::vector<int> allCells;
extern std::vector<int> moveableCells;
extern std::vector<int> fixedCells;
extern std::vector<std::pair<int,int> > cellType;
extern std::vector<int> fixedCellType;
extern std::vector<int> weights;
extern std::vector<int> netList;
extern std::vector< std::pair<double,double> > xycordinatelist;
extern std::vector <int> NonZeroElementsIndices;
extern std::vector <int > NonZeroElementsCounter;
extern std::vector <double> NonZeroElements;
extern std::unordered_map<int,std::vector<int> > mapOfBlockNet;

struct slotMatrix {
    double x;
    double y;
    int column;
    bool isSlotTaken;
};

struct cell {
    double x;
    double y;
    int number;
};
void dataParser(const char* filename);
#endif
