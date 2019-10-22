#---Makefile for BetaScope Driver



########## defaut ##########

CC      = g++ --std=c++17 --std=c++1z --std=gnu++1z -fPIC -no-pie
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

GENERAL_DIR = ../General/

COLORFUL_COUT_DIR = $(GENERAL_DIR)Colorful_Cout/

WAVEFORMANA_DIR = $(GENERAL_DIR)WaveformAna/
WAVEFORMANA_BUILD_DIR = $(WAVEFORMANA_DIR)/build/


################################################################################
# BetaScope_Driver
################################################################################
BETASCOPE_DRIVER_DIR = ./
BETASCOPE_DRIVER_SRC_DIR = $(BETASCOPE_DRIVER_DIR)
BETASCOPE_DRIVER_BUILD_DIR = $(BETASCOPE_DRIVER_DIR)
BETASCOPE_DRIVER_INC_DIR = $(BETASCOPE_DRIVER_DIR)
BETASCOPE_DRIVER_SRC_FILES = $(wildcard $(BETASCOPE_DRIVER_SRC_DIR)/*.cpp)
BETASCOPE_DRIVER_DEP = $(patsubst $(BETASCOPE_DRIVER_SRC_DIR)/%.cpp, $(BETASCOPE_DRIVER_BUILD_DIR)/%.o, $(BETASCOPE_DRIVER_SRC_FILES))
#BETASCOPE_DRIVER_SRC_FILES = $(wildcard $(BETASCOPE_DRIVER_SRC_DIR)/*.c)
#BETASCOPE_DRIVER_DEP += $(patsubst $(BETASCOPE_DRIVER_SRC_DIR)/%.c, $(BETASCOPE_DRIVER_BUILD_DIR)/%.o, $(BETASCOPE_DRIVER_SRC_FILES))
#BETA_SCOPE_DRIVER_DEP += $(patsubst $(BETA_SCOPE_DRIVER_SRC_DIR)/%.c, $(BETA_SCOPE_DRIVER_BUILD_DIR)/%.o, $(BETA_SCOPE_DRIVER_SRC_FILES))


################################################################################
# Building object files.
################################################################################

# $(info [${BETASCOPE_DRIVER_INC_DIR}] )

all: $(BETASCOPE_DRIVER_DEP) main_getResults

$(BETASCOPE_DRIVER_BUILD_DIR)/%.o: $(BETASCOPE_DRIVER_SRC_DIR)/%.cpp
	$(CC) $(DEBUG) -I./ -I../ -I$(GENERAL_DIR) -I$(BETASCOPE_DRIVER_INC_DIR) -c $^ -o $@ $(ROOT_LINKS) $(ROOT_LIBS) $(ROOFIT) $(ROOT_INCLUDE) $(BOOST_LIB)

bin/getResults: main_getResults.c
	$(CC) $(DEBUG) $(LFLAGS) -I./ -I../ -I$(GENERAL_DIR) -I$(BETASCOPE_DRIVER_INC_DIR) $^ -o $@ $(BETASCOPE_DRIVER_DEP) $(ROOT_LINKS) $(ROOT_LIBS) $(ROOFIT) $(ROOT_INCLUDE) $(BOOST_LIB)
################################################################################
# cleanning object files.
################################################################################
.PHONY: clean
clean:
	@echo "cleaning .o files in build and binary files in bin."
	@for file in ./*.o; do rm -rf $$file; echo Delete Object file: $$file; done
	@for file in ./bin/*; do rm -rf $$file; echo Delete Object file: $$file; done
	@echo "finished"
