#ifndef common_h
#define common_h

#include <vector>
#include <map>
#include <queue>
#include <algorithm>
#define NUMBER_OF_channelS_IN_ONE_SWITCH_BLOCK 4
#define NUMBER_OF_PINS_IN_LOGIC_BLOCK 5

typedef struct switchblock switchblock_t;
typedef struct logicblock logicblock_t;
typedef struct channel channel_t;
typedef struct pin pin_t;
extern int switchBlockGridSize;
extern int logicBlockGridSize;
extern int widthOfchannel;
extern int initialwidthOfchannel;
extern int startXLogicBlock;
extern int startYLogicBlock;
extern int xSourceCordinate;;
extern int ySourceCordinate;
extern int xSinkCordinate;
extern int ySinkCordinate;
extern int channelLabel;
extern int pinSeparation;
extern int sinkPinNumber;
extern int sourcePinNumber;
extern switchblock_t sblock;
extern switchblock_t tblock;
extern logicblock_t lblock;
extern channel_t seg;
extern bool isPathAvailable;
extern pin_t pin;
extern bool shortPathEnabled;
extern std::vector<pin_t> finalPath;
extern std::vector <std::vector <pin_t> > listofPaths;
extern std::vector<switchblock_t> listOfSwitchBlocks;				// Holds the switch blocks
extern std::vector<logicblock_t> listOfLogicBlocks;					// Holds the logic blocks
extern std::vector<int> listOfAdjacentSwitchBlocksIndex;			// Holds the index of the adjacent switch blocks
extern std::vector <switchblock_t> listOfAdjacentSwitchBlocks;
extern std::vector <channel_t> targetList;							// Holds the segements of target
extern std::vector< std::pair<int,int> > xycordinatelistSource;	// Holds the list of x and y of source
extern std::vector< std::pair<int,int> > xycordinatelistSink;		// Holds the list of x and y of sink
extern std::vector <int> sourcePinList;
extern std::vector <int> sinkPinList;
extern std::vector <switchblock_t> listOfParentSwitchBlocks;
extern std::vector <switchblock_t> listofChildrenSwitchBlocks;
extern std::vector <int> listOfChannelLabels;								
extern std::vector<std::pair<int,channel_t> > listOfchannelsWithSameLabel;
extern std::vector<float> listOfDistance;
extern std::deque<std::pair<int,channel_t> > expansionList;

/*
 A pin consists of a number, its availability
 */
struct pin {
    int xGraphicsCordinate;
    int yGraphicsCordinate;
    int availablityStatus;
    int pinNumber;
    int direction;
};
/*
 A switch block consists of 5 channels with the 5th channel connected to the logic block.
 */
enum channelDirection {
    TOP = 0,RIGHT, BOTTOM, LEFT
};

/*
 Once a route has been done, and if the pin is available within its path then the pin is not available
 */
enum pinAvailabilityStatus {
    PIN_AVAILAIBLE = 0, PIN_NOT_AVAILABLE
};

/*
 A logic block consists of its x and y cordinates and the number of pins it has
 */

struct logicblock {
    int xCordinate;
    int yCordinate;
    int xGraphicsCordinate;
    int yGraphicsCordinate;
    std::vector<pin_t> listOfPinsOfchannel;
};

/*
 A channel consists of W pins. Every channel will have its own direction and each channel belongs to a switch block.
 */

struct channel {
    int xCordinate;
    int yCordinate;
    int drawLabel;
    int direction;
    int channelLabel;
    std::vector<pin_t> listOfPinsOfchannel;
};

/*
 A switch block contains its x and y cordinates, along with the channels.
 Each switch block has 4 channels.
 */
struct switchblock {
    int xGraphicsCordinate;
    int yGraphicsCordinate;
    int xCordinate;
    int yCordinate;
    std::vector<channel_t> listOfchannelsOfSwitchBlock;
};

void createSwitchBlocks(); 
void createLogicBlocks();
void drawscreen();
void dataParser(const char* filename);
switchblock findSwitchBlockFromList(int x, int y);
logicblock findLogicBlockFromList(int x, int y);
void findAdjacentSwitchBlockIndex(switchblock *sblock, int direction, int direction1, bool shortPathEnabled);
void labelchannels(switchblock_t *sblock);
bool checkIfTargetReached();
int findPinIndexFromPinNumberSwitchBlock(int x, int index);
void resetLabels();
int findSwitchBlockIndexFromList(int x, int y);
int findDistance();
void sortMaxList();
void sortMinList();
void standardList();
#endif /* common_h */
