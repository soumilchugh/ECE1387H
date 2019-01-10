//
//  main.cpp
//  Partitioning
//
//  Created by soumil chugh on 2018-11-18.
//  Copyright © 2018 soumil chugh. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <random>
#include <time.h>       /* time */
#include "common.h"
#include <unordered_map>
#include <algorithm>
#include <math.h>
#include <string.h>
#include "graphics.h"
#include <chrono>

using namespace std;
std::vector<vector<int> > best;

std::unordered_map<int,std::vector<int> > mapOfBlockNet;
std::unordered_map<int,std::vector<int> > mapOfNetBlock;
std::vector<int> allCells;
std::vector <int> netList;
std::vector <std::vector<int> > listOfSplit;
vector <int>::iterator netListIterator;
unordered_map<int,std::vector<int> >::iterator mapOfBlockNetIterator;
unordered_map<int, std::vector<int> >::iterator mapOfNetBlockIterator;
unordered_map<int, int >::iterator mapSortIterator;
std::vector<node_t> listOfNodes;
std::vector<int> split_low;
std::vector<int> split_medium;
std::vector<int> split_high;
std::vector <int> listofCells;
int counter = 0;
std::vector<node_t>::iterator nodeIterator;
std::vector <int> sortedCells;
std::vector <int> listOfNets;
int initialBestSolution = 0;
int max_depth = 0;
float dy = 0;
float difference = 0;
int max_value = 0;
std::vector<int> listOfBlocks;
    bool Lowflag = false;
    bool Highflag = false;
    bool Mediumflag = false;
    bool Lowflag1 = false;
    bool Highflag1 = false;
    bool Mediumflag1 = false;
    int crossing_count = 0;
    bool isFound = false;
struct greaterValue
{
    template<class T>
    bool operator()(T const &a, T const &b) const { return a > b; }
};


 void checkCellIfConnectedWithSameLabel(unordered_map<int,std::vector<int> > mapOfBlockNet2) {
    int count = 1;
    std::vector<int> listOfBlocks;
    while(count <= max_value) {
        cout << "Net Number " << count << ": ";
        for (int i = 1; i <= max_value; i ++) {
            mapOfBlockNetIterator = mapOfBlockNet2.find(i);
            if (mapOfBlockNetIterator != mapOfBlockNet2.end()) {
                for (unsigned int j = 0; j < mapOfBlockNet2[i].size(); j++) {
                    if (count == mapOfBlockNet2[i][j]){
                        cout << i << ",";
                        listOfBlocks.push_back(i);
                        break;
                    }
                }
            }
        }
        cout << "\n";
        mapOfNetBlock[count] = listOfBlocks;
        listOfBlocks.clear();
        count++;
    }
}

void sortDecreasingOrder(unordered_map<int,std::vector<int> > mapOfBlockNet) {
    sortedCells.clear();
    listOfNets.clear();
    std::unordered_map<int,int > mapOfNumberOfNets;
    std::unordered_map<int,int >::iterator mapOfNumberOfNetsIterator;
    for (int i = 0; i < allCells.size(); i++) {
        mapOfNumberOfNets[allCells[i]] = mapOfBlockNet[allCells[i]].size();
        listOfNets.push_back(mapOfBlockNet[allCells[i]].size());
    }
    std::stable_sort(listOfNets.begin(), listOfNets.end(),greaterValue());    
    for (int i = 0; i < listOfNets.size(); i++) {
        for (mapOfNumberOfNetsIterator = mapOfNumberOfNets.begin(); mapOfNumberOfNetsIterator!=mapOfNumberOfNets.end(); ++mapOfNumberOfNetsIterator) {
            if (mapOfNumberOfNetsIterator->second == listOfNets[i]) {
                sortedCells.push_back(mapOfNumberOfNetsIterator->first);
                mapOfNumberOfNets.erase(mapOfNumberOfNetsIterator);
                break;
            }
        }
    }
    
    for (int i = 0; i < sortedCells.size(); i++) {
        cout << sortedCells[i] << "\n";
    }
}

void sortIncreasingOrder(unordered_map<int,std::vector<int> > mapOfBlockNet) {
    sortedCells.clear();
    listOfNets.clear();
    std::unordered_map<int,int > mapOfNumberOfNets;
    std::unordered_map<int,int >::iterator mapOfNumberOfNetsIterator;
    for (int i = 0; i < allCells.size(); i++) {
        //cout << "Cell number " << allCells[i] << ": ";
        //cout << mapOfBlockNet[allCells[i]].size();
        mapOfNumberOfNets[allCells[i]] = mapOfBlockNet[allCells[i]].size();
        listOfNets.push_back(mapOfBlockNet[allCells[i]].size());
        // cout << "\n";
    }
    //sortedCells = allCells;
    std::stable_sort(listOfNets.begin(), listOfNets.end());
    //std::random_shuffle(listOfNets.begin(), listOfNets.end());
    
    for (int i = 0; i < listOfNets.size(); i++) {
        // cout << "List of nets is" << listOfNets[i] << "\n";
        for (mapOfNumberOfNetsIterator = mapOfNumberOfNets.begin(); mapOfNumberOfNetsIterator!=mapOfNumberOfNets.end(); ++mapOfNumberOfNetsIterator) {
            if (mapOfNumberOfNetsIterator->second == listOfNets[i]) {
                sortedCells.push_back(mapOfNumberOfNetsIterator->first);
                mapOfNumberOfNets.erase(mapOfNumberOfNetsIterator);
                break;
            }
        }
    }
    
    /*for (int i = 0; i < sortedCells.size(); i++) {
        cout << sortedCells[i] << "\n";
    }*/
}



void partition() {
    std::size_t const oneThirdSize = sortedCells.size() / 3;
    std::vector<int> split_lo(sortedCells.begin(), sortedCells.begin() + oneThirdSize);
    split_low = split_lo;
    std::vector<int> split_med(sortedCells.begin() + oneThirdSize, sortedCells.begin() + 2*oneThirdSize);
    split_medium = split_med;
    std::vector<int> split_hi(sortedCells.begin() + 2*oneThirdSize, sortedCells.end());
    split_high = split_hi;
    for (int i = 0; i < split_hi.size(); i++) {
        cout << "Split hi " << split_hi[i] << "\n";
    }
    
    for (int i = 0; i < split_med.size(); i++) {
        cout << "split_med " << split_med[i] << "\n";
    }
    
    for (int i = 0; i < split_lo.size(); i++) {
        cout << "split_lo " << split_lo[i] << "\n";
    }
    
}

bool checkIfElementPresentinLow(int data) {
    //cout << "Data is" << data << "\n";
    if (std::find(split_low.begin(), split_low.end(), data) != split_low.end()) {
        //    cout << "Element present" << "\n";
        return true;
    }
    return false;
}

bool checkIfElementPresentinMedium(int data) {
    if (std::find(split_medium.begin(), split_medium.end(), data) != split_medium.end()) {
        return true;
    }
    return false;
}

bool checkIfElementPresentinHigh(int data) {
   // cout << "Data is" << data << "\n";
    
    if (std::find(split_high.begin(), split_high.end(), data) != split_high.end()) {
     //   cout << "Element present" << "\n";
        
        return true;
    }
    return false;
}

bool checkIfElementPresentinLeft(int data, vector<int> leftList) {
    if (std::find(leftList.begin(), leftList.end(), data) != leftList.end()) {
        return true;
    }
    return false;
}

bool checkIfElementPresentinMiddle(int data, vector<int> middleList) {
    if (std::find(middleList.begin(), middleList.end(), data) != middleList.end()) {
        return true;
    }
    return false;
}

bool checkIfElementPresentinRight(int data, vector<int> rightList) {
    if (std::find(rightList.begin(), rightList.end(), data) != rightList.end()) {
        return true;
    }
    return false;
}

int getNumberOfCuts(vector<int> left,vector<int> middle, vector<int> right) {
    listOfBlocks.clear();
    Lowflag = false;
    Highflag = false;
    Mediumflag = false;
    Lowflag1 = false;
    Highflag1 = false;
    Mediumflag1 = false;
    crossing_count = 0;
    isFound = false;
    std::unordered_map<int,std::vector<int> > mapOfBlockNet1;
	std::unordered_map<int,std::vector<int> > mapOfNetBlock1;
    
    for (int i = 0; i < left.size(); i++) {
        mapOfBlockNet1[left[i]] = mapOfBlockNet[left[i]];
    }
    for (int i = 0; i < middle.size(); i++) {
        mapOfBlockNet1[middle[i]] = mapOfBlockNet[middle[i]];
    }
    for (int i = 0; i < right.size(); i++) {
        mapOfBlockNet1[right[i]] = mapOfBlockNet[right[i]];
    }
        
    for (int i = 1; i <= max_value; i++) {
        
        mapOfNetBlockIterator = mapOfNetBlock.find(i);
        isFound = false;
        if (mapOfNetBlockIterator != mapOfNetBlock.end()) {
        	//cout << "Net number is " << i << "\n";
            for (int k = 0; k < mapOfNetBlockIterator->second.size(); k++) {
                Lowflag = checkIfElementPresentinLeft(mapOfNetBlockIterator->second[k],left);
                Highflag = checkIfElementPresentinRight(mapOfNetBlockIterator->second[k],right);
                Mediumflag = checkIfElementPresentinMiddle(mapOfNetBlockIterator->second[k],middle);

                for (int j = k+1; j < mapOfNetBlockIterator->second.size(); j ++) {
                    if (Lowflag) {
                        Highflag1 = checkIfElementPresentinRight(mapOfNetBlockIterator->second[j],right);
                        Mediumflag1 = checkIfElementPresentinMiddle(mapOfNetBlockIterator->second[j],middle);
                        if (Highflag1 == 1 | Mediumflag1 == 1) {
                            crossing_count++;
                            isFound = true;
                            break;
                        }
                    }
                    else if (Highflag) {
                        Lowflag1 = checkIfElementPresentinLeft(mapOfNetBlockIterator->second[j],left);
                        Mediumflag1 = checkIfElementPresentinMiddle(mapOfNetBlockIterator->second[j],middle);
                        if (Lowflag1 == 1 | Mediumflag1 == 1) {
                            crossing_count++;
                            isFound = true;
                            break;
                        }
                    }
                    else if (Mediumflag) {
                        Lowflag1 = checkIfElementPresentinLeft(mapOfNetBlockIterator->second[j],left);
                        Highflag1 = checkIfElementPresentinRight(mapOfNetBlockIterator->second[j],right);
                        if (Lowflag1 == 1 | Highflag1 == 1) {
                            crossing_count++;
                            isFound = true;
                            break;
                        }
                    }
                }
                
                if (isFound) {
                    break;
                }
              
            }
        }
        
    }
    return crossing_count;
    
}

void draw(float x, float y, int color, float parentX, float parentY) {
	if (color == 0) {
		setcolor(RED);
	} else if (color == 1) {
		setcolor(GREEN);
	} else {
		setcolor(DARKGREY);
	}
	fillarc (x,y,20,0.,360.);
	drawline(x,y,parentX,parentY);


}

vector<int> bestSolutionVector;

long nodeCount = 0;

vector<int> bestLeftData;
vector<int> bestRightData;
vector<int> bestMiddleData;

void buildTree(int leftCount, int rightCount, int middleCount, int depth, int lowerBoundValue, std::vector<int> leftLists, std::vector<int> rightLists, std::vector<int> middleLists, bool isLeftConditionMet,bool isRightConditionMet,bool isMiddleConditionMet, float parentX, float parentY,int color) {
    
    int lowBoundValue = 0;
    int bestSolution = 0;
    float x = 0;
    float y = 0;
    float n = pow(3,(max_depth - depth - 2));
    bool noMiddle = false;
    if ((depth == (max_depth)) & (lowerBoundValue <= initialBestSolution)) {
    	isLeftConditionMet = true;
    	isRightConditionMet = true;
    	isMiddleConditionMet = true;
        if (leftLists.size() == split_low.size() & rightLists.size() == split_high.size() & middleLists.size() == split_medium.size() ) {
            bestSolution = lowerBoundValue;
            bestSolutionVector.push_back(bestSolution);
            cout << "Best Solution" << bestSolution;
            cout << "Node count is" << nodeCount << "\n";
            initialBestSolution = bestSolution;
            best.push_back(leftLists);
            best.push_back(middleLists);
            best.push_back(rightLists);
            bestLeftData = leftLists;
            bestRightData = rightLists;
            bestMiddleData = middleLists;
        }
    } 
    if (middleCount == 0 & rightCount == 0) {
    	noMiddle = true;

    }
    if (middleCount == 0 & leftCount == 0) {
    	noMiddle = true;

    }
    if (depth != max_depth) {
    	std::string s = std::to_string(static_cast<long long>(sortedCells[depth]));
        char const *pchar = s.c_str();
    	setcolor (BLACK);
        setfontsize (10);
        drawtext (parentX,parentY-30,pchar,1200.); 
        if (color == 1) 
    		draw(parentX,parentY,1,parentX,parentY);
    	else if (color == 2) {
    		draw(parentX,parentY,2,parentX,parentY);
    	} else {
    		draw(parentX,parentY,0,parentX,parentY);
    	}
	}			
    
    if ((leftCount >= split_low.size()) & (isLeftConditionMet == false)) {
        isLeftConditionMet = true;
    }
    if ((middleCount >= split_medium.size()) & (isMiddleConditionMet == false)) {
        isMiddleConditionMet = true;
    }
    if ((rightCount >= split_high.size()) & (isRightConditionMet == false)) {
        isRightConditionMet = true;
    }


    if (isLeftConditionMet == false) {
        vector<int> leftPassList = leftLists;
        leftPassList.push_back(sortedCells[depth]);
        if (isMiddleConditionMet == true & isRightConditionMet == true) {
            if (leftPassList.size() == (split_low.size() - 1)) {
                if( equal(bestLeftData.begin(), bestLeftData.end(), leftPassList.begin()) ) {
                    isLeftConditionMet = true;
                }
            }

        }
        if (isLeftConditionMet == false) {
        	x = parentX - (difference*n);
        	y = parentY + dy;
        	if (depth != max_depth - 1) {
        		nodeCount++;
	        	draw(x,y,0,parentX,(parentY));
        	}

            lowBoundValue = getNumberOfCuts(leftPassList,middleLists, rightLists); 			
            if (lowBoundValue >= (initialBestSolution)) {
            } else {
                buildTree((leftCount+1) ,rightCount,middleCount, (depth+1),lowBoundValue,leftPassList, rightLists, middleLists,isLeftConditionMet, isRightConditionMet,isMiddleConditionMet,x,y,1);
            }
        }
        leftPassList.clear();
    }
    x = 0;
    y = 0;
    	if (noMiddle == false) {

	    if (isMiddleConditionMet == false) {

	        vector<int> middlePassList = middleLists;
	        middlePassList.push_back(sortedCells[depth]);
	        if (isLeftConditionMet & isRightConditionMet ) {
	            if (middlePassList.size() == (split_medium.size() - 2)) {
	                if( equal(bestMiddleData.begin(), bestMiddleData.end(), middlePassList.begin()) ) {
	                    isMiddleConditionMet = true;
	                }
	            }
	        }
	        if (isMiddleConditionMet == false) {

	           	x = parentX;
                y = parentY + dy;
	        	if (depth != max_depth - 1) {
	        		nodeCount++;
	        		draw(x,y,1,parentX,(parentY));
        		}

	            lowBoundValue = getNumberOfCuts(leftLists,middlePassList,rightLists);

	            if (lowBoundValue >= (initialBestSolution)) {
	            } else {
	                buildTree(leftCount,rightCount,(middleCount +1), (depth+1),lowBoundValue,leftLists,rightLists,middlePassList,isLeftConditionMet, isRightConditionMet, isMiddleConditionMet,x,y,1);
	            }
	        }
	        middlePassList.clear();
	        x = 0;
	        y = 0;
	    }
	}

		    if (isRightConditionMet == false) {
		        vector<int> rightPassList = rightLists;
		        rightPassList.push_back(sortedCells[depth]);
		        if (isLeftConditionMet & isMiddleConditionMet ) {
		            if (rightPassList.size() == (split_high.size() - 2)) {
		                if ( equal(bestRightData.begin(), bestRightData.end(), rightPassList.begin()) ) {
		                    isRightConditionMet = true;
		                }
		            }
		        }
		     	
		        if (isRightConditionMet == false) {
		        
		        	x = parentX + (difference*n);
	                y = parentY + dy;
        			if (depth != max_depth - 1) {
        				nodeCount++;

	        			draw(x,y,2,parentX,(parentY));
        		}
		            lowBoundValue = getNumberOfCuts(leftLists,middleLists,rightPassList);
		            if (lowBoundValue >= (initialBestSolution)) {
		            } else {
		                buildTree(leftCount,(rightCount + 1),middleCount, (depth + 1),lowBoundValue,leftLists,rightPassList,middleLists,isLeftConditionMet,isRightConditionMet, isMiddleConditionMet,x,y,1);
		            }
		        }
		        rightPassList.clear();
		        x = 0;
		        y = 0;
	    }
    
    return;
}

void buildTreeUsingLowerBound(int leftCount, int rightCount, int middleCount, int depth, int lowerBoundValue, std::vector<int> leftLists, std::vector<int> rightLists, std::vector<int> middleLists,    bool isLeftConditionMet, bool isMiddleConditionMet,bool isRightConditionMet,float parentX, float parentY, int color) {
    
    int leftLowBoundValue = 0;
    int rightLowBoundValue = 0;
    int middleLowBoundValue = 0;

    int bestSolution = 0;
    vector<int> leftPassList = leftLists;
    vector<int> middlePassList = middleLists;
    vector<int> rightPassList = rightLists;
    vector<int> leftsorted;
    vector<int> sorted;
    vector<int> rightSorted;
    vector<int> middleSorted;
    vector<int> direction;

    std::unordered_map<int,int > mapSort;
	float x = 0;
    float y = 0;
    float n = pow(3,(max_depth - depth - 2));
    bool noMiddle = false;

    if ((depth == max_depth) & (lowerBoundValue <= initialBestSolution)) {
        if (leftLists.size() == split_low.size() & rightLists.size() == split_high.size() & middleLists.size() == split_medium.size() ) {
            bestSolution = lowerBoundValue;
            bestSolutionVector.push_back(bestSolution);
            cout << "Best Solution" << bestSolution;
            initialBestSolution = bestSolution;
            best.push_back(leftLists);
            best.push_back(middleLists);
            best.push_back(rightLists);
            leftLists.clear();
            rightLists.clear();
            middleLists.clear();
            sorted.clear();
        }
    }
    if (middleCount == 0 & rightCount == 0) {
    	noMiddle = true;

    }
    if (middleCount == 0 & leftCount == 0) {
    	noMiddle = true;

    }
    if (depth != max_depth) {
    	std::string s = std::to_string(static_cast<long long>(sortedCells[depth]));
        char const *pchar = s.c_str();
    	setcolor (BLACK);
        setfontsize (10);
        drawtext (parentX,parentY-30,pchar,1200.); 
        if (color == 1) 
    		draw(parentX,parentY,1,parentX,parentY);
    	else if (color == 2) {
    		draw(parentX,parentY,2,parentX,parentY);
    	} else {
    		draw(parentX,parentY,0,parentX,parentY);
    	}
	}			
    
    if ((leftCount >= split_low.size()) & (isLeftConditionMet == false)) {
        isLeftConditionMet = true;
    }
    if ((middleCount >= split_medium.size()) & (isMiddleConditionMet == false)) {
        isMiddleConditionMet = true;
    }
    if ((rightCount >= split_high.size()) & (isRightConditionMet == false)) {
        isRightConditionMet = true;
    }
    if (!isLeftConditionMet) {
        leftPassList.push_back(sortedCells[depth]);
        leftLowBoundValue = getNumberOfCuts(leftPassList,middleLists, rightLists);
        sorted.push_back(leftLowBoundValue);
    }
    if (!isMiddleConditionMet) {
        middlePassList.push_back(sortedCells[depth]);
        middleLowBoundValue = getNumberOfCuts(leftLists,middlePassList,rightLists);
        sorted.push_back(middleLowBoundValue);
    }
    if (!isRightConditionMet) {
        rightPassList.push_back(sortedCells[depth]);
        rightLowBoundValue = getNumberOfCuts(leftLists,middleLists,rightPassList);
        sorted.push_back(rightLowBoundValue);
    }
    std::sort(sorted.begin(), sorted.end());
    vector<int> sorting = sorted;
    for (int j = 0; j < sorting.size(); j++) {
        for (int i = 0; i < sorted.size(); i++) {
            if (sorted[i] == leftLowBoundValue) {
                mapSortIterator = mapSort.find(LEFT);
                if (mapSortIterator != mapSort.end()) {
                    //continue;
                } else {
                    mapSort[LEFT] = sorted[i];
                    direction.push_back(LEFT);
                    sorted.erase(sorted.begin() + i);
                    break;
                }
            }
            if (sorted[i] == middleLowBoundValue) {
                mapSortIterator = mapSort.find(MIDDLE);
                if (mapSortIterator != mapSort.end()) {
                    //continue;
                } else {
                    mapSort[MIDDLE] = sorted[i];
                    sorted.erase(sorted.begin() + i);
                    direction.push_back(MIDDLE);
                    break;
                }
            }
            if (sorted[i] == rightLowBoundValue) {
                mapSortIterator = mapSort.find(RIGHT);
                if (mapSortIterator != mapSort.end()) {
                    //continue;
                } else {
                mapSort[RIGHT] = sorted[i];
                sorted.erase(sorted.begin() + i);
                direction.push_back(RIGHT);
                break;
                }
            }
            
        }
    }
    if (direction.size() > 0) {
        if (direction[0] == LEFT) {
            if (isLeftConditionMet == false) {
            	x = parentX - (difference*n);
        		y = parentY + dy;
        		if (depth != max_depth - 1) {
        			nodeCount++;

	        			draw(x,y,2,parentX,(parentY));
        		}
                if (mapSort[LEFT] >= (initialBestSolution)) {
                } else {
                    buildTreeUsingLowerBound((leftCount+1) ,rightCount,middleCount, (depth+1),mapSort[LEFT],leftPassList, rightLists, middleLists,isLeftConditionMet,isMiddleConditionMet, isRightConditionMet,x,y,1);
                }
            }
            leftPassList.clear();
	        x = 0;
	        y = 0;
        }
        else if (direction[0] == MIDDLE) {
        	if (noMiddle == false) {
    			
	            if (isMiddleConditionMet == false) {
	            	x = parentX;
	            	y = parentY + dy;
	            	if (depth != max_depth - 1) {
        			nodeCount++;

	        			draw(x,y,2,parentX,(parentY));
        		}
	                if (mapSort[MIDDLE] >= (initialBestSolution)) {                   
	                } else {
	                    buildTreeUsingLowerBound(leftCount,rightCount,(middleCount +1), (depth+1),mapSort[MIDDLE],leftLists,rightLists,middlePassList,isLeftConditionMet,isMiddleConditionMet,isRightConditionMet,x,y,1);
	                }
	            }
	        }
	        middlePassList.clear();
	        x = 0;
	        y = 0;
        }
        else if (direction[0] == RIGHT) {
            if (isRightConditionMet == false) {
            	x = parentX + (difference*n);
	                y = parentY + dy;
	                if (depth != max_depth - 1) {
        			nodeCount++;

	        			draw(x,y,2,parentX,(parentY));
        		}
                if (mapSort[RIGHT] >= (initialBestSolution)) {
                } else {
                    buildTreeUsingLowerBound(leftCount,(rightCount + 1),middleCount, (depth + 1),mapSort[RIGHT],leftLists,rightPassList,middleLists,isLeftConditionMet,isMiddleConditionMet,isRightConditionMet,x,y,1);
                }
            }
            rightPassList.clear();
	        x = 0;
	        y = 0;
        }

    }
    if (direction.size() > 0)
	    direction.erase (direction.begin());
    
	    if (direction.size() > 0) {
	       if (direction[0] == MIDDLE) {
	       	    if (noMiddle == false) {
		            if (isMiddleConditionMet == false) {
		            	x = parentX;
                		y = parentY + dy;
                		if (depth != max_depth - 1) {
        			nodeCount++;

	        			draw(x,y,2,parentX,(parentY));
        		}
		                if (mapSort[MIDDLE] >= (initialBestSolution)) {
		                } else {
		                    buildTreeUsingLowerBound(leftCount,rightCount,(middleCount +1), (depth+1),mapSort[MIDDLE],leftLists,rightLists,middlePassList,isLeftConditionMet,isMiddleConditionMet,isRightConditionMet,x,y,1);
		                }
		            }
	        	}
	        	middlePassList.clear();
	        	x = 0;
	        	y = 0;
	        }
	       else if (direction[0] == LEFT) {
	            if (isLeftConditionMet == false) {
	            	x = parentX - (difference*n);
        			y = parentY + dy;
        			if (depth != max_depth - 1) {
        			nodeCount++;

	        			draw(x,y,2,parentX,(parentY));
        		}
	                if (mapSort[LEFT] >= (initialBestSolution)) {
	                } else {
	                    buildTreeUsingLowerBound((leftCount+1) ,rightCount,middleCount, (depth+1),mapSort[LEFT],leftPassList, rightLists, middleLists,isLeftConditionMet,isMiddleConditionMet,isRightConditionMet,x,y,1);
	                }
	            }
	            leftPassList.clear();
	        	x = 0;
	        	y = 0;
	        }
	    
	        else if (direction[0] == RIGHT) {
	            if (isRightConditionMet == false) {
	            	x = parentX + (difference*n);
	                y = parentY + dy;
	                if (depth != max_depth - 1) {
        			nodeCount++;

	        			draw(x,y,2,parentX,(parentY));
        		}
	                if (mapSort[RIGHT] >= (initialBestSolution)) {
	                } else {
	                    buildTreeUsingLowerBound(leftCount,(rightCount + 1),middleCount, (depth + 1),mapSort[RIGHT],leftLists,rightPassList,middleLists,isLeftConditionMet,isMiddleConditionMet,isRightConditionMet,x,y,1);
	                }
	            }
	            rightPassList.clear();
	        	x = 0;
	        	y = 0;
	        }
	    }
	
	if (direction.size() > 0)
	    direction.erase (direction.begin());

    if (direction.size() > 0) {
        if (direction[0] == RIGHT) {
            if (isRightConditionMet == false) {
            	x = parentX + (difference*n);
	                y = parentY + dy;
	                if (depth != max_depth - 1) {
        			nodeCount++;

	        			draw(x,y,2,parentX,(parentY));
        		}
                if (mapSort[RIGHT] >= (initialBestSolution)) {
                } else {
                    buildTreeUsingLowerBound(leftCount,(rightCount + 1),middleCount, (depth + 1),mapSort[RIGHT],leftLists,rightPassList,middleLists,isLeftConditionMet,isMiddleConditionMet,isRightConditionMet,x,y,1);
                }
            }
            rightPassList.clear();
	        x = 0;
	        y = 0;
        }
        else if (direction[0] == LEFT) {
            if (isLeftConditionMet == false) {
            		x = parentX - (difference*n);
        	y = parentY + dy;
        	if (depth != max_depth - 1) {
        			nodeCount++;

	        			draw(x,y,2,parentX,(parentY));
        		}

                if (mapSort[LEFT] >= (initialBestSolution)) {
                } else {
                    buildTreeUsingLowerBound((leftCount+1) ,rightCount,middleCount, (depth+1),mapSort[LEFT],leftPassList, rightLists, middleLists,isLeftConditionMet,isMiddleConditionMet,isRightConditionMet,x,y,1);
                }
            }
            leftPassList.clear();
	        x = 0;
	        y = 0;
        }
        else if (direction[0] == MIDDLE) {
        	if (noMiddle == false) {

	            if (isMiddleConditionMet == false) {
	            	x = parentX;
                	y = parentY + dy;
                	if (depth != max_depth - 1) {
        			nodeCount++;

	        			draw(x,y,2,parentX,(parentY));
        		}
	                if (mapSort[MIDDLE] >= (initialBestSolution)) {
	                } else {
	                    buildTreeUsingLowerBound(leftCount,rightCount,(middleCount +1), (depth+1),mapSort[MIDDLE],leftLists,rightLists,middlePassList,isLeftConditionMet,isMiddleConditionMet,isRightConditionMet,x,y,1);
	                }
	            }
	        }
	        middlePassList.clear();
	        x = 0;
	        y = 0;
        }
    
    }
    
    return;
    
}
vector<int> finalLeft;
vector<int> finalRight;
vector<int> finalMiddle;

int swapping(int best) {
    std::vector<int> left;
    std::vector<int> right;
    std::vector<int> middle;
    finalLeft.clear();
    finalRight.clear();
    finalMiddle.clear();
    left = split_low;
    right = split_high;
    middle = split_medium;
    int temp = 0;
    int value = 0;
    for (int i = 0; i < left.size(); i ++) {
        for (int j = 0; j < middle.size(); j++) {
            temp = left[i];
            left[i] = middle[j];
            middle[j] = temp;
            value = getNumberOfCuts(left,middle,right);
            if (value < best) {
                best = value;
                finalLeft = left;
                finalRight = right;
                finalMiddle = middle;
                cout << "value is" << value << "\n";

            }
            left = split_low;
            right = split_high;
            middle = split_medium;

        }
        
    }
    left = finalLeft;
    right = finalRight;
    middle = finalMiddle;
    for (int i = 0; i < left.size(); i ++) {
        for (int j = 0; j < right.size(); j++) {
            temp = left[i];
            left[i] = right[j];
            right[j] = temp;
            int value = getNumberOfCuts(left,middle,right);
            if (value < best) {
                best = value;
                finalLeft = left;
                finalRight = right;
                finalMiddle = middle;
                cout << "value is" << value << "\n";

            }
            left = split_low;
            right = split_high;
            middle = split_medium;
        }
    }
    left = finalLeft;
    middle = finalMiddle;
    right = finalRight;
    for (int i = 0; i < middle.size(); i ++) {
        for (int j = 0; j < right.size(); j++) {
            temp = middle[i];
            middle[i] = right[j];
            right[j] = temp;
            int value = getNumberOfCuts(left,middle,right);
            if (value < best) {
                best = value;
                finalLeft = left;
                finalRight = right;
                finalMiddle = middle;
                cout << "value is" << value << "\n";

            }
            left = split_low;
            right = split_high;
            middle = split_medium;
            
        }
    }
    left = finalLeft;
    middle = finalMiddle;
    right = finalRight;
    for (int i = 0; i < left.size(); i ++) {
        for (int j = 0; j < middle.size(); j++) {
            temp = left[i];
            left[i] = middle[j];
            middle[j] = temp;
            int value = getNumberOfCuts(left,middle,right);
            if (value < best) {
                best = value;
                finalLeft = left;
                finalRight = right;
                finalMiddle = middle;
                cout << "value is" << value << "\n";

            }
            left = split_low;
            right = split_high;
            middle = split_medium;
            
        }
    }
    left.clear();
    right.clear();
    middle.clear();
    return best;
}

void button_press (float x, float y) {
    /* Called whenever event_loop gets a button press in the graphics *
     * area.  Allows the user to do whatever he/she wants with button *
     * clicks.                                                        */
     printf("User clicked at coordinates (%f, %f)\n", x, y);
}



void drawscreen() {
    int color = 0;
    set_draw_mode (DRAW_NORMAL);    // Should set this if your program does any XOR drawing in callbacks.
    clearscreen();                  /* Should precede drawing for all drawscreens */
    /*setfontsize (10);
    setlinestyle (SOLID);
    setcolor(BLACK);
    setlinewidth (2);*/
}

void depthFirstCallback (void (*drawscreen_ptr) (void)) {
	nodeCount = 0;
	std::vector<int> left;
	auto started = std::chrono::high_resolution_clock::now();
    buildTree(0,0,0,0,initialBestSolution,left,left,left,false,false,false,750,40,1);
    auto done = std::chrono::high_resolution_clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count() << "\n";
    if (bestSolutionVector.size() > 0) {
        std::vector<int>::const_iterator it1 = std::min_element(bestSolutionVector.begin(), bestSolutionVector.end());
        cout << "Best solution is" << *it1;
        int length = std::distance<std::vector<int>::const_iterator>(bestSolutionVector.begin(), it1);
        vector<int> bestLeft;
        vector<int> bestMiddle;
        vector<int> bestRight;
        int start = 0;
        
        if (length == 0)
            start = 0;
        else
            start = 3*length;
        
        bestLeft = best[start];
        if (length == 0)
            start = 1;
        else
            start = 3*length + 1;
        
        bestMiddle = best[(start)];
        if (length == 0)
            start = 2;
        else
            start = 3*length + 2;
        
        bestRight = best[start];
        cout << "\n";
        cout << "left is " ;
        for (int i = 0; i < bestLeft.size(); i++) {
            cout << bestLeft[i] << ",";
        }
        cout << "\n";
        cout << "right is " ;

        for (int i = 0; i < bestRight.size(); i++) {
            cout << bestRight[i] << ",";

        }
        cout << "\n";
        cout << "middle is " ;
        for (int i = 0; i < bestMiddle.size(); i++) {
            cout << bestMiddle[i] << ",";

        }
    } else {
        cout << "Best solution is" << initialBestSolution;

    }
    cout << "\n";
    cout << "Node count is" << nodeCount << "\n";
}

void lowerBoundCallback (void (*drawscreen_ptr) (void)) { 
	nodeCount = 0;
	std::vector<int> left;
	auto started = std::chrono::high_resolution_clock::now();
	buildTreeUsingLowerBound(0,0,0,0,initialBestSolution,left,left,left,false,false,false,750,40,1);
	auto done = std::chrono::high_resolution_clock::now();
	std::cout << "Execeution time is " << std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count() << "\n";
    if (bestSolutionVector.size() > 0) {
        std::vector<int>::const_iterator it1 = std::min_element(bestSolutionVector.begin(), bestSolutionVector.end());
        cout << "Best solution is" << *it1;
        int length = std::distance<std::vector<int>::const_iterator>(bestSolutionVector.begin(), it1);
        vector<int> bestLeft;
        vector<int> bestMiddle;
        vector<int> bestRight;
        int start = 0;
        
        if (length == 0)
            start = 0;
        else
            start = 3*length;
        
        bestLeft = best[start];
        if (length == 0)
            start = 1;
        else
            start = 3*length + 1;
        
        bestMiddle = best[(start)];
        if (length == 0)
            start = 2;
        else
            start = 3*length + 2;
        
        bestRight = best[start];
        cout << "\n";
        cout << "left is " ;
        for (int i = 0; i < bestLeft.size(); i++) {
            cout << bestLeft[i] << ",";
        }
        cout << "\n";
        cout << "right is " ;

        for (int i = 0; i < bestRight.size(); i++) {
            cout << bestRight[i] << ",";

        }
        cout << "\n";
        cout << "middle is " ;
        for (int i = 0; i < bestMiddle.size(); i++) {
            cout << bestMiddle[i] << ",";

        }
    } else {
        cout << "Best solution is" << initialBestSolution;

    }
    cout << "\n";
    cout << "Node count is" << nodeCount << "\n";


}


int main(int argc, char *argv[]) {
    
    if (argc > 1) {
        dataParser(argv[1]);
    } else {
        cerr << "No filename mentioned" << "\n";
        exit(EXIT_FAILURE);
    }
    init_graphics("Placement", WHITE);
    for (int i = 0; i < allCells.size(); i++) {
        cout << "Cell number " << allCells[i] << ": ";
        for (int j = 0; j < mapOfBlockNet[allCells[i]].size(); j++) {
            cout << mapOfBlockNet[allCells[i]][j] << ", ";
        }
        
        cout << "\n";
    }
    netListIterator = std::max_element(netList.begin(), netList.end());
    max_value = *netListIterator;
    nodeCount = 0;
   	checkCellIfConnectedWithSameLabel(mapOfBlockNet);
    sortedCells = allCells;
    partition();
    int initial = getNumberOfCuts(split_low,split_medium,split_high);
    cout << "Initial is" << initial << "\n";
    sortedCells.clear();
    split_medium.clear();
    split_low.clear();
    split_high.clear();
    sortDecreasingOrder(mapOfBlockNet);
    partition();
    int decreasing = getNumberOfCuts(split_low,split_medium,split_high);
    cout << "decreasing is" << decreasing << "\n";
    std::vector<int>::const_iterator it = std::max_element(sortedCells.begin(), sortedCells.end());
    max_depth = *it;
    cout << "Max Depth is" << max_depth << "\n";
    sortedCells.clear();
    split_medium.clear();
    split_low.clear();
    split_high.clear();
    sortIncreasingOrder(mapOfBlockNet);
    partition();
    int increasing = getNumberOfCuts(split_low,split_medium,split_high);
    cout << "Increasing is " << increasing << "\n";
    if (increasing > decreasing) {
    	initialBestSolution = swapping(decreasing);
    	cout << "Swapping Decreasing is " << initialBestSolution << "\n";
    } else {
    	finalLeft.clear();
    	finalMiddle.clear();
    	finalRight.clear();
    	sortedCells.clear();
    	sortIncreasingOrder(mapOfBlockNet);
    	partition();
    	int increasing = getNumberOfCuts(split_low,split_medium,split_high);
    	initialBestSolution = swapping(increasing);
    	cout << "Swapping Increasing is " << initialBestSolution << "\n";
    }
    sortedCells.clear();
    /*for (int i = 0; i < finalLeft.size(); i++) {
        sortedCells.push_back(finalLeft[i]);
    }
    for (int i = 0; i < finalMiddle.size(); i++) {
        sortedCells.push_back(finalMiddle[i]);
    }
    for (int i = 0; i < finalRight.size(); i++) {
        sortedCells.push_back(finalRight[i]);
    }*/
    //sortedCells = allCells;
    //sortIncreasingOrder(mapOfBlockNet);
   	sortDecreasingOrder(mapOfBlockNet);
    difference = 1500/(pow(3,max_depth - 1) + 1);
    dy = 1500/max_depth;
    init_world (0.,0.,1500.,1500.);
    update_message("Partitioning");
    create_button ("Proceed", "DepthFirst", depthFirstCallback);
    create_button("DepthFirst","LowerBound", lowerBoundCallback);
 	event_loop(button_press,NULL, NULL,drawscreen);
   	close_graphics(); 
}
