#include <stdio.h>
#include <iostream>
#include <vector>
#include "common.h"
#include <math.h>
#include <sstream>
#include <fstream>
using namespace std;

std::vector< std::pair<int,int>> cellType;
std::vector<int> fixedCellType;
std::vector<int> edges;
std::vector<int> weights;
std::vector<int> allCells;
std::vector<int> moveableCells;
std::vector<int> fixedCells;
std::vector<int> netList;
bool isFixedCells = false;
std::vector<int> data;
std::vector< std::pair<double,double> > xycordinatelist;
double xCordinate,yCordinate;

void findMoveableCells() {
    for (int i = 0; i < allCells.size(); ++i) {
        if (std::find(fixedCells.begin(), fixedCells.end(), allCells[i]) == fixedCells.end()) {
            moveableCells.push_back(allCells[i]);
        }
    }
}

void dataParser(const char* filename) {
    ifstream in_file;
    in_file.open(filename);
    if (!in_file.is_open()) {
      cerr << "Cannot open file - " << filename << "; please check file exists\n";
      exit(EXIT_FAILURE);
   }
   string line;
    while(getline(in_file,line)) {
        istringstream iss(line);
        double value;
        if (!(iss >> value)) {
            cerr << "I/O Error: A input line does not start with an int... exiting...\n";
            exit(EXIT_FAILURE);
        } else {
            data.clear();
            if (value != -1) {
                if (!isFixedCells) {
                    int moveableCell = value;
                    allCells.push_back(moveableCell);
                    iss >> value;
                    cellType.push_back(std::make_pair(moveableCell, value));
                    while(iss >> value) {
                        if (value != -1){
                            data.push_back(value);
                            netList.push_back(value);
                        }
                    }
                    mapOfBlockNet[moveableCell] = data;
                    data.clear();
                    moveableCell = 0;
                } else {
                     fixedCells.push_back(value);
                     iss >> value;
                     xCordinate = value;
                     iss >> value;
                     yCordinate = value;
                     xycordinatelist.push_back(std::make_pair(xCordinate,yCordinate));
                } 
            } else {
                isFixedCells = true;
            }
        }
    }
    findMoveableCells();
}