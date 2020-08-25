#---Makefile for BetaScope Driver



########## defaut ##########

CC      = g++ -std=c++17 -fPIC -no-pie
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
ROOFIT       = -lRooFit -lMinuit -lRooFitCore -lRooStats -lHtml
ROOT_LIBS    = -L${ROOTSYS}/lib -lTreePlayer -lCore
ROOT_INCLUDE = -I${ROOTSYS}/include
BOOST_LIB    = -L/usr/include/boost/property_tree -L/usr/include/boost/ -lboost_thread -lboost_system -fopenmp

GENERAL_DIR = ../General/

COLORFUL_COUT_DIR = $(GENERAL_DIR)Colorful_Cout/

WAVEFORMANA_DIR = $(GENERAL_DIR)WaveformAna/
WAVEFORMANA_BUILD_DIR = $(WAVEFORMANA_DIR)/build/


BIN = ./bin/
################################################################################
# BETASCOPE_PLOTTER
################################################################################
SCRIPTS_DIR = ../
BETASCOPE_PLOTTER_DIR = $(SCRIPTS_DIR)/betaScopePlot/
BETASCOPE_PLOTTER_SRC_DIR = $(BETASCOPE_PLOTTER_DIR)/src/
BETASCOPE_PLOTTER_BIN_DIR = $(BETASCOPE_PLOTTER_DIR)/bin/
BETASCOPE_PLOTTER_BUILD_DIR = $(BETASCOPE_PLOTTER_DIR)build/
BETASCOPE_PLOTTER_INC_DIR = $(BETASCOPE_PLOTTER_DIR)/include/
BETASCOPE_PLOTTER_SRC_FILES = $(wildcard $(BETASCOPE_PLOTTER_SRC_DIR)/*.cpp)
BETASCOPE_PLOTTER_DEP = $(patsubst $(BETASCOPE_PLOTTER_SRC_DIR)/%.cpp, $(BETASCOPE_PLOTTER_BUILD_DIR)/%.o, $(BETASCOPE_PLOTTER_SRC_FILES))
#BETASCOPE_PLOTTER_SRC_FILES = $(wildcard $(BETASCOPE_PLOTTER_SRC_DIR)/*.c)
#BETASCOPE_PLOTTER_DEP += $(patsubst $(BETASCOPE_PLOTTER_SRC_DIR)/%.c, $(BETASCOPE_PLOTTER_BUILD_DIR)/%.o, $(BETASCOPE_PLOTTER_SRC_FILES))
#BETA_SCOPE_DRIVER_DEP += $(patsubst $(BETA_SCOPE_DRIVER_SRC_DIR)/%.c, $(BETA_SCOPE_DRIVER_BUILD_DIR)/%.o, $(BETA_SCOPE_DRIVER_SRC_FILES))


################################################################################
# Building object files.
################################################################################

# $(info [${BETASCOPE_PLOTTER_INC_DIR}] )

all: $(BETASCOPE_PLOTTER_DEP) \
	$(BETASCOPE_PLOTTER_BIN_DIR)/getResults \
	$(BETASCOPE_PLOTTER_BIN_DIR)/genPlotConfig

$(BETASCOPE_PLOTTER_BUILD_DIR)/%.o: $(BETASCOPE_PLOTTER_SRC_DIR)/%.cpp
	$(CC) $(DEBUG) -Iinclude -I$(SCRIPTS_DIR) -I$(BETASCOPE_PLOTTER_DIR) -I$(BETASCOPE_PLOTTER_INC_DIR) -I$(GENERAL_DIR) -c $^ -o $@ $(ROOT_LINKS) $(ROOT_LIBS) $(ROOFIT) $(ROOT_INCLUDE) $(BOOST_LIB) -lfmt

$(BETASCOPE_PLOTTER_BIN_DIR)/getResults: $(BETASCOPE_PLOTTER_DIR)/main/main_getResults.cpp $(BETASCOPE_PLOTTER_DEP)
	$(CC) $(DEBUG) -Iinclude -I$(SCRIPTS_DIR) -I$(BETASCOPE_PLOTTER_DIR) -I$(BETASCOPE_PLOTTER_INC_DIR) -I$(GENERAL_DIR) $^ -o $@ $(ROOT_LINKS) $(ROOT_LIBS) $(ROOFIT) $(ROOT_INCLUDE) $(BOOST_LIB) -lfmt -lstdc++fs

$(BETASCOPE_PLOTTER_BIN_DIR)/genPlotConfig: $(BETASCOPE_PLOTTER_DIR)/main/main_genPlotConfig.cpp $(BETASCOPE_PLOTTER_DEP)
	$(CC) $(DEBUG) -Iinclude -I$(SCRIPTS_DIR) -I$(BETASCOPE_PLOTTER_DIR) -I$(BETASCOPE_PLOTTER_INC_DIR) -I$(GENERAL_DIR) $^ -o $@ $(ROOT_LINKS) $(ROOT_LIBS) $(ROOFIT) $(ROOT_INCLUDE) $(BOOST_LIB) -lfmt

################################################################################
# cleanning object files.
################################################################################
.PHONY: clean
clean:
	@echo "cleaning .o files in build and binary files in bin."
	@for file in ./build/*.o; do rm -rf $$file; echo Delete Object file: $$file; done
	@for file in ./bin/*; do rm -rf $$file; echo Delete Object file: $$file; done
	@echo "finished"
