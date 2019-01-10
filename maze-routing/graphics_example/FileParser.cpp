#include <stdio.h>
#include <iostream>
#include <vector>
#include "common.h"
#include <math.h>
using namespace std;
int sourceXcoord, sourceYcoord, sourcePinNum;
int sinkXcoord, sinkYcoord, sinkPinNum;
int switchBlockGridSize;
int logicBlockGridSize;
std::vector< std::pair<int,int> > xycordinatelistSourceInitial; // Holds the list of x and y of source
std::vector< std::pair<int,int> > xycordinatelistSinkInitial;       // Holds the list of x and y of sink
std::vector <int> sourcePinList;
std::vector <int> sinkPinList;
std::vector<float> listOfDistance;


int findDistance() {
    float maxDistance = 0;
    float currentDistance = 0;
    float dist;
    int index = 0;
    int xSink, ySink, xSource, ySource;
    int size = xycordinatelistSourceInitial.size();
    for (int i = 0; i < size; i ++) { 
        xSink = xycordinatelistSinkInitial[i].first;         // get the x and y cordinates of sink
        ySink = xycordinatelistSinkInitial[i].second;
        xSource = xycordinatelistSourceInitial[i].first;     // get the x and y coridinates of source
        ySource = xycordinatelistSourceInitial[i].second;
        dist = pow((xSink - xSource), 2) + pow((ySink - ySource), 2);       //calculating Euclidean distance
        currentDistance = sqrt(dist);    
        listOfDistance.push_back(currentDistance); 
    }
}


void sortMaxList() {
    int count = 0;
    count = listOfDistance.size();
    while (count != 0) {
        std::vector<float>::const_iterator it = std::max_element(listOfDistance.begin(), listOfDistance.end());
        int length = std::distance<std::vector<float>::const_iterator>(listOfDistance.begin(), it);
        xycordinatelistSource.push_back(std::make_pair(xycordinatelistSourceInitial[length].first,xycordinatelistSourceInitial[length].second));
        xycordinatelistSink.push_back(std::make_pair(xycordinatelistSinkInitial[length].first,xycordinatelistSinkInitial[length].second));
        xycordinatelistSinkInitial.erase (xycordinatelistSinkInitial.begin()+length);
        xycordinatelistSourceInitial.erase(xycordinatelistSourceInitial.begin()+length);
        count --;
        listOfDistance.erase(listOfDistance.begin() + length);
    }

}

void sortMinList() {
    int count = 0;
    count = listOfDistance.size();
    while (count != 0) {
        std::vector<float>::const_iterator it = std::min_element(listOfDistance.begin(), listOfDistance.end());
        int length = std::distance<std::vector<float>::const_iterator>(listOfDistance.begin(), it);
        xycordinatelistSource.push_back(std::make_pair(xycordinatelistSourceInitial[length].first,xycordinatelistSourceInitial[length].second));
        xycordinatelistSink.push_back(std::make_pair(xycordinatelistSinkInitial[length].first,xycordinatelistSinkInitial[length].second));
        xycordinatelistSinkInitial.erase (xycordinatelistSinkInitial.begin()+length);
        xycordinatelistSourceInitial.erase(xycordinatelistSourceInitial.begin()+length);
        count --;
        listOfDistance.erase(listOfDistance.begin() + length);
    }

}

void standardList() {
    xycordinatelistSource = xycordinatelistSourceInitial;
    xycordinatelistSink = xycordinatelistSinkInitial;
}


void dataParser(const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (fp == NULL)
        perror ("Error opening file");

    fscanf(fp, "%d", &logicBlockGridSize);
    switchBlockGridSize = logicBlockGridSize + 1;
    fscanf(fp, "%d", &initialwidthOfchannel);
    while ((fscanf(fp, "%d %d %d %d %d %d", &sourceXcoord, &sourceYcoord, &sourcePinNum, &sinkXcoord, &sinkYcoord, &sinkPinNum) != EOF)
           && (sourceXcoord != -1) && (sourceYcoord != -1) && (sourcePinNum != -1) && (sinkXcoord != -1) && (sinkYcoord != -1) && (sinkPinNum != -1)) {
        xycordinatelistSourceInitial.push_back(std::make_pair(sourceXcoord,sourceYcoord));
        xycordinatelistSinkInitial.push_back(std::make_pair(sinkXcoord,sinkYcoord));
        sourcePinList.push_back(sourcePinNum);
        sinkPinList.push_back(sinkPinNum);
    }
    findDistance(); 
}