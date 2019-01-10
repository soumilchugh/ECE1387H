#include <iostream>
#include <vector>
#include "common.h"
#include <math.h>
#include <sstream>
#include <fstream>
using namespace std;

std::vector<int> data;

void dataParser(const char* filename) {
    ifstream in_file;
    in_file.open(filename);
    if (!in_file.is_open()) {
      cerr << "Cannot open file - " << filename << "please check file exists\n";
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
                int cell = value;
                allCells.push_back(cell);
                while(iss >> value) {
                    if (value != -1){
                        data.push_back(value);
                        netList.push_back(value);
                    }
                }
                mapOfBlockNet[cell] = data;
                data.clear();                
            }
        }
    }
}
