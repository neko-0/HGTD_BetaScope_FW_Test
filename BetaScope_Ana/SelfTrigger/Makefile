#---Makefile for BetaScope Analysis template



########## defaut ##########

CC      = g++ -std=c++17 -fPIC -no-pie -funroll-loops -ftree-vectorizer-verbose=1
DEBUG   = -g -Wall
CFLAGS  = -c
CFLAGS_NO_C  = -Wall $(DEBUG)
#LFLAGS  =

#-I/usr/include/glib-2.0 -I/usr/lib64/glib-2.0/include
################################################################################
# ROOT Libs and links
################################################################################
GLIB         = `pkg-config --cflags --libs glib-2.0`
ROOT_LINKS   = `root-config --cflags --glibs`
ROOFIT       = -lRooFit -lHtml -lMinuit -lRooFitCore -lRooStats
ROOT_LIBS    = -L${ROOTSYS}/lib -lTreePlayer -lCore
ROOT_INCLUDE = -I${ROOTSYS}/include
BOOST_LIB    = -L/usr/include/boost/property_tree -L/usr/include/boost/ -lboost_thread -lboost_system

GENERAL_DIR = $(BETASCOPE)/General/
BETASCOPE_DRIVER = $(BETASCOPE)/BetaScope_Driver/

BETASCOPE_DRIVER_INC = $(BETASCOPE_DRIVER)/include/
BETASCOPE_DRIVER_DEPS = $(wildcard $(BETASCOPE_DRIVER)/build/*.o)

WAVEFORM_ANA_INC = $(GENERAL_DIR)/WaveformAna/include/
WAVEFORM_ANA_DEPS = $(wildcard $(GENERAL_DIR)/WaveformAna/build/*.o)
WAVEFORM_ANA_DEPS += $(wildcard $(GENERAL_DIR)/WaveformAna/build/*.so)

UTILITIES_INC = $(GENERAL_DIR)/utilities/include/
UTILITIES_DEPS = $(wildcard $(GENERAL_DIR)/utilities/build/*.o)

LOGGER_INC = $(GENERAL_DIR)/logger/include/
LOGGER_DEPS = $(wildcard $(GENERAL_DIR)/logger/build/*.o)

DEPS = $(BETASCOPE_DRIVER_DEPS)
DEPS += $(WAVEFORM_ANA_DEPS)
DEPS += $(UTILITIES_DEPS)
DEPS += $(LOGGER_DEPS)


#--SelfTrigger
SelfTrigger_ANA = $(BETASCOPE_ANALYSIS_DIR)/SelfTrigger/
SelfTrigger_ANA_DIR = $(SelfTrigger_ANA)/
SelfTrigger_ANA_TEMPATE = $(SelfTrigger_ANA)/AnaTemplate/
SelfTrigger_ANA_BUILD_DIR = $(SelfTrigger_ANA)/build/
SelfTrigger_ANA_FILES = $(wildcard $(SelfTrigger_ANA_TEMPATE)/*.cpp)
SelfTrigger_ANA_DEP = $(patsubst $(SelfTrigger_ANA_TEMPATE)/%.cpp, $(SelfTrigger_ANA_BUILD_DIR)/%.o, $(SelfTrigger_ANA_FILES))
SelfTrigger_ANA_MAIN = $(SelfTrigger_ANA)/main/
SelfTrigger_ANA_BIN = $(SelfTrigger_ANA)/bin/

DEPS += $(SelfTrigger_ANA_DEP)



################################################################################
# Building object files.
################################################################################

$(SelfTrigger_ANA_BUILD_DIR)/%.o: $(SelfTrigger_ANA_TEMPATE)/%.cpp
	$(CC) $(DEBUG) -I./ -I../ -I$(BETASCOPE) -I$(GENERAL_DIR) -I$(BETASCOPE_DRIVER) -I$(SelfTrigger_ANA_TEMPATE) -c $^ -o $@ $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE) $(BOOST_LIB)


#===============================================================================
# Make exe

all: $(SelfTrigger_ANA_BIN)Run_SelfTrigger \

$(SelfTrigger_ANA_BIN)Run_SelfTrigger: $(SelfTrigger_ANA_MAIN)Run_SelfTrigger.cpp $(DEPS)
	$(CC) $(DEBUG) $(LFLAGS) -I./ -I../ -I$(BETASCOPE_ANALYSIS_DIR) -I$(BETASCOPE) -I$(GENERAL_DIR) -I$(BETASCOPE_DRIVER) -I$(SelfTrigger_ANA_TEMPATE) $^ -o $@ -lstdc++fs $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE) $(BOOST_LIB)



#================================================================================================
#================================================================================================

.PHONY: clean
clean:
	@echo "cleaning .o files in build and binary files in bin."
	@for file in $$BETASCOPE_ANALYSIS_DIR/SelfTrigger/build/*; do rm -rf $$file; echo Delete Object file: $$file; done
	@for file in $$BETASCOPE_ANALYSIS_DIR/SelfTrigger/bin/*; do rm -rf $$file; echo Delete binary file: $$file; done
	@echo "finished"
