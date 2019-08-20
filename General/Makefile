#---Makefile for the General directory



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
ROOFIT       = -lRooFit -lHtml -lMinuit -lRooFitCore -lRooStats
ROOT_LIBS    = -L${ROOTSYS}/lib -lTreePlayer -lCore
ROOT_INCLUDE = -I${ROOTSYS}/include
BOOST_LIB    = -L/usr/include/boost/property_tree -L/usr/include/boost/ -lboost_thread -lboost_system


################################################################################
# Colorful_Cout
################################################################################
COLORFUL_COUT_DIR = Colorful_Cout/
COLORFUL_COUT_SRC_DIR = $(COLORFUL_COUT_DIR)src/
COLORFUL_COUT_BUILD_DIR = $(COLORFUL_COUT_DIR)build/
COLORFUL_COUT_INC_DIR = $(COLORFUL_COUT_DIR)include/
COLORFUL_COUT_SRC_FILES = $(wildcard $(COLORFUL_COUT_SRC_DIR)/*.cpp)
COLORFUL_COUT_SRC_FILES += $(wildcard $(COLORFUL_COUT_SRC_DIR)/*.c)
COLORFUL_COUT_DEP = $(patsubst $(COLORFUL_COUT_SRC_DIR)/%.cpp, $(COLORFUL_COUT_BUILD_DIR)/%.o, $(COLORFUL_COUT_SRC_FILES))
#COLORFUL_COUT_DEP += $(patsubst $(COLORFUL_COUT_SRC_DIR)/%.c, $(COLORFUL_COUT_BUILD_DIR)/%.o, $(COLORFUL_COUT_SRC_FILES))

################################################################################
# utilities
################################################################################
UTILITIES_DIR = utilities/
UTILITIES_SRC_DIR = $(UTILITIES_DIR)src/
UTILITIES_BUILD_DIR = $(UTILITIES_DIR)build/
UTILITIES_INC_DIR = $(UTILITIES_DIR)include/
UTILITIES_SRC_FILES = $(wildcard $(UTILITIES_SRC_DIR)/*.cpp)
UTILITIES_SRC_FILES += $(wildcard $(UTILITIES_SRC_DIR)/*.c)
UTILITIES_DEP = $(patsubst $(UTILITIES_SRC_DIR)/%.cpp, $(UTILITIES_BUILD_DIR)/%.o, $(UTILITIES_SRC_FILES))
#COLORFUL_COUT_DEP += $(patsubst $(COLORFUL_COUT_SRC_DIR)/%.c, $(COLORFUL_COUT_BUILD_DIR)/%.o, $(COLORFUL_COUT_SRC_FILES))


################################################################################
# Waveform Ana
################################################################################
WAVEFORM_ANA_DIR = WaveformAna/
WAVEFORM_ANA_SRC_DIR = $(WAVEFORM_ANA_DIR)src/
WAVEFORM_ANA_BUILD_DIR = $(WAVEFORM_ANA_DIR)build/
WAVEFORM_ANA_INC_DIR = $(WAVEFORM_ANA_DIR)include/
WAVEFORM_ANA_SRC_FILES = $(wildcard $(WAVEFORM_ANA_SRC_DIR)/*.cpp)
WAVEFORM_ANA_SRC_FILES += $(wildcard $(WAVEFORM_ANA_SRC_DIR)/*.c)
WAVEFORM_ANA_DEP = $(patsubst $(WAVEFORM_ANA_SRC_DIR)/%.cpp, $(WAVEFORM_ANA_BUILD_DIR)/%.o, $(WAVEFORM_ANA_SRC_FILES))


################################################################################
# Building object files.
################################################################################

all: $(WAVEFORM_ANA_DEP)\
	$(COLORFUL_COUT_DEP)\
	$(UTILITIES_DEP)

$(COLORFUL_COUT_BUILD_DIR)/%.o: $(COLORFUL_COUT_SRC_DIR)/%.cpp
	$(CC) $(DEBUG) -I./ -I$(COLORFUL_COUT_INC_DIR) -I$(WAVEFORM_ANA_DIR) -c $^ -o $@ $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE) $(BOOST_LIB)

$(WAVEFORM_ANA_BUILD_DIR)/%.o: $(WAVEFORM_ANA_SRC_DIR)/%.cpp
	$(CC) $(DEBUG) -I./ -I$(COLORFUL_COUT_INC_DIR) -I$(WAVEFORM_ANA_DIR) -I$(WAVEFORM_ANA_INC_DIR) -c $^ -o $@ $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE) $(BOOST_LIB)

$(UTILITIES_BUILD_DIR)/%.o: $(UTILITIES_SRC_DIR)/%.cpp
	$(CC) $(DEBUG) -I./ -I$(UTILITIES_INC_DIR) -I$(UTILITIES_DIR) -c $^ -o $@



################################################################################
# cleanning object files.
################################################################################
.PHONY: clean
clean:
	@echo "cleaning .o files in build and binary files in bin."
	@for file in ./*/build/*; do rm -rf $$file; echo Delete Object file: $$file; done
	@for file in ./*/*/build/*; do rm -rf $$file; echo Delete Object file: $$file; done
	@for file in ./bin/*; do rm -rf $$file; echo Delete binary file: $$file; done
	@for file in ./*/*/bin/*; do rm -rf $$file; echo Delete Object file: $$file; done
	@echo "finished"
