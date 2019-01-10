ECE1387 - CAD: Assignment 2 - Analytical Placement
Soumil Chugh

Compiling the Placer:
On ECF systems, the placer can be compiled using 'make' on the command line.

Folder Structure:
The root directory is named as Placement. This folder contains the makefile and the test files. The executable of the placer is formed here. In addition to these files, there are 
3 folders:src, ref and the bin.
The ref folder contains the SuitSparse build files
The src folder contains the .cpp and .h files of the analytical placement algorithm
The bin folder gets created on compiling and contains the object files that are used to 
Create the executable.

Rebuilding the Placer:
To rebuild the placer executable file, run 'make clean'.
This will remove the object files and the executable file.
To build the placer again, run 'make'

Running the Placer:
To run the placer, run './ap <circuit>' where <circuit> is a test file, formatted as described in the assignment handout.
 
Changing Weights:

To change the weights, inside the src folder, you will find the placer.cpp file.The assignWeights function has a if condition that checks if the net is connected to a virtual block or not. Here is the variable weight is set to 1. Change this value to any other value. Please note it will be changed at two places in the same function. Once this is done, rebuild the placer.

Graphical Buttons:
Spreading - Each click of spreading button runs one spread iteration. The button will stop responding once stopping criteria is reached.
Remove Nets - This removes the nets from the display
Add Nets - This adds the nets back in display
Snap - This snaps the cells in slots with no type constraint
Snap Slot - This snaps the cells in slots with type constraints



 