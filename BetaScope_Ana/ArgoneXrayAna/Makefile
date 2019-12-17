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

UTILITIES_INC = $(GENERAL_DIR)/utilities/include/
UTILITIES_DEPS = $(wildcard $(GENERAL_DIR)/utilities/build/*.o)

COLORFUL_COUT_INC = $(GENERAL_DIR)/Colorful_Cout/include/

DEPS = $(BETASCOPE_DRIVER_DEPS)
DEPS += $(WAVEFORM_ANA_DEPS)
DEPS += $(UTILITIES_DEPS)


#--ArgoneXrayAna
ARGONE_XRAY_ANA = ./
ARGONE_XRAY_ANA_DIR = $(BETASCOPE_ANA)/
ARGONE_XRAY_ANA_TEMPATE = $(ARGONE_XRAY_ANA)/AnaTemplate/
ARGONE_XRAY_ANA_BUILD_DIR = $(ARGONE_XRAY_ANA)/build/
ARGONE_XRAY_ANA_FILES = $(wildcard $(ARGONE_XRAY_ANA_TEMPATE)/*.cpp)
ARGONE_XRAY_ANA_DEP = $(patsubst $(ARGONE_XRAY_ANA_TEMPATE)/%.cpp, $(ARGONE_XRAY_ANA_BUILD_DIR)/%.o, $(ARGONE_XRAY_ANA_FILES))
ARGONE_XRAY_ANA_MAIN = $(ARGONE_XRAY_ANA)/main/
ARGONE_XRAY_ANA_BIN = $(ARGONE_XRAY_ANA)/bin/

ARGONE_DEPS = $(BETASCOPE_DRIVER_DEPS)
ARGONE_DEPS += $(WAVEFORM_ANA_DEPS)
ARGONE_DEPS += $(ARGONE_XRAY_ANA_DEP)


################################################################################
# Building object files.
################################################################################

$(ARGONE_XRAY_ANA_BUILD_DIR)/%.o: $(ARGONE_XRAY_ANA_TEMPATE)/%.cpp
	$(CC) $(DEBUG) -I./ -I../ -I$(BETASCOPE) -I$(GENERAL_DIR) -I$(BETASCOPE_DRIVER) -I$(ARGONE_XRAY_ANA_TEMPATE) -c $^ -o $@ $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE) $(BOOST_LIB)



#===============================================================================
# Make exe

all: $(ARGONE_XRAY_ANA_BIN)ArgoneTest

$(ARGONE_XRAY_ANA_BIN)ArgoneTest: $(ARGONE_XRAY_ANA_MAIN)ArgoneTest.cpp $(ARGONE_DEPS) $(UTILITIES_DEPS) $(WAVEFORM_ANA_DEPS)
	$(CC) $(DEBUG) $(LFLAGS) -I./ -I../ -I$(BETASCOPE) -I$(GENERAL_DIR) -I$(BETASCOPE_DRIVER) -I$(BETASCOPE_DRIVER_INC) -I$(ARGONE_XRAY_ANA_TEMPATE) $^ -o $@ -lstdc++fs $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE) $(BOOST_LIB)



#================================================================================================
#================================================================================================

.PHONY: clean
clean:
	@echo "cleaning .o files in build and binary files in bin."
	@for file in $$BETASCOPE_ANALYSIS_DIR/ArgoneXrayAna/build/*; do rm -rf $$file; echo Delete Object file: $$file; done
	@for file in $$BETASCOPE_ANALYSIS_DIR/ArgoneXrayAna/bin/*; do rm -rf $$file; echo Delete binary file: $$file; done
	@echo "finished"
