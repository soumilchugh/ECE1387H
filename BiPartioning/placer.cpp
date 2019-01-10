#include <stdio.h>
#include <iostream>
#include <vector>
#include "graphics.h"
#include <cstdlib>
#include <random>
#include <time.h>       /* time */
#include "umfpack.h"
#include "common.h"
#include <unordered_map>
#include <algorithm>
#include <math.h>
#include <string.h>

using namespace std;
int n;
vector<vector<double> > qmatrix(n, vector<double>(n));
unordered_map<size_t,double> weightMap;
unordered_map<int,std::vector<int> > mapOfBlockNet;
std::unordered_map<int,std::vector<int> > mapOfNetBlock;
unordered_map<size_t, double>::iterator it;
unordered_map<int,std::vector<int> >::iterator it1;
vector <int>::iterator it3;
vector <int> NonZeroElementsIndices;
vector <int > NonZeroElementsCounter;
vector <double> NonZeroElements;
std::vector <std::pair<int,cell>> listOfAllCells;
std::vector <std::pair<int,cell>> listOfMovableCells;
std::queue<std::vector <int> > queueOfMovableCells;
std::queue<std::vector <int> > queueOfTotalMovableCells;
vector <std::pair<int,cell>> finallistOfMovableCells;
std::vector <double> cxlist;
std::vector <double> cylist;
vector <int> sortedMovableCells;
vector <double> yCordinates;
vector <int> indexes;
vector <double> xCordinates;
vector <double> y1Cordinates;
vector <double> hpwLList;
bool isWiresRemoved;
cell_t block;   
double *X;
double *Y;
int *    Ap;
int *    Ai;
double * Ax;
double  *Bx;
double  *By;
double hpwl;
double N;
void *Symbolic, *Numeric ;
int buttonPressCounter;
double *null = (double *) NULL ;
int virtualBlockValue = 0;
double CX = 0;
double CY = 0;
bool isStop = false;
bool isSnapped = false;
bool isSnappedType = false;

inline size_t key(int i,int j) {
    size_t ret = i;
    ret *= 2654435761U;
    return ret ^ j;
}

void assignWeights(unordered_map<int,std::vector<int> > mapOfNetBlock2) {
    int count = 0;
    double value = 0;
    double weight = 0;
    for(unsigned int j = 0; j < mapOfNetBlock2.size(); j++) {
        double numberOfPins = mapOfNetBlock2[j].size();
        weight = 2.0 / numberOfPins;
        if (numberOfPins == 0)
            weight = 0.0;
        if (numberOfPins > 0) {
            for (unsigned int k = 0; k < mapOfNetBlock2[j].size(); k ++) {
                count = k;
                for (int i = count + 1; i < mapOfNetBlock2[j].size(); i ++) {
                    it = weightMap.find(key(mapOfNetBlock2[j][count],mapOfNetBlock2[j][i]));
                    if (it != weightMap.end()) {
                        value = it->second;
                        if (mapOfNetBlock2[j][count] == virtualBlockValue || mapOfNetBlock2[j][i] == virtualBlockValue) {
                            weight = 1;
                        }
                        double total_weight = weight + value;
                        weightMap[key(mapOfNetBlock2[j][count],mapOfNetBlock2[j][i])] = total_weight;
                        weightMap[key(mapOfNetBlock2[j][i],mapOfNetBlock2[j][count])] = total_weight;
                    } else {
                        if (mapOfNetBlock2[j][count] == virtualBlockValue || mapOfNetBlock2[j][i] == virtualBlockValue) {
                            weight = 1;
                        }
                        weightMap[key(mapOfNetBlock2[j][count],mapOfNetBlock2[j][i])] = weight;
                        weightMap[key(mapOfNetBlock2[j][i],mapOfNetBlock2[j][count])] = weight;

                    }
                }
            }
        }
        count = 0;
    }
}

std::unordered_map<int,std::vector<int> > checkCellIfConnectedWithSameLabel(unordered_map<int,std::vector<int> > mapOfBlockNet2, int max) {
    int count = 1;
    int max_value = 0;
    std::unordered_map<int,std::vector<int> > mapOfNetBlock2;
    std::vector<int> listOfBlocks;
    while(count <= max) {
        for (int i = 1; i <= max; i ++) {
            it1 = mapOfBlockNet2.find(i);
            if (it1 != mapOfBlockNet2.end()) {
                for (unsigned int j = 0; j < mapOfBlockNet2[i].size(); j++) {
                    if (count == mapOfBlockNet2[i][j]){
                        listOfBlocks.push_back(i);
                        break;
                    }
                }
            }
        }
        mapOfNetBlock2[count] = listOfBlocks;
        listOfBlocks.clear();
        count++;
    }
    
    return mapOfNetBlock2;
}

void getDiagonalElement(int moveableCell,int x, std::vector<int> allCells1) {
    double weight = 0;
    for (unsigned int i = 0; i < allCells1.size(); ++i) {
        it = weightMap.find(key(moveableCell,allCells1[i]));
        if (it != weightMap.end()) {
            weight += weightMap[key(moveableCell,allCells1[i])];
        }
    } 
    qmatrix[x][x] = weight;
}

void getNonDiagonalElements(int moveableCell, int x, std::vector<int> moveableCells1) {
    int y = 0;
    for (unsigned int i = 0; i < moveableCells1.size(); i++) {
        it = weightMap.find(key(moveableCell,moveableCells1[i]));
        if (it != weightMap.end()) {
            if (x != y)
                qmatrix[x][y] = -1*(weightMap[key(moveableCell,moveableCells1[i])]);
        }
        y++;
    }
}

void buildQMatrix(std::vector<int> moveableCells1,std::vector<int> allCells1) {
    int x = 0;
    for (unsigned int i = 0; i < moveableCells1.size(); ++i) {
        getDiagonalElement(moveableCells1[i],x,allCells1);
        getNonDiagonalElements(moveableCells1[i],x,moveableCells1);
        x++;
    }
}


void countNonZeroElements() {
    int counter = 0;
    NonZeroElementsIndices.clear();
    for (int i = 0; i < (qmatrix.size()); ++i) {
        for (int j = 0; j < (qmatrix.size()); ++j) {
            if (qmatrix[j][i] != 0) {
                counter ++;
                NonZeroElementsIndices.push_back(j);
                NonZeroElements.push_back(qmatrix[j][i]);
            }
        }
        NonZeroElementsCounter.push_back(counter);
    }
}

void loadAp() {
    for (int i = 0; i < NonZeroElementsCounter.size(); ++i ){
        if (i >= 1)
            Ap[i] = NonZeroElementsCounter[i];
        else 
            Ap[i] = 0;
    }   
}

void loadAi() {
    for (int i = 0; i < NonZeroElementsIndices.size(); ++i ){
        Ai[i] = NonZeroElementsIndices[i];
    }
}

void loadAx() {
    for (int i = 0; i < NonZeroElements.size(); ++i ){
        Ax[i] = NonZeroElements[i];
    }
}

void buildBxMatrix(std::vector<int> moveableCells1, std::vector<int> fixedCells1) {
    double weight = 0;
    for (int i = 0; i < moveableCells1.size(); ++i) {
        weight = 0;
        for (int k = 0; k < fixedCells1.size(); k++) {
            it = weightMap.find(key(moveableCells1[i],fixedCells1[k]));
            if (it != weightMap.end()) {
                weight += ((weightMap[key(moveableCells1[i],fixedCells1[k])])*(xycordinatelist[k].first));
            }       
        }
        Bx[i] = weight;
    }    
}

void buildByMatrix(std::vector<int> moveableCells1, std::vector<int> fixedCells1) {
    double weight = 0;
    for (int i = 0; i < moveableCells1.size(); ++i) {
        weight = 0;
        for (int k = 0; k < fixedCells1.size(); k++) {
            it = weightMap.find(key(moveableCells1[i],fixedCells1[k]));
            if (it != weightMap.end()) {
                weight += ((weightMap[key(moveableCells1[i],fixedCells1[k])])*(xycordinatelist[k].second));
            }       
        }
        By[i] = weight;
    } 

}

int linearSolver(std::vector<int> moveableCells, std::vector<int> fixedCells) {
    int i ;
    delete [] Ap;
    delete [] Ai;
    delete [] Ax;
    delete [] Bx;
    delete [] By;
    delete [] X;
    delete [] Y;
    
    Bx = new double[n];
    By = new double[n];
    Ap = new int[NonZeroElementsCounter.size() + 5];
    Ap[0] = 0;
    for (int i = 0; i < NonZeroElementsCounter.size(); i++){
        Ap[i+1] = NonZeroElementsCounter[i];
    }   
    Ai = new int[NonZeroElementsIndices.size()];
    for (int i = 0; i < NonZeroElementsIndices.size(); i++){
        Ai[i] = NonZeroElementsIndices[i];
    }
    Ax = new double[NonZeroElements.size()];
    for (int i = 0; i < NonZeroElements.size(); i++ ){
        Ax[i] = NonZeroElements[i];
    }
    buildBxMatrix(moveableCells,fixedCells);
    buildByMatrix(moveableCells,fixedCells);
    X = new double[n];
    Y = new double[n];
    (void) umfpack_di_symbolic (n, n, Ap, Ai, Ax, &Symbolic, null, null) ;
    (void) umfpack_di_numeric (Ap, Ai, Ax, Symbolic, &Numeric, null, null) ;
    umfpack_di_free_symbolic (&Symbolic) ;
    (void) umfpack_di_solve (UMFPACK_A, Ap, Ai, Ax, X, Bx, Numeric, null, null) ;
    (void) umfpack_di_solve (UMFPACK_A, Ap, Ai, Ax, Y, By, Numeric, null, null) ;
    umfpack_di_free_numeric (&Numeric) ;
    return 0;
}

void loadMovablecells(std::vector<int> moveableCells1) {
    cell_t block;
    for (int i = 0; i < moveableCells1.size(); i++){
        memset(&block,0,sizeof(block));
        block.x = X[i];
        block.y = Y[i];
        block.number = moveableCells1[i];
        listOfAllCells.push_back(std::make_pair(moveableCells1[i],block));
        listOfMovableCells.push_back(std::make_pair(moveableCells1[i],block));
    }
}

void loadFixedCells(std::vector<int> fixedCells1) {
    cell_t block;
    for (int i = 0; i < fixedCells1.size(); i++){
        memset(&block,0,sizeof(block));
        block.x = xycordinatelist[i].first;
        block.y = xycordinatelist[i].second;
        block.number = fixedCells1[i];
        listOfAllCells.push_back(std::make_pair(fixedCells1[i],block));
    }
}


int findIndex(int key,std::vector <std::pair<int,cell>> listOfAllCells1) {
    for (unsigned int i = 0; i < listOfAllCells1.size(); i++) {
        if (listOfAllCells1[i].first == key) {
            return i;
        }
    }
}

void computeHpwL(std::vector <std::pair<int,cell>> listOfAllCells1) {
    hpwLList.clear();
    xCordinates.clear();
    yCordinates.clear();
    double max_x, max_y, min_x, min_y, key, index;
    for (unsigned int i = 1; i < mapOfNetBlock.size(); i++) {
        for (unsigned int j = 0; j < mapOfNetBlock[i].size();j++) {
            key = mapOfNetBlock[i][j];
            index = findIndex(key,listOfAllCells1);
            xCordinates.push_back(listOfAllCells1[index].second.x);
            yCordinates.push_back(listOfAllCells1[index].second.y);
        }
        std::vector<double>::const_iterator it = std::max_element(xCordinates.begin(), xCordinates.end());
        max_x = *it;
        std::vector<double>::const_iterator it1 = std::min_element(xCordinates.begin(), xCordinates.end());
        min_x = *it1;
        xCordinates.clear();
        std::vector<double>::const_iterator it2 = std::max_element(yCordinates.begin(), yCordinates.end());
        max_y = *it2;
        std::vector<double>::const_iterator it3 = std::min_element(yCordinates.begin(), yCordinates.end());
        min_y = *it3;
        yCordinates.clear();
        hpwl = (max_x - min_x) + (max_y - min_y);
        hpwLList.push_back(hpwl);
        hpwl = 0;

    }
    double sumOfElems= 0;
    for(std::vector<double>::iterator it = hpwLList.begin(); it != hpwLList.end(); ++it)
        sumOfElems += *it;
    cout << "HPWL is" << sumOfElems << "\n";
}

void drawSnapGrid() {
    int color = 0;
    int count = 0;
    set_draw_mode (DRAW_NORMAL);    // Should set this if your program does any XOR drawing in callbacks.
    clearscreen();                  /* Should precede drawing for all drawscreens */
    setfontsize (10);
    setlinestyle (SOLID);
    setcolor(BLACK);
    setlinewidth (2);
    double x, y,x1,y1,key,index;
    drawline(0,0,1500,0);
    drawline(1500,0,1500,1500);
    drawline(1500,1500,0,1500);
    drawline(0,1500,0,0);
    x = 0;
    for (int i = 0; i < N; i++) {
        drawline(x,0,x,1500);
        x = x + 1500/N;
    }
    x = 0;
    y = 0;
    for (int i = 0; i < N; i++) {
        drawline(0,y,1500,y);
        y = y + 1500/N;
    }
    if (!isWiresRemoved) {
        for (unsigned int i = 1; i < mapOfNetBlock.size(); i++) {
            for (int j = 0; j < mapOfNetBlock[i].size();j++) {
                key = mapOfNetBlock[i][j];
                index = findIndex(key,finallistOfMovableCells);
                x = (1500/N)*(finallistOfMovableCells[index].second.x);
                y = (1500/N)*(finallistOfMovableCells[index].second.y);
                for (int k = j+1; k < mapOfNetBlock[i].size(); k++) {
                    key = mapOfNetBlock[i][k];
                    index = findIndex(key,finallistOfMovableCells);
                    x1 = (1500/N)*finallistOfMovableCells[index].second.x;
                    y1 = (1500/N)*finallistOfMovableCells[index].second.y;
                   setcolor(LIGHTGREY);
                   drawline(x , y, x1,y1);
                } 
            }
        }
    }
    for (unsigned int i = 1; i < mapOfNetBlock.size(); i++) {
        for (unsigned int j = 0; j < mapOfNetBlock[i].size();j++) {
            key = mapOfNetBlock[i][j];
            index = findIndex(key,finallistOfMovableCells);
            x = (1500/N)*(finallistOfMovableCells[index].second.x);
            y = (1500/N)*(finallistOfMovableCells[index].second.y);
            std::string s = std::to_string(static_cast<long long>(finallistOfMovableCells[index].first));
            char const *pchar = s.c_str();
            setcolor (BLACK);
            setfontsize (10);
            drawtext (x,y - 30,pchar,1200.);            
            setcolor(RED);
            fillarc (x,y,20,0.,360.);
        }
    }
}

void drawSnapGridusingType() {
    int color = 0;
    int count = 0;
    set_draw_mode (DRAW_NORMAL);    // Should set this if your program does any XOR drawing in callbacks.
    clearscreen();                  /* Should precede drawing for all drawscreens */
    setfontsize (10);
    setlinestyle (SOLID);
    setcolor(BLACK);
    setlinewidth (2);
    double x, y,x1,y1,key,index;
    drawline(0,0,1500,0);
    drawline(1500,0,1500,1500);
    drawline(1500,1500,0,1500);
    drawline(0,1500,0,0);
    x = 0;
    for (int i = 0; i < N; i++) {
        drawline(x,0,x,1500);
        x = x + 1500/N;
    }
    x = 0;
    y = 0;
    for (int i = 0; i < N; i++) {
        drawline(0,y,1500,y);
        y = y + 1500/N;
    }
    if (!isWiresRemoved) {
        for (int i = 1; i < mapOfNetBlock.size(); i++) {
            for (int j = 0; j < mapOfNetBlock[i].size();j++) {
                key = mapOfNetBlock[i][j];
                index = findIndex(key,finallistOfMovableCells);
                x = (1500/N)*(finallistOfMovableCells[index].second.x);
                y = (1500/N)*(finallistOfMovableCells[index].second.y);
                for (int k = j+1; k < mapOfNetBlock[i].size(); k++) {
                    key = mapOfNetBlock[i][k];
                    index = findIndex(key,finallistOfMovableCells);
                    x1 = (1500/N)*finallistOfMovableCells[index].second.x;
                    y1 = (1500/N)*finallistOfMovableCells[index].second.y;
                   setcolor(LIGHTGREY);
                   drawline(x , y, x1,y1);
                } 
            }
        }
    }
    for (int i = 1; i < mapOfNetBlock.size(); i++) {
        for (int j = 0; j < mapOfNetBlock[i].size();j++) {
            key = mapOfNetBlock[i][j];
            index = findIndex(key,finallistOfMovableCells);
            x = (1500/N)*(finallistOfMovableCells[index].second.x);
            y = (1500/N)*(finallistOfMovableCells[index].second.y);
            std::string s = std::to_string(static_cast<long long>(finallistOfMovableCells[index].first));
            char const *pchar = s.c_str();
            for (int k = 0; k < cellType.size(); k++) {
                if (cellType[k].first == finallistOfMovableCells[index].first & cellType[k].second == 1) {
                        setcolor(YELLOW);
                        break;
                } else {
                        setcolor(RED);
                }
            }
        
            
            
            fillarc (x,y,20,0.,360.);
            setcolor (BLACK);
            setfontsize (10);
            drawtext (x,y - 30,pchar,1200.);            
            
        }
    }
}


void createSnapGridUsingType() {
    slotMatrix_t slot;
    cell_t block;
    double middleX = 0;
    double middleY = 0;
    double blockX = 0;
    double blockY = 0;
    double distance = 0;
    int type = 0;
    int column = 0;
    bool isSlotTaken = false;
    vector <double> listOfdistance;
    vector <std::pair<int,cell>> listOfMovableCells1;
    bool isTaken = false;
    std::vector<slotMatrix_t> listofslotMatrix;
    finallistOfMovableCells.clear();
    for (int i = 0; i < listOfAllCells.size(); i++) {
        for (int k = 0; k < cellType.size(); k++) {
                if (cellType[k].first == listOfAllCells[i].first & cellType[k].second == 1) {
                    listOfMovableCells1.push_back(make_pair(listOfAllCells[i].first,listOfAllCells[i].second));
                    break;
                }
            }
        
    }
    for (int i = 0; i < N; i ++) {
        for (int j = 0; j < N; j++) {
            memset(&slot,0,sizeof(slot));
            slot.x = i + 0.5;
            slot.y = j + 0.5;
            slot.column = i;
            slot.isSlotTaken = false;
            listofslotMatrix.push_back(slot);
        } 
    }
    for (int i = 0; i < listofslotMatrix.size(); i++) {
        memset(&slot,0,sizeof(slot));

        slot = listofslotMatrix[i];
        column = slot.column;
        if (column > 0 ) {
        if (column == 1 || ((column - 1) % 5 == 0)) {

            middleX = slot.x;
            middleY = slot.y;

            for (int j = 0; j < listOfMovableCells1.size(); j++) {
                memset(&block,0,sizeof(block));
                blockX = 0;
                blockY = 0;
                block = listOfMovableCells1[j].second;
                blockX = block.x;
                blockY = block.y;
                double dist = pow((middleX - blockX), 2) + pow((middleY - blockY), 2);       //calculating Euclidean distance
                distance = sqrt(dist);  
                listOfdistance.push_back(distance);
            
            }
            std::vector<double>::const_iterator it = std::min_element(listOfdistance.begin(), listOfdistance.end());
            int length = std::distance<std::vector<double>::const_iterator>(listOfdistance.begin(), it);
            memset(&block,0,sizeof(block));
            block.x = middleX;
            block.y = middleY;
            finallistOfMovableCells.push_back(make_pair(listOfMovableCells1[length].first,block));
            listOfMovableCells1.erase (listOfMovableCells1.begin()+length);
            listOfdistance.clear();
            
            memset(&slot,0,sizeof(slot));
            slot.x = middleX;
            slot.y = middleY;
            slot.isSlotTaken = true;
            slot.column = column;
            listofslotMatrix[i] = slot;
            if (listOfMovableCells1.size() == 0) {
                break;
            }
        }
    }
    }
    listOfMovableCells1.clear();
    for (int i = 0; i < listOfAllCells.size(); i++) {
        for (int k = 0; k < cellType.size(); k++) {
                if (cellType[k].first == listOfAllCells[i].first & cellType[k].second == 0) {
                    listOfMovableCells1.push_back(make_pair(listOfAllCells[i].first,listOfAllCells[i].second));
                    break;
                }
            }
        
    }

    for (int i = 0; i < listofslotMatrix.size(); i++) {
        memset(&slot,0,sizeof(slot));

        slot = listofslotMatrix[i];

            middleX = slot.x;
            middleY = slot.y;
            isSlotTaken = slot.isSlotTaken;
            if (!isSlotTaken) {
            for (int j = 0; j < listOfMovableCells1.size(); j++) {
                memset(&block,0,sizeof(block));
                blockX = 0;
                blockY = 0;
                block = listOfMovableCells1[j].second;
                blockX = block.x;
                blockY = block.y;
                double dist = pow((middleX - blockX), 2) + pow((middleY - blockY), 2);       //calculating Euclidean distance
                distance = sqrt(dist);  
                listOfdistance.push_back(distance);
            
            }
            std::vector<double>::const_iterator it = std::min_element(listOfdistance.begin(), listOfdistance.end());
            int length = std::distance<std::vector<double>::const_iterator>(listOfdistance.begin(), it);
            memset(&block,0,sizeof(block));
            block.x = middleX;
            block.y = middleY;
            finallistOfMovableCells.push_back(make_pair(listOfMovableCells1[length].first,block));
            listOfMovableCells1.erase (listOfMovableCells1.begin()+length);
            listOfdistance.clear();
            memset(&slot,0,sizeof(slot));
            slot.x = middleX;
            slot.y = middleY;
            slot.isSlotTaken = true;
            slot.column = column;
            listofslotMatrix[i] = slot;
            if (listOfMovableCells1.size() == 0) {
                break;
            }
        }
    }
    drawSnapGridusingType();
    computeHpwL(finallistOfMovableCells);
}


void createSnapGrid() {
    slotMatrix_t slot;
    cell_t block;
     double middleX = 0;
    double middleY = 0;
    double blockX = 0;
    double blockY = 0;
    double distance = 0;
    vector <double> listOfdistance;
    vector <std::pair<int,cell>> listOfMovableCells1;

    finallistOfMovableCells.clear();
    for (int i = 0; i < listOfMovableCells.size(); i++) {
        listOfMovableCells1.push_back(make_pair(listOfMovableCells[i].first,listOfMovableCells[i].second));
    }
    bool isTaken = false;
    std::vector<slotMatrix_t> listofslotMatrix;
    for (int i = 0; i < N; i ++) {
        for (int j = 0; j < N; j++) {
            memset(&slot,0,sizeof(slot));
            slot.x = i + 0.5;
            slot.y = j + 0.5;
            for (int k = 0; k < xycordinatelist.size(); k++) {
                if (xycordinatelist[k].first == slot.x & xycordinatelist[k].second == slot.y) {
                    isTaken = true;
                    break;
                }
            }
            if (!isTaken)
                listofslotMatrix.push_back(slot);
            else {
                isTaken = false;
                continue;
            }
        } 
    }
    for (int i = 0; i < listofslotMatrix.size(); i++) {
        memset(&slot,0,sizeof(slot));

        slot = listofslotMatrix[i];
        middleX = slot.x;
        middleY = slot.y;

        for (int j = 0; j < listOfMovableCells1.size(); j++) {
            memset(&block,0,sizeof(block));
            blockX = 0;
            blockY = 0;
            block = listOfMovableCells1[j].second;
            blockX = block.x;
            blockY = block.y;
            double dist = pow((middleX - blockX), 2) + pow((middleY - blockY), 2);       //calculating Euclidean distance
            distance = sqrt(dist);  
            listOfdistance.push_back(distance);
        }
        std::vector<double>::const_iterator it7 = std::min_element(listOfdistance.begin(), listOfdistance.end());
        int length = std::distance<std::vector<double>::const_iterator>(listOfdistance.begin(), it7);
        memset(&block,0,sizeof(block));
        block.x = middleX;
        block.y = middleY;
        finallistOfMovableCells.push_back(make_pair(listOfMovableCells1[length].first,block));
        listOfMovableCells1.erase (listOfMovableCells1.begin()+length);
        listOfdistance.clear();
    }

    for (int i = 0; i < fixedCells.size(); i++) {
        memset(&block,0,sizeof(block));
        block.x = xycordinatelist[i].first;
        block.y = xycordinatelist[i].second;
        finallistOfMovableCells.push_back(make_pair(fixedCells[i],block));
    }
    drawSnapGrid();
    computeHpwL(finallistOfMovableCells);
}

void createSnapGridWithoutWires() {
    slotMatrix_t slot;
    cell_t block;
     double middleX = 0;
    double middleY = 0;
    double blockX = 0;
    double blockY = 0;
    double distance = 0;
    vector <double> listOfdistance;
    vector <std::pair<int,cell>> listOfMovableCells1;
    finallistOfMovableCells.clear();
    for (int i = 0; i < listOfMovableCells.size(); i++) {
        listOfMovableCells1.push_back(make_pair(listOfMovableCells[i].first,listOfMovableCells[i].second));
    }
    bool isTaken = false;
    std::vector<slotMatrix_t> listofslotMatrix;
    for (int i = 0; i < N; i ++) {
        for (int j = 0; j < N; j++) {
            memset(&slot,0,sizeof(slot));
            slot.x = i + 0.5;
            slot.y = j + 0.5;
            for (int k = 0; k < xycordinatelist.size(); k++) {
                if (xycordinatelist[k].first == slot.x & xycordinatelist[k].second == slot.y) {
                    isTaken = true;
                    break;
                }
            }
            if (!isTaken)
                listofslotMatrix.push_back(slot);
            else {
                isTaken = false;
                continue;
            }
        } 
    }
    for (int i = 0; i < listofslotMatrix.size(); i++) {
        memset(&slot,0,sizeof(slot));

        slot = listofslotMatrix[i];
        middleX = slot.x;
        middleY = slot.y;

        for (int j = 0; j < listOfMovableCells1.size(); j++) {
            memset(&block,0,sizeof(block));
            blockX = 0;
            blockY = 0;
            block = listOfMovableCells1[j].second;
            blockX = block.x;
            blockY = block.y;
            double dist = pow((middleX - blockX), 2) + pow((middleY - blockY), 2);       //calculating Euclidean distance
            distance = sqrt(dist);  
            listOfdistance.push_back(distance);
        }
        std::vector<double>::const_iterator it = std::min_element(listOfdistance.begin(), listOfdistance.end());
        int length = std::distance<std::vector<double>::const_iterator>(listOfdistance.begin(), it);
        memset(&block,0,sizeof(block));
        block.x = middleX;
        block.y = middleY;
        finallistOfMovableCells.push_back(make_pair(listOfMovableCells1[length].first,block));
        listOfMovableCells1.erase (listOfMovableCells1.begin()+length);
        listOfdistance.clear();
    }

    for (int i = 0; i < fixedCells.size(); i++) {
        memset(&block,0,sizeof(block));
        block.x = xycordinatelist[i].first;
        block.y = xycordinatelist[i].second;
        finallistOfMovableCells.push_back(make_pair(fixedCells[i],block));
    }
    drawSnapGrid();
}

void drawscreen() {
    int color = 0;
    set_draw_mode (DRAW_NORMAL);    // Should set this if your program does any XOR drawing in callbacks.
    clearscreen();                  /* Should precede drawing for all drawscreens */
    setfontsize (10);
    setlinestyle (SOLID);
    setcolor(BLACK);
    setlinewidth (2);
    double x, y,x1,y1,key,index;
    drawline(0,0,1500,0);
    drawline(1500,0,1500,1500);
    drawline(1500,1500,0,1500);
    drawline(0,1500,0,0);
    x = 0;
    for (int i = 0; i < N; i++) {
        drawline(x,0,x,1500);
        x = x + 1500/N;
    }
    x = 0;
    y = 0;
    for (int i = 0; i < N; i++) {
        drawline(0,y,1500,y);
        y = y + 1500/N;
    }
    if (!isWiresRemoved) {
        for (int i = 1; i < mapOfNetBlock.size(); i++) {
            for (int j = 0; j < mapOfNetBlock[i].size();j++) {
                key = mapOfNetBlock[i][j];
                index = findIndex(key,listOfAllCells);
                x = (1500/N)*(listOfAllCells[index].second.x);
                y = (1500/N)*(listOfAllCells[index].second.y);
                
               
                for (int k = j+1; k < mapOfNetBlock[i].size(); k++) {
                    key = mapOfNetBlock[i][k];
                    index = findIndex(key,listOfAllCells);
                    x1 = (1500/N)*listOfAllCells[index].second.x;
                    y1 = (1500/N)*listOfAllCells[index].second.y;
                    setcolor(LIGHTGREY);
                    drawline(x , y, x1,y1);
                } 
            }
        }
    }
    for (int i = 1; i < mapOfNetBlock.size(); i++) {
        for (int j = 0; j < mapOfNetBlock[i].size();j++) {
            key = mapOfNetBlock[i][j];
            index = findIndex(key,listOfAllCells);
            x = (1500/N)*(listOfAllCells[index].second.x);
            y = (1500/N)*(listOfAllCells[index].second.y);           
            std::string s = std::to_string(static_cast<long long>(listOfAllCells[index].first));
            char const *pchar = s.c_str();
            setcolor (BLACK);
            setfontsize (10);
            drawtext (x,y-30,pchar,1200.);            
            setcolor(RED);
            fillarc (x,y,20,0.,360.);
        }
    }
    
}


void button_press (float x, float y) {
    /* Called whenever event_loop gets a button press in the graphics *
     * area.  Allows the user to do whatever he/she wants with button *
     * clicks.                                                        */
     printf("User clicked at coordinates (%f, %f)\n", x, y);
}



void solve(std::vector<int> allofCells, int quadrant,std::vector<int> fixedofCells, double Cx, double Cy,double Ns) {
    double xCordinate,yCordinate = 0;
    unordered_map<int,std::vector<int> > mapOfBlockNet1;
    unordered_map<int,std::vector<int> > mapOfNetBlock1;
    std::vector<int> allCells1;
    std::vector<int> data1;
    std::vector<int> fixedCells1;
    NonZeroElementsCounter.clear();
    unordered_map<size_t, double>::iterator it;    
    NonZeroElementsIndices.clear();
    NonZeroElements.clear();
    for (it = weightMap.begin(); it != weightMap.end(); ++it) {
        weightMap.erase(it);
    }
    //auto it5= weightMap.begin();
    //while (it != weightMap.end()) { it5= weightMap.erase(it5);}
    mapOfBlockNet1.clear();
    mapOfNetBlock1.clear();
    qmatrix.clear();
    allCells1.clear();
    data1.clear();
    fixedCells1.clear();
    std::vector<int>::const_iterator it4 = std::max_element(allofCells.begin(), allofCells.end());
    virtualBlockValue = *it4 + 1 + quadrant;
    fixedCells1 = fixedofCells;
    fixedCells1.push_back(virtualBlockValue);
    if (sortedMovableCells.size() > 0) {
        if (quadrant == 1) {
            xCordinate = Cx + Ns/4;
            yCordinate = Cy + Ns/4;
            xycordinatelist.push_back(std::make_pair(xCordinate,yCordinate));
        }
        else if (quadrant == 3) {
            xCordinate = Cx + 3*Ns/4;
            yCordinate = Cy + Ns/4;
            xycordinatelist.push_back(std::make_pair(xCordinate,yCordinate));
        }
        else if (quadrant == 2) {
            xCordinate = Cx + Ns/4;
            yCordinate = Cy + 3*Ns/4;
            xycordinatelist.push_back(std::make_pair(xCordinate,yCordinate));
        }
        else if (quadrant == 4) {
            xCordinate = Cx + 3*Ns/4;
            yCordinate = Cy + 3*Ns/4;
            xycordinatelist.push_back(std::make_pair(xCordinate,yCordinate));

        }
        std::vector<int>::const_iterator it2 = std::max_element(netList.begin(), netList.end());
        int maxNetvalue = *it2 + 1 + quadrant;
        
        for (int i = 0; i < sortedMovableCells.size();i++) {
            allCells1.push_back(sortedMovableCells[i]);
        }
        for (int i = 0; i < fixedCells1.size(); i++) {
            allCells1.push_back(fixedCells1[i]);
        }
        for (int i = 0; i < allCells1.size(); i++) {
            it1 = mapOfBlockNet.find(allCells1[i]);
            if (it1 != mapOfBlockNet.end()) {
                mapOfBlockNet1[allCells1[i]] = mapOfBlockNet[allCells1[i]];
                if (std::find(fixedCells.begin(), fixedCells.end(), allCells1[i]) == fixedCells.end()) {
                    mapOfBlockNet1[allCells1[i]].push_back(maxNetvalue);
                }
            } 
        }
        data1.push_back(maxNetvalue);
        mapOfBlockNet1[virtualBlockValue] = data1;
        n = sortedMovableCells.size();
        it3 = std::max_element(allCells1.begin(), allCells1.end());
        mapOfNetBlock1 = checkCellIfConnectedWithSameLabel(mapOfBlockNet1,maxNetvalue);
        qmatrix.resize(n);
        for (int i = 0; i < n; ++i)
            qmatrix[i].resize(n);
        assignWeights(mapOfNetBlock1);    
        buildQMatrix(sortedMovableCells,allCells1);
        countNonZeroElements();
       
        linearSolver(sortedMovableCells,fixedCells1);
        xycordinatelist.pop_back();
        for (int i = 0; i < sortedMovableCells.size(); i++) {
            for (int j = 0; j < listOfAllCells.size(); j++) {
                if (sortedMovableCells[i] == listOfAllCells[j].first) {
                    memset(&block,0,sizeof(block));
                    block = listOfAllCells[j].second;
                    block.x = X[i];
                    block.y = Y[i];
                    listOfAllCells[j].second = block;
                    listOfMovableCells[j].second = block;
                    break;
                } 
            }
        }
    }
    return;
}

void sort(std::vector<int> split,int quadrant,double Ns,double Cx, double Cy) {
    int average;
    sortedMovableCells.clear();
    yCordinates.clear();
    indexes.clear();
    int count = 0;
    if (split.size() != 0) {
        for (int i = 0; i < split.size();i++){
            yCordinates.push_back(listOfMovableCells[split[i]].second.y);
        }
        count = yCordinates.size();
        while (count != 0) {
            std::vector<double>::const_iterator it = std::min_element(yCordinates.begin(), yCordinates.end());
            double min_value = *it;
            int index = std::distance<std::vector<double>::const_iterator>(yCordinates.begin(), it);
            yCordinates.erase(yCordinates.begin() + index);
            count --;
            for (int i = 0; i < y1Cordinates.size();i++){ 
                if (y1Cordinates[i] == min_value) {
                    indexes.push_back(i);
                }
            }
        }
        average = indexes.size()/2; 
        if (split.size() >= 0) {
            std::vector<int> splity_lo(indexes.begin(), indexes.begin() + average);
            std::vector<int> splity_hi(indexes.begin() + average, indexes.end());

            for (int i = 0; i < splity_lo.size(); i++) {
                sortedMovableCells.push_back(listOfMovableCells[splity_lo[i]].first);
            }
            queueOfMovableCells.push(sortedMovableCells);
            if (quadrant == 1) {
                CX = Cx;
                CY = Cy;
            }
            else if (quadrant == 3) {
                CX = Cx + Ns;
                CY = Cy;
            }
            solve(allCells,quadrant,fixedCells,CX,CY, Ns);

            cylist.push_back(CY);
            cxlist.push_back(CX);
            sortedMovableCells.clear();
            quadrant++;
            for (int i = 0; i < splity_hi.size(); i++) {
                sortedMovableCells.push_back(listOfMovableCells[splity_hi[i]].first);
            }
            if (quadrant == 2) {
                
                CX = Cx;
                CY = Cy + Ns;
            }
            else if (quadrant == 4) {
                CX = Cx + Ns;
                CY = Cy + Ns;
            }
            cylist.push_back(CY);
            cxlist.push_back(CX);
            solve(allCells,quadrant,fixedCells,CX,CY, Ns);
            queueOfMovableCells.push(sortedMovableCells);
        } else {
            sortedMovableCells.push_back(listOfMovableCells[split[0]].first);
            queueOfMovableCells.push(sortedMovableCells);

        }
    } else {
        queueOfMovableCells.push(sortedMovableCells);
    }

}

void spread(std::vector<int> moveableCells, double Ns, double cx, double cy) {
    vector <double> x1Cordinates;

    xCordinates.clear();
    x1Cordinates.clear();
    y1Cordinates.clear();
    indexes.clear();
    int count = 0;
    for (unsigned int i = 0; i < moveableCells.size();i++){
        for (unsigned int j = 0; j < listOfMovableCells.size(); j++) {
            if (moveableCells[i] == listOfMovableCells[j].first) {
                xCordinates.push_back(listOfMovableCells[j].second.x);
            }
        }
    }
    for (unsigned int i = 0; i < listOfMovableCells.size(); i++) {
        x1Cordinates.push_back(listOfMovableCells[i].second.x);
        y1Cordinates.push_back(listOfMovableCells[i].second.y);
    }
    count = xCordinates.size();
    while (count != 0) {
        std::vector<double>::const_iterator it = std::min_element(xCordinates.begin(), xCordinates.end());
        double min_value = *it;
        int index = std::distance<std::vector<double>::const_iterator>(xCordinates.begin(), it);
        xCordinates.erase(xCordinates.begin() + index);
        count --;
        for (unsigned int i = 0; i < x1Cordinates.size();i++){ 
            if (x1Cordinates[i] == min_value) {
                indexes.push_back(i);
            }
        }
    }
    int average = indexes.size()/2; 
    std::vector<int> split_lo(indexes.begin(), indexes.begin() + average);
    std::vector<int> split_hi(indexes.begin() + average, indexes.end());

    sort(split_lo,1,Ns,cx,cy);
    sort(split_hi,3,Ns,cx,cy);
    return;
}

void spreading() {
    n = moveableCells.size();
    std::vector<int>::const_iterator it3 = std::max_element(netList.begin(), netList.end());
    mapOfNetBlock = checkCellIfConnectedWithSameLabel(mapOfBlockNet,*it3);
    qmatrix.resize(n);
    for (int i = 0; i < n; ++i)
        qmatrix[i].resize(n);
    assignWeights(mapOfNetBlock);
    buildQMatrix(moveableCells,allCells);
    countNonZeroElements();
    linearSolver(moveableCells,fixedCells);
}

void snapCallback (void (*drawscreen_ptr) (void)) {
    isSnapped = true;
    createSnapGrid();
}

void checkIfSpreadingCondition() {
    slotMatrix_t slot;
    int counter = 0;
    double middleX = 0;
    double middleY = 0;
    vector <double> listOfdistance;
    vector <std::pair<int,cell>> listOfMovableCells1;
    vector <std::pair<int,cell>> finallistOfMovableCells;

    for (unsigned int i = 0; i < listOfMovableCells.size(); i++) {
        listOfMovableCells1.push_back(make_pair(listOfMovableCells[i].first,listOfMovableCells[i].second));
    }
    bool isTaken = false;
    std::vector<slotMatrix_t> listofslotMatrix;
    for (int i = 0; i < N; i ++) {
        for (int j = 0; j < N; j++) {
            memset(&slot,0,sizeof(slot));
            slot.x = i;
            slot.y = j;
            for (unsigned int k = 0; k < xycordinatelist.size(); k++) {
                if ((xycordinatelist[k].first == slot.x) & (xycordinatelist[k].second == slot.y)) {
                    isTaken = true;
                    break;
                }
            }
            if (!isTaken)
                listofslotMatrix.push_back(slot);
            else {
                isTaken = false;
                continue;
            }
        } 
    }
    int internalCounter = 0;
    for (unsigned int i = 0; i < listofslotMatrix.size(); i++) {
        memset(&slot,0,sizeof(slot));
        slot = listofslotMatrix[i];
        middleX = slot.x;
        middleY = slot.y;
        internalCounter = 0;
        for (unsigned int i = 0; i < listOfMovableCells.size(); i++) {
            if ((listOfMovableCells[i].second.x <= middleX + 1) & ((listOfMovableCells[i].second.x >= middleX))) {
                if ((listOfMovableCells[i].second.y <= middleY + 1) & (listOfMovableCells[i].second.y >= middleY)) {
                    internalCounter ++;
                }
            }
        }
        if ((internalCounter < 4) & (internalCounter > 1)) {
            counter ++;
        }
    }
    if (counter > 0.15*listOfMovableCells.size()) {
        isStop = true;
    }
}

void spreadingCallback (void (*drawscreen_ptr) (void)) {
    printf ("You pressed the spread button!\n");
    if (!isStop) {
        double cx = 0;
        double cy = 0;
        double Ns = 0;
        std::vector<int> split_vector;
        buttonPressCounter++;
        Ns = N/(pow(2,buttonPressCounter));
        queueOfTotalMovableCells = queueOfMovableCells;
        queueOfMovableCells = queue<std::vector <int> >();
        if ((queueOfTotalMovableCells.size() == 0) & (buttonPressCounter == 1)){
            spread(moveableCells,Ns,0,0);
        }
        else if (queueOfTotalMovableCells.size() > 0) {
            while (!queueOfTotalMovableCells.empty()) {
                split_vector.clear();
                split_vector = queueOfTotalMovableCells.front();
                queueOfTotalMovableCells.pop();
                cx = cxlist.front();
                cy = cylist.front();
                cxlist.erase(cxlist.begin());
                cylist.erase(cylist.begin());
                spread(split_vector,Ns,cx,cy);
            }
        }
        else {
            return;
        }
        computeHpwL(listOfAllCells);
        checkIfSpreadingCondition();
        drawscreen();
    } else {
        cout << "stopping criteria met" << "\n";
    }
}

void removeNetsCallback (void (*drawscreen_ptr) (void)) {
    isWiresRemoved = true;
    if (!isSnapped & !isSnappedType) {
    set_draw_mode (DRAW_NORMAL);    // Should set this if your program does any XOR drawing in callbacks.
    clearscreen();                  /* Should precede drawing for all drawscreens */
    setfontsize (10);
    setlinestyle (SOLID);
    setcolor(BLACK);
    setlinewidth (2);
    double x, y,key,index;
    drawline(0,0,1500,0);
    drawline(1500,0,1500,1500);
    drawline(1500,1500,0,1500);
    drawline(0,1500,0,0);
    x = 0;
    for (int i = 0; i < N; i++) {
        drawline(x,0,x,1500);
        x = x + 1500/N;
    }
    x = 0;
    y = 0;
    for (int i = 0; i < N; i++) {
        drawline(0,y,1500,y);
        y = y + 1500/N;
    }
    for (unsigned int i = 1; i < mapOfNetBlock.size(); i++) {
        for (unsigned int j = 0; j < mapOfNetBlock[i].size();j++) {
            key = mapOfNetBlock[i][j];
            index = findIndex(key,listOfAllCells);
            x = (1500/N)*(listOfAllCells[index].second.x);
            y = (1500/N)*(listOfAllCells[index].second.y);
            std::string s = std::to_string(static_cast<long long>(listOfAllCells[index].first));
            char const *pchar = s.c_str();
            setcolor (BLACK);
            setfontsize (10);
            drawtext (x,y-30,pchar,1200.);            
            setcolor(RED);
            fillarc (x,y,20,0.,360.);
        }
    } }
    else if (isSnapped) {
        createSnapGridWithoutWires();
    } else if (isSnappedType) {
        drawSnapGridusingType();

    }
}

void snapTypeCallback (void (*drawscreen_ptr) (void)) {
    isSnappedType = true;
    createSnapGridUsingType();
}

void addNetsCallback (void (*drawscreen_ptr) (void)) {
    isWiresRemoved = false;
    if (!isSnapped & !isSnappedType) {
        set_draw_mode (DRAW_NORMAL);    // Should set this if your program does any XOR drawing in callbacks.
        clearscreen();                  /* Should precede drawing for all drawscreens */
        setfontsize (10);
        setlinestyle (SOLID);
        setcolor(BLACK);
        setlinewidth (2);
        double x, y,x1,y1,key,index;
        drawline(0,0,1500,0);
        drawline(1500,0,1500,1500);
        drawline(1500,1500,0,1500);
        drawline(0,1500,0,0);
        x = 0;
        for (int i = 0; i < N; i++) {
            drawline(x,0,x,1500);
            x = x + 1500/N;
        }
        x = 0;
        y = 0;
        for (int i = 0; i < N; i++) {
            drawline(0,y,1500,y);
            y = y + 1500/N;
        }
        for (unsigned int i = 1; i < mapOfNetBlock.size(); i++) {
            for (unsigned int j = 0; j < mapOfNetBlock[i].size();j++) {
                key = mapOfNetBlock[i][j];
                index = findIndex(key,listOfAllCells);
                x = (1500/N)*(listOfAllCells[index].second.x);
                y = (1500/N)*(listOfAllCells[index].second.y);
                
               
                for (unsigned int k = j+1; k < mapOfNetBlock[i].size(); k++) {
                    key = mapOfNetBlock[i][k];
                    index = findIndex(key,listOfAllCells);
                    x1 = (1500/N)*listOfAllCells[index].second.x;
                    y1 = (1500/N)*listOfAllCells[index].second.y;
                    setcolor(LIGHTGREY);
                    drawline(x , y, x1,y1);
                } 
            }
        }
        for (unsigned int i = 1; i < mapOfNetBlock.size(); i++) {
            for (unsigned int j = 0; j < mapOfNetBlock[i].size();j++) {
                key = mapOfNetBlock[i][j];
                index = findIndex(key,listOfAllCells);
                x = (1500/N)*(listOfAllCells[index].second.x);
                y = (1500/N)*(listOfAllCells[index].second.y);
                std::string s = std::to_string(static_cast<long long>(listOfAllCells[index].first));
                char const *pchar = s.c_str();
                setcolor (BLACK);
                setfontsize (10);
                drawtext (x,y-30,pchar,1200.);            
                setcolor(RED);
                fillarc (x,y,20,0.,360.);
            }
        }
    } else if (isSnapped){
        createSnapGrid();
    } else if (isSnappedType) {
        drawSnapGridusingType();
    }
}


int main(int argc, char *argv[]) {
    if (argc > 1) {
        dataParser(argv[1]);
    } else {
        cerr << "No filename mentioned" << "\n";
        exit(EXIT_FAILURE);
    }
    spreading();
    loadMovablecells(moveableCells);
    loadFixedCells(fixedCells);
    N = ceil(sqrt(listOfAllCells.size()));
    cout << "N is " << N << "\n";
    computeHpwL(listOfAllCells);
    buttonPressCounter = 0;
    init_graphics("Placement", WHITE);
    init_world (0.,0.,1500.,1500.);
    update_message("Placement");
    create_button ("Proceed", "Spreading", spreadingCallback);
    create_button ("Spreading", "Snap", snapCallback);
    create_button ("Snap", "Remove Nets", removeNetsCallback);
    create_button ("Remove Nets", "Add nets", addNetsCallback);
    create_button ("Add nets", "Snap Type", snapTypeCallback);

    event_loop(button_press,NULL, NULL,drawscreen);
   	close_graphics();
   	printf ("Graphics closed down.\n");
    delete [] Ap;
    delete [] Ai;
    delete [] Ax;
    delete [] Bx;
    delete [] By;
    delete [] X;
    delete [] Y;
    return 0;
}
