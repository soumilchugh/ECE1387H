#include "common.h"
#include "graphics.h"
#include <iostream>
#include <stdio.h>

using namespace std;
switchblock_t sblock;
channel_t seg;
logicblock_t lblock;
std::vector<switchblock> listOfSwitchBlocks;
std::vector<logicblock> listOfLogicBlocks;                   // Holds the logic blocks
pin_t pin;
int xGraphicsCordinate;
int yGraphicsCordinate;
std::vector<pin_t> finalPathToBeRouted;
int startXLogicBlock = 20;
int startYLogicBlock = 20;
int logicblockXCordinates[5] = {startXLogicBlock, startXLogicBlock+75, startXLogicBlock+45,startXLogicBlock+30,startXLogicBlock};
int logicblockYCordinates[5] = {startYLogicBlock,startYLogicBlock,startYLogicBlock+30,startYLogicBlock+40,startYLogicBlock+40};
int switchblockXCordinates[5] = {startXLogicBlock+75,startXLogicBlock+110,startXLogicBlock+175,startXLogicBlock+175,startXLogicBlock+75};
int switchblockYCordinates[5] = {startYLogicBlock+55, startYLogicBlock+25, startYLogicBlock+25, startYLogicBlock+125, startYLogicBlock+125};
bool isPathAvailable = true;

void getSourceLogicBlock (int x, int y) {
    memset(&lblock, 0, sizeof(lblock));
    lblock = findLogicBlockFromList(x ,y); 
}

void getSinkLogicBlock(int x, int y) {
    memset(&lblock, 0, sizeof(lblock));
    lblock = findLogicBlockFromList(x,y);
}

void getPinofLogicBlockchannel(int i, int x, int y) {
    pin.xGraphicsCordinate = x;
    pin.yGraphicsCordinate = y;
    pin.availablityStatus = PIN_AVAILAIBLE;
    pin.pinNumber = i;
    pin.direction = TOP;
}

int findPinIndexFromPinNumber(int x) {
    for (int i = 0; i < lblock.listOfPinsOfchannel.size(); i++) {
        if (x == lblock.listOfPinsOfchannel[i].pinNumber) {
            return i;
        }
    }

}

int findPinIndexFromPinNumberSwitchBlock (int x, int index) {
    for (int i = 0; i < sblock.listOfchannelsOfSwitchBlock[index].listOfPinsOfchannel.size(); i++) {
        if (x == sblock.listOfchannelsOfSwitchBlock[index].listOfPinsOfchannel[i].pinNumber) {
            return i;
        }
    }
}

void createLogicBlocks() {
    int x,y = 0;
    x = startXLogicBlock;
    y = startYLogicBlock;
    int midpointx1;
    int midpointx2;
    int midpointy1;
    int midpointy2 , midpointx3, midpointy3, midpointx4, midpointy4;
    int x1,y1,x2,y2,x3,y3,x4,y4;
    for (int yCordinate = logicBlockGridSize; yCordinate >= 0 ; yCordinate --) {
        for (int xCordinate = 0; xCordinate < switchBlockGridSize; xCordinate++) {
            if (yCordinate != logicBlockGridSize & xCordinate != 0) {
                memset(&lblock, 0, sizeof(lblock));
                lblock.xGraphicsCordinate = x;
                lblock.yGraphicsCordinate = y;
                lblock.xCordinate = xCordinate - 1;
                lblock.yCordinate = yCordinate;
                memset(&pin, 0, sizeof(pin));
                getPinofLogicBlockchannel(1,x+75,y+55);
                lblock.listOfPinsOfchannel.push_back(pin);
                x1 = x+45;
                y1 = y+30;
                x2 = x+75;
                y2 = y;
                x3 = x+75;
                y3 = y+55;
                x4 = x+110;
                y4 = y+25;
                midpointx1 = ((x1 + x2)/2);
                midpointy1 = ((y1 + y2)/2);
                midpointx2 = ((x3+x4)/2);
                midpointy2 = ((y3+y4)/2);
                memset(&pin, 0, sizeof(pin));
                getPinofLogicBlockchannel(3,midpointx2,midpointy2);
                lblock.listOfPinsOfchannel.push_back(pin);
                midpointx3 = (midpointx1 + x1)/2;
                midpointy3 = (midpointy1 + y1)/2;
                midpointx4 = (midpointx2 + x3)/2;
                midpointy4 = (midpointy2 + y3)/2;
                memset(&pin, 0, sizeof(pin));
                getPinofLogicBlockchannel(2,midpointx4,midpointy4);
                lblock.listOfPinsOfchannel.push_back(pin);
                midpointx1 = (midpointx1 + x2)/2;
                midpointy1 = (midpointy1 + y2)/2;
                midpointx2 = (midpointx2 + x4)/2;
                midpointy2 = (midpointy2 + y4)/2;
                memset(&pin, 0, sizeof(pin));
                getPinofLogicBlockchannel(4,midpointx2,midpointy2);
                lblock.listOfPinsOfchannel.push_back(pin);
                memset(&pin, 0, sizeof(pin));
                getPinofLogicBlockchannel(5,x+110,y+25);
                lblock.listOfPinsOfchannel.push_back(pin);
                listOfLogicBlocks.push_back(lblock);
            }
            x = x + 300;
        }
        x = startXLogicBlock;
        y = y + 200;
    }

}


void createchannels() {
    int x, y;
    int x1, y1;
    int x2, y2;
    x = startXLogicBlock;
    y = startYLogicBlock;
    x1 = x + 300;
    y1 = y;
    y2 = y + 200;
    int count = 0;
    int counter = widthOfchannel - 1;
    for (int i = logicBlockGridSize; i >= 0; i --) {
        for (int j = 0; j < switchBlockGridSize; j++) {
            setcolor (BLACK);
            if (j != logicBlockGridSize) {
                while( counter > 0) {
                    drawline(x + 175, (y + 125 - pinSeparation*count), x1 + 75, (y + 125 - pinSeparation*count));
                    count ++;
                    counter--;
                }
            }
            count = 0;
            counter = widthOfchannel - 1;
            // Vertical
            if (i != 0) {
                while (counter >= 0) { 
                    drawline((x + 175 - pinSeparation*count),y+125,(x + 175 - pinSeparation*count),y2+25);
                    count ++;
                    counter --;
                }
            }
            count  = 0;
            counter = widthOfchannel;
            x1 = x1 + 300;
            x = x + 300;
        }
        x = startXLogicBlock;
        x1 = x + 300;
        y = y + 200;
        y1 = y1 + 200;
        y2 = y2 + 200;
    }
}

void drawscreen() {
    int x,y = 0;
    int color = 0;
    x = startXLogicBlock;
    y = startYLogicBlock;
    set_draw_mode (DRAW_NORMAL);    // Should set this if your program does any XOR drawing in callbacks.
    clearscreen();                  /* Should precede drawing for all drawscreens */
    setfontsize (10);
    setlinestyle (SOLID);
    setcolor(BLUE);
    setlinewidth (2);
    int midpointx1;
    int midpointx2;
    int midpointy1;
    int midpointy2 , midpointx3, midpointy3, midpointx4, midpointy4;
    int x1,y1,x2,y2,x3,y3,x4,y4;
    for (int yCordinate = logicBlockGridSize; yCordinate >= 0 ; yCordinate --) {
        for (int xCordinate = 0; xCordinate < switchBlockGridSize; xCordinate++) {
            setcolor (DARKGREY);
            if (yCordinate != logicBlockGridSize & xCordinate != 0) {
                t_point polypts2[5] = {{x,y}, {x+75,y}, {x+45,y+30},{x+30,y+40}, {x,y+40}};                
                fillpoly(polypts2,5);
                setcolor (BLACK);
                memset(&lblock, 0, sizeof(lblock));
                drawline(x+45,y+30,x+75,y+55);
                setlinewidth (2);
                drawline(x+75,y, x+110,y+25);
                x1 = x+45;
                y1 = y+30;
                x2 = x+75;
                y2 = y;
                x3 = x+75;
                y3 = y+55;
                x4 = x+110;
                y4 = y+25;
                midpointx1 = ((x1 + x2)/2);
                midpointy1 = ((y1 + y2)/2);
                midpointx2 = ((x3+x4)/2);
                midpointy2 = ((y3+y4)/2);
                setlinewidth (2);
                drawline(midpointx1,midpointy1,midpointx2,midpointy2);
                midpointx3 = (midpointx1 + x1)/2;
                midpointy3 = (midpointy1 + y1)/2;
                midpointx4 = (midpointx2 + x3)/2;
                midpointy4 = (midpointy2 + y3)/2;
                setlinewidth (2);
                drawline(midpointx3,midpointy3,midpointx4,midpointy4);
                midpointx1 = (midpointx1 + x2)/2;
                midpointy1 = (midpointy1 + y2)/2;
                midpointx2 = (midpointx2 + x4)/2;
                midpointy2 = (midpointy2 + y4)/2;
                setlinewidth (2);
                drawline(midpointx1,midpointy1,midpointx2,midpointy2);
            }
            setcolor(YELLOW);
            t_point polypts[5] = {{x+75,y+55}, {x+110,y+25},{x+175,y+25}, {x+175,y+125},{x+75,y+125}};
            fillpoly(polypts,5);        
            x = x + 300;
        }
        x = startXLogicBlock;
        y = y + 200;
    }
    setcolor(BLACK);
    createchannels();
    setcolor(RED);
    setlinewidth (2);
    int count = 0;
    for (int k = 0; k < listofPaths.size(); k++) {
        finalPathToBeRouted = listofPaths[k];            
        if (finalPathToBeRouted.size() > 0) {
            int size = finalPathToBeRouted.size() - 1;
            getSinkLogicBlock(xycordinatelistSink[count].first, xycordinatelistSink[count].second);
            drawline(lblock.listOfPinsOfchannel[findPinIndexFromPinNumber(sinkPinList[count])].xGraphicsCordinate,lblock.listOfPinsOfchannel[findPinIndexFromPinNumber(sinkPinList[count])].yGraphicsCordinate, finalPathToBeRouted[0].xGraphicsCordinate,finalPathToBeRouted[0].yGraphicsCordinate);
            getSourceLogicBlock(xycordinatelistSource[count].first, xycordinatelistSource[count].second);
            drawline(finalPathToBeRouted[size].xGraphicsCordinate,finalPathToBeRouted[size].yGraphicsCordinate, lblock.listOfPinsOfchannel[findPinIndexFromPinNumber(sourcePinList[count])].xGraphicsCordinate,lblock.listOfPinsOfchannel[findPinIndexFromPinNumber(sourcePinList[count])].yGraphicsCordinate);
            for (int i = 0; i < size; i++) { 
                drawline(finalPathToBeRouted[i].xGraphicsCordinate,finalPathToBeRouted[i].yGraphicsCordinate, finalPathToBeRouted[i+1].xGraphicsCordinate, finalPathToBeRouted[i+1].yGraphicsCordinate);
            }   
        }
        count++;
        if (count % 5 ==  0) {
            setcolor (MAGENTA);
        }
        else if (count % 5 ==  1) {
            setcolor (DARKGREEN);
        }
        else if (count % 5 ==  2) {
            setcolor (CYAN);
        }
        else if (count % 5 == 3) {
            setcolor (RED);
        }
        else if (count % 5 == 4) {
            setcolor (DARKGREY);
        }
        else {
            setcolor(BLUE);
        }
        finalPathToBeRouted.clear();
    }
    
}

void getPinOfTopchannel(int i,switchblock_t *sblock, int X, int Y) {

    pin.xGraphicsCordinate = X + 175 - (pinSeparation*i);
    pin.yGraphicsCordinate = Y + 25;
    pin.availablityStatus = PIN_AVAILAIBLE;
    pin.pinNumber = i;
    pin.direction = TOP;
}

void getPinOfBottomchannel(int i, switchblock_t *sblock, int X, int Y) {
    pin.xGraphicsCordinate = X + 175 - (pinSeparation*i);
    pin.yGraphicsCordinate = Y + 125;
    pin.availablityStatus = PIN_AVAILAIBLE;
    pin.pinNumber = i;
    pin.direction = BOTTOM;
}

void getPinOfRightchannel(int i, switchblock_t *sblock, int X, int Y) {
    pin.xGraphicsCordinate = X + 175;
    pin.yGraphicsCordinate = Y + 125 - (pinSeparation*i);
    pin.availablityStatus = PIN_AVAILAIBLE;
    pin.pinNumber = i;
    pin.direction = RIGHT;
}

void getPinOfLeftchannel(int i, switchblock_t *sblock, int X, int Y) {
    
    pin.xGraphicsCordinate = X + 75;
    pin.yGraphicsCordinate = Y + 125 - (pinSeparation*i);
    pin.availablityStatus = PIN_AVAILAIBLE;
    pin.pinNumber = i;
    pin.direction = LEFT;
}

void getTopchannelofSwitchBlock(switchblock_t *s, int X, int Y) {
    
    int top_label;
    int weight1 = switchBlockGridSize + (switchBlockGridSize - 1);
    int weight2 = switchBlockGridSize - 1;
    if (s->yCordinate == switchBlockGridSize - 1) {
        top_label = -1;
    }
    else {
        top_label = s->xCordinate + weight1*s->yCordinate + weight2;
    }
    if (top_label >= 2*(logicBlockGridSize+1)*logicBlockGridSize) {
        top_label = -1;
    }
    seg.xCordinate = s->xCordinate;
    seg.yCordinate = s->yCordinate;
    seg.drawLabel = top_label;
    seg.direction = TOP;
    seg.channelLabel = -1;
    for (int i = widthOfchannel - 1; i >= 0; i--) {
        memset(&pin, 0, sizeof(pin));
        getPinOfTopchannel(i,s,X,Y);
        seg.listOfPinsOfchannel.push_back(pin);
    }
}

void getBottomchannelofSwitchBlock(switchblock_t *s, int X, int Y) {
    
    int bottom_label;
    int weight = switchBlockGridSize + (switchBlockGridSize - 1);
    if (s->yCordinate == 0) {
        bottom_label = -1;
    } else {
        bottom_label = s->xCordinate + weight*s->yCordinate - switchBlockGridSize;
        
    }
    if (bottom_label >= 2*(logicBlockGridSize+1)*logicBlockGridSize) {
        bottom_label = -1;
    }
    seg.xCordinate = s->xCordinate;
    seg.yCordinate = s->yCordinate;
    seg.drawLabel = bottom_label;
    seg.direction = BOTTOM;
    seg.channelLabel = -1;
    for (int i = widthOfchannel - 1; i >= 0; i--) {
        memset(&pin, 0, sizeof(pin));
        getPinOfBottomchannel(i,s,X,Y);
        seg.listOfPinsOfchannel.push_back(pin);
    }
}

void getLeftchannelofSwitchBlock(switchblock_t *s, int X, int Y) {    
    int weight = switchBlockGridSize + (switchBlockGridSize - 1);
    int left_label;
    if (s->xCordinate == 0){
        left_label = -1;
    } else {
        left_label = s->xCordinate + weight*s->yCordinate - 1;
    }
    if (left_label >= 2*(logicBlockGridSize)*(logicBlockGridSize + 1)) {
        left_label = -1;
    }
    seg.xCordinate = s->xCordinate;
    seg.yCordinate = s->yCordinate;
    seg.drawLabel = left_label;
    seg.direction = LEFT;
    seg.channelLabel = -1;
    for (int i = widthOfchannel - 1; i >= 0; i--) {
        memset(&pin, 0, sizeof(pin));
        getPinOfLeftchannel(i,s, X,Y);
        seg.listOfPinsOfchannel.push_back(pin);
    }    
}

void getRightchannelofSwitchBlock(switchblock *s, int X, int Y) {
    int weight = switchBlockGridSize + (switchBlockGridSize - 1);
    int right_label;
    if (s->xCordinate == switchBlockGridSize - 1){
        right_label = -1;
    } else {
        right_label = s->xCordinate + weight*s->yCordinate;
    }
    if (right_label >= 2*(logicBlockGridSize+1)*logicBlockGridSize) {
        right_label = -1;
    }
    seg.xCordinate = s->xCordinate;
    seg.yCordinate = s->yCordinate;
    seg.drawLabel = right_label;
    seg.direction = RIGHT;
    seg.channelLabel = -1;
    for (int i = widthOfchannel - 1; i >= 0; i--) {
        memset(&pin, 0, sizeof(pin));
        getPinOfRightchannel(i,s,X,Y);
        seg.listOfPinsOfchannel.push_back(pin);
    }    
}

bool checkIfEvenPinAvaliable(int channelNumber, switchblock *sblock) {
    for (int i = 0; i < widthOfchannel; i+=2) {
        if (sblock->listOfchannelsOfSwitchBlock[channelNumber].listOfPinsOfchannel[findPinIndexFromPinNumberSwitchBlock(i,channelNumber)].availablityStatus == PIN_AVAILAIBLE)  {
            return true;
        }
    } 
    return false;
}

bool checkIfOddPinAvailable(int channelNumber, switchblock *sblock) {
    for (int i = 1; i < widthOfchannel; i+=2) {
        if (sblock->listOfchannelsOfSwitchBlock[channelNumber].listOfPinsOfchannel[findPinIndexFromPinNumberSwitchBlock(i,channelNumber)].availablityStatus == PIN_AVAILAIBLE)  {
            return true;
        }
    }
    return false;
}

bool checkIfPinAvailable(int channelNumber, switchblock *sblock) {
    for (int i = 0; i < widthOfchannel; i++) {
        if (sblock->listOfchannelsOfSwitchBlock[channelNumber].listOfPinsOfchannel[findPinIndexFromPinNumberSwitchBlock(i,channelNumber)].availablityStatus == PIN_AVAILAIBLE)  {
            return true;
        }
    } 
    return false;
}

void labelTarget(switchblock_t *sblock) {
    for (int i = 0; i < sblock->listOfchannelsOfSwitchBlock.size(); i++) { 
        if (sblock->listOfchannelsOfSwitchBlock[i].drawLabel >= 0) {
            if (sinkPinNumber == 1 || sinkPinNumber == 2 ) {
                if (checkIfEvenPinAvaliable(i,sblock)) {
                    sblock->listOfchannelsOfSwitchBlock[i].channelLabel = channelLabel;
                    if (std::find(listOfChannelLabels.begin(), listOfChannelLabels.end(), sblock->listOfchannelsOfSwitchBlock[i].drawLabel) == listOfChannelLabels.end()) {
                        listOfChannelLabels.push_back(sblock->listOfchannelsOfSwitchBlock[i].drawLabel);
                        expansionList.push_front(std::make_pair(channelLabel,sblock->listOfchannelsOfSwitchBlock[i]));
                        if (!checkIfTargetReached()) {
                            break;
                        }
                    } 
                }
            } else if (sinkPinNumber == 3 || sinkPinNumber == 4 ) {
                if (checkIfOddPinAvailable(i,sblock)) {
                    sblock->listOfchannelsOfSwitchBlock[i].channelLabel = channelLabel;
                    if (std::find(listOfChannelLabels.begin(), listOfChannelLabels.end(), sblock->listOfchannelsOfSwitchBlock[i].drawLabel) == listOfChannelLabels.end()) {
                        listOfChannelLabels.push_back(sblock->listOfchannelsOfSwitchBlock[i].drawLabel);
                        expansionList.push_front(std::make_pair(channelLabel,sblock->listOfchannelsOfSwitchBlock[i]));
                        if (!checkIfTargetReached()) {
                            break;
                        }
                    }
                }

            } else if (sinkPinNumber == 5) {
                if (checkIfPinAvailable(i,sblock)) {                    
                    sblock->listOfchannelsOfSwitchBlock[i].channelLabel = channelLabel;
                    if (std::find(listOfChannelLabels.begin(), listOfChannelLabels.end(), sblock->listOfchannelsOfSwitchBlock[i].drawLabel) == listOfChannelLabels.end()) {
                        listOfChannelLabels.push_back(sblock->listOfchannelsOfSwitchBlock[i].drawLabel);
                        expansionList.push_front(std::make_pair(channelLabel,sblock->listOfchannelsOfSwitchBlock[i]));
                       if (!checkIfTargetReached()) {
                            break;
                        }
                    }
                }
            }

        }
    }
}

void labelchannels(switchblock_t *sblock) {
    for (int i = 0; i < sblock->listOfchannelsOfSwitchBlock.size(); i++) {
        if (sblock->listOfchannelsOfSwitchBlock[i].channelLabel == -1 & sblock->listOfchannelsOfSwitchBlock[i].drawLabel >= 0) {
            if (sinkPinNumber == 1 || sinkPinNumber == 2 ) {
                if (checkIfEvenPinAvaliable(i,sblock)) {
                    sblock->listOfchannelsOfSwitchBlock[i].channelLabel = channelLabel;
                    if (std::find(listOfChannelLabels.begin(), listOfChannelLabels.end(), sblock->listOfchannelsOfSwitchBlock[i].drawLabel) == listOfChannelLabels.end()) {
                        listOfChannelLabels.push_back(sblock->listOfchannelsOfSwitchBlock[i].drawLabel);
                        expansionList.push_front(std::make_pair(channelLabel,sblock->listOfchannelsOfSwitchBlock[i]));
                        if (!checkIfTargetReached()) {
                            break;
                        }
                    } 
                }
            } else if (sinkPinNumber == 3 || sinkPinNumber == 4 ) {
                if (checkIfOddPinAvailable(i,sblock)) {
                    sblock->listOfchannelsOfSwitchBlock[i].channelLabel = channelLabel;
                    if (std::find(listOfChannelLabels.begin(), listOfChannelLabels.end(), sblock->listOfchannelsOfSwitchBlock[i].drawLabel) == listOfChannelLabels.end()) {
                        listOfChannelLabels.push_back(sblock->listOfchannelsOfSwitchBlock[i].drawLabel);
                        expansionList.push_front(std::make_pair(channelLabel,sblock->listOfchannelsOfSwitchBlock[i]));
                        if (!checkIfTargetReached()) {
                            break;
                        }
                    }
                }

            } else if (sinkPinNumber == 5) {
                    if (checkIfPinAvailable(i,sblock)) {
                    sblock->listOfchannelsOfSwitchBlock[i].channelLabel = channelLabel;
                    if (std::find(listOfChannelLabels.begin(), listOfChannelLabels.end(), sblock->listOfchannelsOfSwitchBlock[i].drawLabel) == listOfChannelLabels.end()) {
                        listOfChannelLabels.push_back(sblock->listOfchannelsOfSwitchBlock[i].drawLabel);
                        expansionList.push_front(std::make_pair(channelLabel,sblock->listOfchannelsOfSwitchBlock[i]));
                       if (!checkIfTargetReached()) {
                            break;
                        }
                    }
                }
            }
        }   
    }
}

void resetLabels() {
    for (int i = 0; i < listOfSwitchBlocks.size(); i++) {
        sblock = listOfSwitchBlocks[i];
        for (int i = 0; i < sblock.listOfchannelsOfSwitchBlock.size(); i++) {
            sblock.listOfchannelsOfSwitchBlock[i].channelLabel = -1;
        }
    }
}

void createSwitchBlocks() {
    xGraphicsCordinate = startXLogicBlock;
    yGraphicsCordinate = startYLogicBlock;
    int secondHalfCount = logicBlockGridSize;
    int count = 0;
    for (int y_cordinate = logicBlockGridSize; y_cordinate >=0 ; y_cordinate--) {  
        for (int x_cordinate = 0; x_cordinate < logicBlockGridSize + 1; x_cordinate++) {
            memset(&seg, 0, sizeof(seg));
            memset(&sblock, 0, sizeof(sblock));
            sblock.xCordinate = x_cordinate;
            sblock.yCordinate = y_cordinate;
            sblock.xGraphicsCordinate = xGraphicsCordinate + 75;
            sblock.yGraphicsCordinate = yGraphicsCordinate + 55;
            getTopchannelofSwitchBlock(&sblock,xGraphicsCordinate,yGraphicsCordinate);
            sblock.listOfchannelsOfSwitchBlock.push_back(seg);
            memset(&seg, 0, sizeof(seg));
            getRightchannelofSwitchBlock(&sblock,xGraphicsCordinate,yGraphicsCordinate);
            sblock.listOfchannelsOfSwitchBlock.push_back(seg);
            memset(&seg, 0, sizeof(seg));
            getBottomchannelofSwitchBlock(&sblock,xGraphicsCordinate,yGraphicsCordinate);
            sblock.listOfchannelsOfSwitchBlock.push_back(seg);
            memset(&seg, 0, sizeof(seg));
            getLeftchannelofSwitchBlock(&sblock,xGraphicsCordinate,yGraphicsCordinate);
            sblock.listOfchannelsOfSwitchBlock.push_back(seg);
            listOfSwitchBlocks.push_back(sblock);
            xGraphicsCordinate = xGraphicsCordinate + 300;
        }
        xGraphicsCordinate = startXLogicBlock;
        yGraphicsCordinate = yGraphicsCordinate + 200;
    }
}

void findAdjacentSwitchBlockIndex(switchblock *sblock, int direction, int direction1, bool shortPath) {
    int x, y = 0;
    x = sblock->xCordinate;
    y = sblock->yCordinate;
    int switchBlockIndex;
    int currentdirection;
    int currentdirection1;
    for (int j = 0; j< 4; j++) {
        switchBlockIndex = -1;    
        for (int i = 0; i < listOfSwitchBlocks.size(); i++) {
            switch(j) {
                case TOP:
                    currentdirection = TOP;
                    if (listOfSwitchBlocks[i].xCordinate == x & listOfSwitchBlocks[i].yCordinate == y+1) {
                        switchBlockIndex = i;
                    }
                    break; 
                case RIGHT:
                currentdirection = RIGHT;

                if (listOfSwitchBlocks[i].xCordinate == x+1 & listOfSwitchBlocks[i].yCordinate == y) {
                        switchBlockIndex = i;
                    }
                    break;
                case BOTTOM:
                    currentdirection = BOTTOM;
                    if (listOfSwitchBlocks[i].xCordinate == x & listOfSwitchBlocks[i].yCordinate == y-1) {
                        switchBlockIndex = i;
                    }
                    break;
                case LEFT:
                    currentdirection = LEFT;
                    if (listOfSwitchBlocks[i].xCordinate == x-1 & listOfSwitchBlocks[i].yCordinate == y) {
                        switchBlockIndex = i;
                    }
                    break;
            }
        }
        if (shortPath == true) {
            if (currentdirection == direction1 || currentdirection == direction) {
                if (sinkPinNumber == 1 || sinkPinNumber == 2 ) {
                    if (checkIfOddPinAvailable(currentdirection,sblock)) {
                        isPathAvailable = true;
                        listOfAdjacentSwitchBlocksIndex.push_back(switchBlockIndex);
                    } else {
                        isPathAvailable = false;
                    }
                } else if (sinkPinNumber == 3 || sinkPinNumber == 4 ) {
                    if (checkIfEvenPinAvaliable(currentdirection,sblock)) {
                        isPathAvailable = true;
                        listOfAdjacentSwitchBlocksIndex.push_back(switchBlockIndex);
                    } else {
                        isPathAvailable = false;
                    }
                } else if (sinkPinNumber == 5) {
                    if (checkIfPinAvailable(currentdirection,sblock)) {
                        isPathAvailable = true;
                        listOfAdjacentSwitchBlocksIndex.push_back(switchBlockIndex);
                    }
                    else {
                        isPathAvailable = false;
                    }
                }
            
            }
            
        } else {
            if (sinkPinNumber == 1 || sinkPinNumber == 2 ) {
                    if (checkIfOddPinAvailable(currentdirection,sblock)) {
                        isPathAvailable = true;
                        listOfAdjacentSwitchBlocksIndex.push_back(switchBlockIndex);
                    } else {
                        isPathAvailable = false;
                    }
                } else if (sinkPinNumber == 3 || sinkPinNumber == 4 ) {
                    if (checkIfEvenPinAvaliable(currentdirection,sblock)) {
                        isPathAvailable = true;
                        listOfAdjacentSwitchBlocksIndex.push_back(switchBlockIndex);
                    } else {
                        isPathAvailable = false;
                    }
                } else if (sinkPinNumber == 5) {
                    if (checkIfPinAvailable(currentdirection,sblock)) {
                        isPathAvailable = true;
                        listOfAdjacentSwitchBlocksIndex.push_back(switchBlockIndex);
                    }
                    else {
                        isPathAvailable = false;
                    }
                }
        }

    }
}

logicblock findLogicBlockFromList(int x, int y) {
    int currentLogicBlockIndex = 0;
    for (int i = 0; i < listOfLogicBlocks.size(); i++) {
        if (listOfLogicBlocks[i].xCordinate == x & listOfLogicBlocks[i].yCordinate == y) {
            currentLogicBlockIndex = i;
            break;
        }
    }
    return listOfLogicBlocks[currentLogicBlockIndex];
}

switchblock findSwitchBlockFromList(int x, int y) {
    int currentSwitchBlockIndex = 0;
    for (int i = 0; i < listOfSwitchBlocks.size(); i++) {
        if (listOfSwitchBlocks[i].xCordinate == x & listOfSwitchBlocks[i].yCordinate == y) {
            currentSwitchBlockIndex = i;
            break;
        }
    }
    return listOfSwitchBlocks[currentSwitchBlockIndex];
}


bool checkIfTargetReached() {
    if (tblock.xCordinate == sblock.xCordinate & tblock.yCordinate == sblock.yCordinate) {
        isPathAvailable = true;
        for (int i = 0; i < targetList.size(); i++) {
            if ((std::find(listOfChannelLabels.begin(), listOfChannelLabels.end(), targetList[i].drawLabel) == listOfChannelLabels.end()) ) {
                return false;
            }  
        }
        return false;
    } 
    for (int i = 0; i < targetList.size(); i++) {
        if ((std::find(listOfChannelLabels.begin(), listOfChannelLabels.end(), targetList[i].drawLabel) != listOfChannelLabels.end()) ) {

            return false;
        }  
    }        
    return true;
}

