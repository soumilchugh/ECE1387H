# Pick a platform below.  X11 for Linux/Mac/Unix, and WIN32 for windows.
# In either X11 or WIN32, Postscript is also simultaneously available.
# You can also pick NO_GRAPHICS, which will allow your code to compile without
# change on any platform, but no graphics will display (all calls are stubbed
# out).
# 
# Compiling to support X11 requires the X11 development libraries. On Ubuntu, use
# 'sudo apt-get install libx11-dev' to install.
# Please look online for information on how to install X11 on other Linux distributions.

# Please note that a Mac can run the X11 graphics if the X11 library is installed.

PLATFORM = X11
#PLATFORM = WIN32
#PLATFORM = NO_GRAPHICS

HDR = graphics.h easygl_constants.h
SRC = graphics.cpp FileParser.cpp biPartition.cpp
EXE = partition
BACKUP_FILENAME=`date "+backup-%Y%m%d-%H%M.zip"`
FLAGS = -g -Wall -std=c++0x -D$(PLATFORM)

# Need to tell the linker to link to the X11 libraries.
# WIN32 automatically links to the win32 API libraries (no need for flags)
ifeq ($(PLATFORM),X11)
   GRAPHICS_LIBS = -L/opt/X11/lib -lX11
endif

$(EXE): graphics.o FileParser.o biPartition.o
	g++ $(FLAGS) graphics.o FileParser.o biPartition.o $(GRAPHICS_LIBS) -o $(EXE)

graphics.o: graphics.cpp $(HDR)
	g++ -c $(FLAGS) graphics.cpp

FileParser.o: FileParser.cpp $(HDR)
	g++ -c $(FLAGS) FileParser.cpp

biPartition.o: biPartition.cpp $(HDR)
	g++ -c $(FLAGS) biPartition.cpp

backup:
# Back up the source, makefile and Visual Studio project & solution. 
	echo Backing up your files into ${BACKUP_FILENAME}
	zip ${BACKUP_FILENAME} $(SRC) $(HDR) makefile easygl.sln easygl.vcxproj

clean:
	rm $(EXE) *.o
