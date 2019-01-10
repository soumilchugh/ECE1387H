#include <stdio.h>
#include <iostream>
#include <vector>
#include "common.h"
#include "graphics.h"
#include <queue>
#include <cstdlib>
#include <pthread.h>
#include <random>
#include <time.h>       /* time */

#define NUM_THREADS 2

using namespace std;
std::vector< std::pair<int,int> > xycordinatelistSource;	// Holds the list of x and y of source
std::vector< std::pair<int,int> > xycordinatelistSink;		// Holds the list of x and y of sink
std::vector <switchblock_t> listOfParentSwitchBlocks;
std::deque<std::pair<int,channel_t> > expansionList;
int xSourceCordinate;
int ySourceCordinate;
int xSinkCordinate;
int ySinkCordinate;
int sinkPinNumber;
int widthOfchannel;
int initialwidthOfchannel;
int sourcePinNumber;
switchblock_t tblock;
int channelLabel = 0;
std::vector<std::pair<int,channel_t> > listOfchannelsWithSameLabel;
std::vector <switchblock_t> listofChildrenSwitchBlocks;
std::vector<pin_t> finalPath;
bool foundCommonchannel = false;
bool doneForFirstBlock = false;
int currentPinNumber = 0;
bool restartTrace = false;
std::vector <std::vector <pin_t> > listofPaths;
std::vector<channel_t> finalPathchannels;
std::vector <int> finalPathDirection;
std::vector <int> finalPathPinIndex;
std::vector<std::vector <channel_t> > finalPath1channels;
std::vector <std::vector <int> > finalPath1Direction;
std::vector <std::vector <int> > finalPath1PinIndex;
std::vector <int> listOfChannelLabels; 
std::vector<int> listOfAdjacentSwitchBlocksIndex; 
std::vector <switchblock_t> listOfAdjacentSwitchBlocks;  
std::vector <channel_t> targetList;
int pinSeparation = 0;
int finalchannelLabel = 0;
int commonchannelFoundCounter = 0;
int startpoint = 0;
bool found;
bool notFound;
int direction2;
int direction1;
int numberOfLoops = 0;
bool shortPathEnabled = true;
random_device rd;
void button_press (float x, float y) {
    /* Called whenever event_loop gets a button press in the graphics *
     * area.  Allows the user to do whatever he/she wants with button *
     * clicks.                                                        */

     printf("User clicked at coordinates (%f, %f)\n", x, y);
}

int findSwitchBlockIndexFromList(int x, int y) {
    int currentSwitchBlockIndex = 0;
    for (int i = 0; i < listOfSwitchBlocks.size(); i++) {
        if (listOfSwitchBlocks[i].xCordinate == x & listOfSwitchBlocks[i].yCordinate == y) {
            currentSwitchBlockIndex = i;
            return currentSwitchBlockIndex;
        }
    }
}

/*
This function checks the channel which is common between the two switch blocks.
*/
void checkCommonchannel(switchblock *sblock, int label) {
	int index = 0;
    foundCommonchannel = false;
	for (int channelNumber = 0; channelNumber < sblock->listOfchannelsOfSwitchBlock.size(); channelNumber++) {
		if (sblock->listOfchannelsOfSwitchBlock[channelNumber].drawLabel == label) {
			commonchannelFoundCounter ++;
			if (!doneForFirstBlock) {
				if (sinkPinNumber == 1 || sinkPinNumber == 2 ) {
					for (int i = 0; i < widthOfchannel; i+= 2) {
						if (sblock->listOfchannelsOfSwitchBlock[channelNumber].listOfPinsOfchannel[findPinIndexFromPinNumberSwitchBlock(i,channelNumber)].availablityStatus == PIN_AVAILAIBLE) {
							finalPath.push_back(sblock->listOfchannelsOfSwitchBlock[channelNumber].listOfPinsOfchannel[findPinIndexFromPinNumberSwitchBlock(i,channelNumber)]);
							finalPathchannels.push_back(sblock->listOfchannelsOfSwitchBlock[channelNumber]);
							finalPathPinIndex.push_back(findPinIndexFromPinNumberSwitchBlock(i,channelNumber));
							index = findSwitchBlockIndexFromList(sblock->listOfchannelsOfSwitchBlock[channelNumber].xCordinate, sblock->listOfchannelsOfSwitchBlock[channelNumber].yCordinate);
							listOfSwitchBlocks[index].listOfchannelsOfSwitchBlock[channelNumber].listOfPinsOfchannel[findPinIndexFromPinNumberSwitchBlock(i,channelNumber)].availablityStatus = PIN_NOT_AVAILABLE;

							foundCommonchannel = true;
							break;
						}
					}
					if (foundCommonchannel == false) {
						break;
					}
				} else if (sinkPinNumber == 3 || sinkPinNumber == 4 ) {
					for (int i = 1; i < widthOfchannel; i+=2) {
						if (sblock->listOfchannelsOfSwitchBlock[channelNumber].listOfPinsOfchannel[findPinIndexFromPinNumberSwitchBlock(i,channelNumber)].availablityStatus == PIN_AVAILAIBLE)  {
							finalPath.push_back(sblock->listOfchannelsOfSwitchBlock[channelNumber].listOfPinsOfchannel[findPinIndexFromPinNumberSwitchBlock(i,channelNumber)]);
							finalPathPinIndex.push_back(findPinIndexFromPinNumberSwitchBlock(i,channelNumber));
							finalPathchannels.push_back(sblock->listOfchannelsOfSwitchBlock[channelNumber]);
							index = findSwitchBlockIndexFromList(sblock->listOfchannelsOfSwitchBlock[channelNumber].xCordinate, sblock->listOfchannelsOfSwitchBlock[channelNumber].yCordinate);					
							listOfSwitchBlocks[index].listOfchannelsOfSwitchBlock[channelNumber].listOfPinsOfchannel[findPinIndexFromPinNumberSwitchBlock(i,channelNumber)].availablityStatus = PIN_NOT_AVAILABLE;														
							
                            foundCommonchannel = true;
							break;
						}
					}
					
					if (foundCommonchannel == false) {
					
					}
				}
				else if (sinkPinNumber == 5) {
					for (int j = 0; j < sblock->listOfchannelsOfSwitchBlock[channelNumber].listOfPinsOfchannel.size(); j++) {
						if (sblock->listOfchannelsOfSwitchBlock[channelNumber].listOfPinsOfchannel[j].availablityStatus == PIN_AVAILAIBLE) {
							finalPath.push_back(sblock->listOfchannelsOfSwitchBlock[channelNumber].listOfPinsOfchannel[findPinIndexFromPinNumberSwitchBlock(j,channelNumber)]);
							finalPathPinIndex.push_back(findPinIndexFromPinNumberSwitchBlock(j,channelNumber));
							finalPathchannels.push_back(sblock->listOfchannelsOfSwitchBlock[channelNumber]);
							index = findSwitchBlockIndexFromList(sblock->listOfchannelsOfSwitchBlock[channelNumber].xCordinate, sblock->listOfchannelsOfSwitchBlock[channelNumber].yCordinate);
							listOfSwitchBlocks[index].listOfchannelsOfSwitchBlock[channelNumber].listOfPinsOfchannel[findPinIndexFromPinNumberSwitchBlock(j,channelNumber)].availablityStatus = PIN_AVAILAIBLE;
						
                            foundCommonchannel = true;
							break;
						}
					}
					if (foundCommonchannel == false) {
						break;
					}
				} 
			} else {
				if (sblock->listOfchannelsOfSwitchBlock[channelNumber].listOfPinsOfchannel[findPinIndexFromPinNumberSwitchBlock(currentPinNumber,channelNumber)].availablityStatus == PIN_AVAILAIBLE) {
					finalPath.push_back(sblock->listOfchannelsOfSwitchBlock[channelNumber].listOfPinsOfchannel[findPinIndexFromPinNumberSwitchBlock(currentPinNumber,channelNumber)]);
					finalPathPinIndex.push_back(findPinIndexFromPinNumberSwitchBlock(currentPinNumber,channelNumber));
					finalPathchannels.push_back(sblock->listOfchannelsOfSwitchBlock[channelNumber]);
					index = findSwitchBlockIndexFromList(sblock->listOfchannelsOfSwitchBlock[channelNumber].xCordinate, sblock->listOfchannelsOfSwitchBlock[channelNumber].yCordinate);
					listOfSwitchBlocks[index].listOfchannelsOfSwitchBlock[channelNumber].listOfPinsOfchannel[findPinIndexFromPinNumberSwitchBlock(currentPinNumber,channelNumber)].availablityStatus = PIN_NOT_AVAILABLE;
					
                    foundCommonchannel = true;
					found = true;
					break;
				}
				else {
					found = false;
				} 

			}
			
    	} 

	}

}

void traceBack() {
	int count = 0;
	memset(&sblock, 0, sizeof(sblock));
	memset (&pin,0,sizeof(pin));
	memset(&tblock, 0, sizeof(tblock));
	tblock = findSwitchBlockFromList(xSinkCordinate + 1,ySinkCordinate);
	sblock = tblock;
	finalPath.clear();
	doneForFirstBlock = false;
	int direction = 0;
	int currentDirection = 0;
	listOfchannelsWithSameLabel.clear();
	finalPathchannels.clear();
	finalPathDirection.clear();
	finalPathPinIndex.clear();
	int index = 0;
    int counter = 0;
	while (channelLabel >= 0) {
		commonchannelFoundCounter = 0;
		while (count < expansionList.size() & expansionList[count].first == channelLabel) {
			listOfchannelsWithSameLabel.push_back(std::make_pair(expansionList[count].first, expansionList[count].second));
			count++;
		}		
		for (int k = 0; k < listOfchannelsWithSameLabel.size(); k++) {
			checkCommonchannel(&sblock,listOfchannelsWithSameLabel[k].second.drawLabel);
			if (restartTrace) {
    			finalPath1channels.push_back(finalPathchannels);
    			finalPath1Direction.push_back(finalPathDirection);
    			finalPath1PinIndex.push_back(finalPathPinIndex);
				finalPathchannels.clear();
				finalPathDirection.clear();
				finalPathPinIndex.clear();
    			finalPath.clear();
    			listOfchannelsWithSameLabel.clear();
    			channelLabel = finalchannelLabel;
    			commonchannelFoundCounter = 0;
    			restartTrace = false;
                if (numberOfLoops == 100) {
                    return;
                } else
                return traceBack();   
    		}	
    		if (foundCommonchannel & restartTrace == false) {
    			found = true;
    			currentPinNumber = finalPath[finalPath.size() - 1].pinNumber;
    			direction = finalPath[finalPath.size() - 1].direction;
    			if (direction == 0 || direction == 1 ) {
    				currentDirection = direction + 2;
    			} 
    			else if (direction == 2 || direction == 3 ) {
    				currentDirection = direction - 2;
    			}
    			doneForFirstBlock = true;
    			finalPath.push_back(listOfchannelsWithSameLabel[k].second.listOfPinsOfchannel[findPinIndexFromPinNumberSwitchBlock(currentPinNumber,currentDirection)]);   	
    			
    			finalPathchannels.push_back(listOfchannelsWithSameLabel[k].second);
    			finalPathDirection.push_back(direction);
    			finalPathPinIndex.push_back(findPinIndexFromPinNumberSwitchBlock(currentPinNumber,currentDirection));
    			index = findSwitchBlockIndexFromList(listOfchannelsWithSameLabel[k].second.xCordinate, listOfchannelsWithSameLabel[k].second.yCordinate);
    			listOfSwitchBlocks[index].listOfchannelsOfSwitchBlock[listOfchannelsWithSameLabel[k].second.direction].listOfPinsOfchannel[findPinIndexFromPinNumberSwitchBlock(currentPinNumber,currentDirection)].availablityStatus = PIN_NOT_AVAILABLE;
    			memset(&sblock, 0, sizeof(sblock));
    			sblock = findSwitchBlockFromList(listOfchannelsWithSameLabel[k].second.xCordinate,listOfchannelsWithSameLabel[k].second.yCordinate);
    			break;
    		} else {
    			continue;
    		}
    	}

    	if (found == false) {
			finalPath1channels.push_back(finalPathchannels);
    		finalPath1Direction.push_back(finalPathDirection);
    		finalPath1PinIndex.push_back(finalPathPinIndex);
			finalPathchannels.clear();
			finalPathDirection.clear();
			finalPathPinIndex.clear();
    		finalPath.clear();
    		listOfchannelsWithSameLabel.clear();
    		channelLabel = finalchannelLabel;
    		restartTrace = false;
            numberOfLoops++;
            if (numberOfLoops == 100) {
                return;
            } else
    		  return traceBack();	
    	}
    	if (commonchannelFoundCounter == 0) {
    		finalPath1channels.push_back(finalPathchannels);
    		finalPath1Direction.push_back(finalPathDirection);
    		finalPath1PinIndex.push_back(finalPathPinIndex);
			finalPathchannels.clear();
			finalPathDirection.clear();
			finalPathPinIndex.clear();
    		finalPath.clear();
    		listOfchannelsWithSameLabel.clear();
    		channelLabel = finalchannelLabel;
    		restartTrace = false;
            numberOfLoops++;

    		if (numberOfLoops == 100) {
                return;
            } else
              return traceBack();   
    	}
    	if (finalPath.size() == 0) {
    		finalPath.clear();
    		return ;
    	}
    	listOfchannelsWithSameLabel.clear();
    	channelLabel -- ;
	}
}

bool routing() {
    int currentIndex;
    int xmax,ymax,xmin,ymin;
	for (int i = 0; i < xycordinatelistSource.size(); i++) {
    	memset(&sblock, 0, sizeof(sblock));
    	memset(&lblock, 0, sizeof(lblock));
    	memset(&tblock, 0, sizeof(tblock));

        if (shortPathEnabled == false) {
          //  cout << "Short test path enabled" << "\n";
            i = currentIndex;
        }
    	targetList.clear();
    	listOfChannelLabels.clear();
    	listOfParentSwitchBlocks.clear();
        listOfAdjacentSwitchBlocks.clear();
        isPathAvailable == true;
    	channelLabel = 0;
    	finalPath.clear();
    	finalPathchannels.clear();
		finalPathDirection.clear();
		finalPathPinIndex.clear();
    	resetLabels();
        expansionList.clear();
    	listOfchannelsWithSameLabel.clear();
    	xSinkCordinate = xycordinatelistSink[i].first;			// get the x and y cordinates of sink
    	ySinkCordinate = xycordinatelistSink[i].second;
    	xSourceCordinate = xycordinatelistSource[i].first;		// get the x and y coridinates of source
    	ySourceCordinate = xycordinatelistSource[i].second;
        if (shortPathEnabled) {
            if ((xSinkCordinate - xSourceCordinate) >= 0 & (ySinkCordinate - ySourceCordinate) >= 0) {
                direction1 = RIGHT;
                direction2 = TOP;
            } 
            else if ((xSinkCordinate - xSourceCordinate) >= 0 & (ySinkCordinate - ySourceCordinate) <= 0) {
                direction1 = RIGHT;
                direction2 = BOTTOM;

            } 
            else if ((xSinkCordinate - xSourceCordinate) <= 0 & (ySinkCordinate - ySourceCordinate) >= 0) {
                direction1 = LEFT;
                direction2 = TOP;
            } 
            else if ((xSinkCordinate - xSourceCordinate) <= 0 & (ySinkCordinate - ySourceCordinate) <= 0) {
                direction1 = LEFT;
                direction2 = BOTTOM;
            } 
        }
    	sinkPinNumber = sinkPinList[i];							// get the source pin number
    	sourcePinNumber = sourcePinList[i];						// Get the sink pin number
    	lblock = findLogicBlockFromList(xSourceCordinate,ySourceCordinate);	// 
    	sblock = findSwitchBlockFromList(xSourceCordinate + 1,ySourceCordinate);
    	labelchannels(&sblock);
    	tblock = findSwitchBlockFromList(xSinkCordinate + 1,ySinkCordinate);
    	targetList.push_back(tblock.listOfchannelsOfSwitchBlock[0]);
    	targetList.push_back(tblock.listOfchannelsOfSwitchBlock[1]);
    	targetList.push_back(tblock.listOfchannelsOfSwitchBlock[2]);
    	targetList.push_back(tblock.listOfchannelsOfSwitchBlock[3]);
    	listOfParentSwitchBlocks.clear();
    	while (checkIfTargetReached()) {
    		listOfAdjacentSwitchBlocks.clear();
    		listOfAdjacentSwitchBlocksIndex.clear();
    		channelLabel++;
    		if (listOfParentSwitchBlocks.size() == 0) {
                findAdjacentSwitchBlockIndex(&sblock,direction1,direction2,shortPathEnabled);
                
            	for (int k = 0; k < listOfAdjacentSwitchBlocksIndex.size(); k++) {
    				if (listOfAdjacentSwitchBlocksIndex[k] > 0) {
    					listOfAdjacentSwitchBlocks.push_back(listOfSwitchBlocks[listOfAdjacentSwitchBlocksIndex[k]]);
    				}
    			}
                for (std::vector<switchblock_t>::const_iterator it = listOfAdjacentSwitchBlocks.begin(); it != listOfAdjacentSwitchBlocks.end(); ++it) {
    	    		memset(&sblock, 0, sizeof(sblock));
    	    		sblock = *it;
    	    		labelchannels(&sblock);
    	    		listOfParentSwitchBlocks.push_back(sblock);
    			}

        	} else {
                for (std::vector<switchblock_t>::const_iterator it = listOfParentSwitchBlocks.begin(); it != listOfParentSwitchBlocks.end(); ++it) {
            		listOfAdjacentSwitchBlocksIndex.clear();
                    listOfAdjacentSwitchBlocks.clear(); 
                    memset(&sblock, 0, sizeof(sblock));
                	sblock = *it; 
                 	findAdjacentSwitchBlockIndex(&sblock,direction1,direction2,shortPathEnabled); 
                   
                 	for (int k = 0; k < listOfAdjacentSwitchBlocksIndex.size(); k++) {
    					if (listOfAdjacentSwitchBlocksIndex[k] > 0) {
    						listOfAdjacentSwitchBlocks.push_back(listOfSwitchBlocks[listOfAdjacentSwitchBlocksIndex[k]]);
    					}
    				}
                    for (std::vector<switchblock_t>::const_iterator it = listOfAdjacentSwitchBlocks.begin(); it != listOfAdjacentSwitchBlocks.end(); ++it) {
		    			memset(&sblock, 0, sizeof(sblock));
		    			sblock = *it;
		    			labelchannels(&sblock);
		    			if (!checkIfTargetReached()) {
	                    	break;
	                	}
                        if (isPathAvailable == false) {
                            break;
                        }
                        if (xSourceCordinate > xSinkCordinate){
                            xmax = xSourceCordinate;
                            xmin = xSinkCordinate;
                        } else if (xSourceCordinate < xSinkCordinate) {
                            xmax = xSinkCordinate;
                            xmin = xSourceCordinate;
                        } else {
                            xmax = xSinkCordinate + 1;
                            xmin = xSourceCordinate - 1;
                        }
                        if (ySourceCordinate > ySinkCordinate){
                            ymax = ySourceCordinate;
                            ymin = ySinkCordinate;
                        } else if (ySourceCordinate < ySinkCordinate) {
                            ymax = ySinkCordinate;
                            ymin = ySourceCordinate;
                        }else {
                            ymax = ySinkCordinate + 1;
                            ymin = ySourceCordinate - 1;
                        }
                        // this is the virtual boundary
                        if (sblock.xCordinate > (xmax + 1) || sblock.yCordinate > (ymax + 1) || sblock.xCordinate < (xmin - 1) || sblock.yCordinate < (ymin - 1))
                            continue;
                        else
		    			   listofChildrenSwitchBlocks.push_back(sblock);
	    			}
	    			if (!checkIfTargetReached()) {
	                    break;
	                }
	                listOfAdjacentSwitchBlocksIndex.clear();
        			listOfAdjacentSwitchBlocks.clear(); 
            	}
            	listOfParentSwitchBlocks.clear();
            	listOfParentSwitchBlocks = listofChildrenSwitchBlocks;
            	listofChildrenSwitchBlocks.clear();
    			listOfAdjacentSwitchBlocksIndex.clear();
        		listOfAdjacentSwitchBlocks.clear(); 
        	}
            if (listOfParentSwitchBlocks.size() > 1000000) {
                    break;
            }
            if (isPathAvailable == false) {
                break;
            }
        }	
    	finalchannelLabel = channelLabel;
        numberOfLoops = 0;
    	traceBack();

        if (finalPath.size() != 0) {
            listofPaths.push_back(finalPath);
            shortPathEnabled = true;
        } 
    	int index = 0;
    	for (int i = 0; i < finalPath1channels.size(); i++) {
    		finalPathchannels.clear();
    		finalPathDirection.clear();
    		finalPathPinIndex.clear();
    		finalPathchannels = finalPath1channels[i];
    		finalPathDirection = finalPath1Direction[i];
    		finalPathPinIndex = finalPath1PinIndex[i];

    		for (int j = 0; j < finalPathchannels.size(); j++) {
    			index = findSwitchBlockIndexFromList(finalPathchannels[j].xCordinate, finalPathchannels[j].yCordinate);
    			listOfSwitchBlocks[index].listOfchannelsOfSwitchBlock[finalPathchannels[j].direction].listOfPinsOfchannel[finalPathPinIndex[j]].availablityStatus = PIN_AVAILAIBLE;
    		} 
    	}
        currentIndex = i;
    	finalPath1channels.clear();
    	finalPath1Direction.clear();
    	finalPath1PinIndex.clear();
        if (numberOfLoops == 100 && shortPathEnabled == false) {
            return true;
        }
    	else if (numberOfLoops == 100) {
            shortPathEnabled = false; 
        }  
	}
	return true;
}

void resetList() {
    listOfSwitchBlocks.clear();
    listOfLogicBlocks.clear();
    listofPaths.clear();
    expansionList.clear();                             // Holds the list of channels of a block
    listOfAdjacentSwitchBlocksIndex.clear();            // Holds the index of the adjacent switch blocks
    listOfAdjacentSwitchBlocks.clear();
    targetList.clear();                                 // Holds the segements of target
    listOfParentSwitchBlocks.clear();
    listofChildrenSwitchBlocks.clear();
    listOfChannelLabels.clear();                             
    listOfchannelsWithSameLabel.clear();
}

int main(int argc, char *argv[]) {
    int r;
    int count = 0;
    if (argc > 1) {
        dataParser(argv[1]);
    }

    for (int i = initialwidthOfchannel - 1; i <= initialwidthOfchannel; i++) {       
        if (logicBlockGridSize >= 10)
            sortMinList();
        else {
            standardList();
        }
        isPathAvailable == true;
        clock_t begin = clock();
        shortPathEnabled = true;
        resetList();
        widthOfchannel = i;
        pinSeparation = 20 - widthOfchannel;
        std::cout << "Trying Width of channel " << widthOfchannel << "\n";
        createSwitchBlocks();
        createLogicBlocks();
        cout << "Routing" << "\n";
        while(!routing());
        clock_t end = clock();
        double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
        cout << "elapsed_secs" <<  elapsed_secs << "\n";
        count ++;
        if (listofPaths.size() == (xycordinatelistSource.size())) {
            break;
        } else {
            continue;
            }
    }
    int TrackCounter = 0;
    std::vector<pin_t> countList;
    for (int i = 0; i < listofPaths.size(); i++) {
         countList = listofPaths[i];
         for (int j = 0; j< countList.size(); j++){
            if (j % 2 == 0)                         // Count the alternative ones since these are pins and 2 pins make one track
                TrackCounter++;
         }
    }
    cout << "Track Counter" << TrackCounter << "\n";
    cout << "Minimum W found is " << widthOfchannel << "\n";
    init_graphics("Some Example Graphics", WHITE);
    cout << "Rout ended" ;
   	init_world (0.,0.,1500.,1500.);
   	update_message("Interactive graphics example.");
   	drawscreen();
   	event_loop(button_press,NULL, NULL,drawscreen);
   	close_graphics ();
   	printf ("Graphics closed down.\n");


}