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
BOOST_LIB    = -L/usr/include/boost/property_tree -L/usr/include/boost/ -L/usr/include/boost/asio/ -lboost_thread -lboost_system -lboost_program_options

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


#--BetaScopeWaveformAna
BETASCOPE_ANA = $(BETASCOPE_ANALYSIS_DIR)/BetaScopeWaveformAna/
BETASCOPE_WAVEFORM_ANA_DIR = $(BETASCOPE_ANA)/
BETASCOPE_WAVEFORM_ANA_TEMPATE = $(BETASCOPE_ANA)/AnaTemplate/
BETASCOPE_WAVEFORM_ANA_BUILD_DIR = $(BETASCOPE_ANA)/build/
BETASCOPE_WAVEFORM_ANA_FILES = $(wildcard $(BETASCOPE_WAVEFORM_ANA_TEMPATE)/*.cpp)
BETASCOPE_WAVEFORM_ANA_DEP = $(patsubst $(BETASCOPE_WAVEFORM_ANA_TEMPATE)/%.cpp, $(BETASCOPE_WAVEFORM_ANA_BUILD_DIR)/%.o, $(BETASCOPE_WAVEFORM_ANA_FILES))
BETASCOPE_WAVEFORM_ANA_MAIN = $(BETASCOPE_ANA)/main/
BETASCOPE_WAVEFORM_ANA_BIN = $(BETASCOPE_ANA)/bin/

DEPS += $(BETASCOPE_WAVEFORM_ANA_DEP)



################################################################################
# Building object files.
################################################################################

$(BETASCOPE_WAVEFORM_ANA_BUILD_DIR)/%.o: $(BETASCOPE_WAVEFORM_ANA_TEMPATE)/%.cpp
	$(CC) $(DEBUG) -I./ -I../ -I$(BETASCOPE) -I$(GENERAL_DIR) -I$(BETASCOPE_DRIVER) -I$(BETASCOPE_WAVEFORM_ANA_TEMPATE) -c $^ -o $@ $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE) $(BOOST_LIB)


#===============================================================================
# Make exe

all: $(BETASCOPE_WAVEFORM_ANA_BIN)GenerateWaveformConfig \
	$(BETASCOPE_WAVEFORM_ANA_BIN)Run_WaveformAna \


$(BETASCOPE_WAVEFORM_ANA_BIN)GenerateWaveformConfig: $(BETASCOPE_WAVEFORM_ANA_MAIN)GenerateWaveformConfig.cpp $(DEPS)
	$(CC) $(DEBUG) $(LFLAGS) -I./ -I../ -I$(BETASCOPE_ANALYSIS_DIR) -I$(BETASCOPE) -I$(GENERAL_DIR) -I$(BETASCOPE_DRIVER) -I$(BETASCOPE_WAVEFORM_ANA_TEMPATE) $^ -o $@ -lstdc++fs $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE) $(BOOST_LIB)

$(BETASCOPE_WAVEFORM_ANA_BIN)Run_WaveformAna: $(BETASCOPE_WAVEFORM_ANA_MAIN)Run_WaveformAna.cpp $(DEPS)
	$(CC) $(DEBUG) $(LFLAGS) -I./ -I../ -I$(BETASCOPE_ANALYSIS_DIR) -I$(BETASCOPE) -I$(GENERAL_DIR) -I$(BETASCOPE_DRIVER) -I$(BETASCOPE_WAVEFORM_ANA_TEMPATE) $^ -o $@ -lstdc++fs $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE) $(BOOST_LIB)



#================================================================================================
#================================================================================================

.PHONY: clean
clean:
	@echo "cleaning .o files in build and binary files in bin."
	@for file in $$BETASCOPE_ANALYSIS_DIR/BetaScopeWaveformAna/build/*; do rm -rf $$file; echo Delete Object file: $$file; done
	@for file in $$BETASCOPE_ANALYSIS_DIR/BetaScopeWaveformAna/bin/*; do rm -rf $$file; echo Delete binary file: $$file; done
	@echo "finished"
