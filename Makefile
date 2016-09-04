MAINNAME= ID

#########################################################
# compiler and its flags 
#########################################################
CXX= g++

CXXFLAGS= -Wall -Wvla -Werror -std=c++11 

CXXFLAGSDEMO= $(CXXFLAGS) -O -DNDEBUG

CXXFLAGSTEST= $(CXXFLAGS) -g -D_GLIBCXX_DEBUG -DID_GOOGLE_TEST
CXXLINKFLAGSTEST= -lgtest -lgtest_main -pthread
#########################################################

##########################################################
# sources files
##########################################################
SRCSDEMO= ./IDdemo.cpp

SRCSTEST= ./Tests/TestGrid.cpp ./Tests/TestID.cpp ./Tests/TestIDpair.cpp ./Tests/TestGridsOfGroups.cpp ./Tests/TestIDgroupsBase.cpp ./Tests/TestIDgroups.cpp ./Tests/TestIDgroupsPair.cpp

HEADERFILES= Grid.hpp IDgroupsBase.hpp IDgroupsPair.hpp IDpair.hpp GridsOfGroups.hpp IDgroups.hpp ID.hpp Pair.hpp
##########################################################


#######################
# executables name
#######################
TEST= $(MAINNAME)test
DEMO= $(MAINNAME)demo
#######################


#########################################################
# actions
#########################################################
all: $(DEMO)

$(DEMO): $(SRCSDEMO) $(HEADERFILES)
	$(CXX) $(CXXFLAGSDEMO) $(SRCSDEMO) -o $@ 

$(TEST): $(SRCSTEST) $(HEADERFILES)
	$(CXX) $(CXXFLAGSTEST) $(SRCSTEST) -o $@ $(CXXLINKFLAGSTEST) 
	./$@

clean:
	rm *.o $(DEMO) $(TEST) -f
#########################################################


##############################################
# installs that should be done once
##############################################
installonce: gtestinstall makedependinstall

gtestinstall:
	sudo apt-get update
	sudo apt-get install libgtest-dev
	sudo apt-get install cmake
	cd /usr/src/gtest; \
	sudo cmake CMakeLists.txt; \
	sudo make; \
	sudo cp *.a /usr/lib
	sudo chmod ugoa+r /usr/lib/libgtest.a
	sudo chmod ugoa+r /usr/lib/libgtest_main.a
##############################################

.PHONY: all clean installonce gtestinstall
