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
# LOGGER
################################################################################
LOGGER_DIR = logger/
LOGGER_SRC_DIR = $(LOGGER_DIR)src/
LOGGER_BUILD_DIR = $(LOGGER_DIR)build/
LOGGER_INC_DIR = $(LOGGER_DIR)include/
LOGGER_SRC_FILES = $(wildcard $(LOGGER_SRC_DIR)/*.cpp)
LOGGER_SRC_FILES += $(wildcard $(LOGGER_SRC_DIR)/*.c)
LOGGER_DEP = $(patsubst $(LOGGER_SRC_DIR)/%.cpp, $(LOGGER_BUILD_DIR)/%.o, $(LOGGER_SRC_FILES))
#LOGGER_DEP += $(patsubst $(LOGGER_SRC_DIR)/%.c, $(LOGGER_BUILD_DIR)/%.o, $(LOGGER_SRC_FILES))

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
#LOGGER_DEP += $(patsubst $(LOGGER_SRC_DIR)/%.c, $(LOGGER_BUILD_DIR)/%.o, $(LOGGER_SRC_FILES))

################################################################################
# Trigger
################################################################################
TRIGGER_DIR = Trigger/
TRIGGER_SRC_DIR = $(TRIGGER_DIR)src/
TRIGGER_BUILD_DIR = $(TRIGGER_DIR)build/
TRIGGER_INC_DIR = $(TRIGGER_DIR)include/
TRIGGER_SRC_FILES = $(wildcard $(TRIGGER_SRC_DIR)/*.cpp)
TRIGGER_SRC_FILES += $(wildcard $(TRIGGER_SRC_DIR)/*.c)
TRIGGER_DEP = $(patsubst $(TRIGGER_SRC_DIR)/%.cpp, $(TRIGGER_BUILD_DIR)/%.o, $(TRIGGER_SRC_FILES))
#LOGGER_DEP += $(patsubst $(LOGGER_SRC_DIR)/%.c, $(LOGGER_BUILD_DIR)/%.o, $(LOGGER_SRC_FILES))


################################################################################
# Waveform Ana
################################################################################
WAVEFORM_ANA_DIR = WaveformAna/
WAVEFORM_ANA_SRC_DIR = $(WAVEFORM_ANA_DIR)src/
WAVEFORM_ANA_BUILD_DIR = $(WAVEFORM_ANA_DIR)build/
WAVEFORM_ANA_INC_DIR = $(WAVEFORM_ANA_DIR)include/
WAVEFORM_ANA_SRC_FILES = $(wildcard $(WAVEFORM_ANA_SRC_DIR)/*.cpp)
WAVEFORM_ANA_SRC_FILES += $(wildcard $(WAVEFORM_ANA_SRC_DIR)/*.c)
WAVEFORM_ANA_SRC_FILES += $(wildcard $(WAVEFORM_ANA_SRC_DIR)/*.cxx)
WAVEFORM_ANA_DEP = $(patsubst $(WAVEFORM_ANA_SRC_DIR)/%.cpp, $(WAVEFORM_ANA_BUILD_DIR)/%.o, $(WAVEFORM_ANA_SRC_FILES))
WAVEFORM_ANA_DEP += $(patsubst $(WAVEFORM_ANA_SRC_DIR)/%.cxx, $(WAVEFORM_ANA_BUILD_DIR)/%.o, $(WAVEFORM_ANA_SRC_FILES))

WAVEFORM_ANA_DEP += $(patsubst $(WAVEFORM_ANA_SRC_DIR)/%.cxx, $(WAVEFORM_ANA_BUILD_DIR)/%.Dict.cxx, $(WAVEFORM_ANA_SLIB_SRC_FILES))
WAVEFORM_ANA_DEP += $(patsubst $(WAVEFORM_ANA_SRC_DIR)/%.cxx, $(WAVEFORM_ANA_BUILD_DIR)/%.Dict.so, $(WAVEFORM_ANA_SLIB_SRC_FILES))
WAVEFORM_ANA_DEP += $(patsubst $(WAVEFORM_ANA_SRC_DIR)/%.cxx, $(WAVEFORM_ANA_BUILD_DIR)/%.Dict, $(WAVEFORM_ANA_SLIB_SRC_FILES))


WAVEFORM_ANA_DIR = WaveformAna/
WAVEFORM_ANA_SLIB_SRC_FILES += $(wildcard $(WAVEFORM_ANA_SRC_DIR)/*.cxx)
WAVEFORM_ANA_SLIB_DEP = $(patsubst $(WAVEFORM_ANA_SRC_DIR)/%.cxx, $(WAVEFORM_ANA_BUILD_DIR)/%.o, $(WAVEFORM_ANA_SLIB_SRC_FILES))
WAVEFORM_ANA_SLIB_INC = $(patsubst $(WAVEFORM_ANA_SRC_DIR)/%.cxx, $(WAVEFORM_ANA_INC_DIR)/%.hpp, $(WAVEFORM_ANA_SLIB_SRC_FILES))

WAVEFORM_SLIB_FILES = $(patsubst $(WAVEFORM_ANA_SRC_DIR)/%.cxx, $(WAVEFORM_ANA_BUILD_DIR)/%.Dict.so, $(WAVEFORM_ANA_SLIB_SRC_FILES))
WAVEFORM_SLIB_DICT_FILES = $(patsubst $(WAVEFORM_ANA_SRC_DIR)/%.cxx, $(WAVEFORM_ANA_BUILD_DIR)/%.Dict.cxx, $(WAVEFORM_ANA_SLIB_SRC_FILES))

################################################################################
# Building object files.
################################################################################

all:$(WAVEFORM_ANA_DEP)\
	$(LOGGER_DEP)\
	$(UTILITIES_DEP) \
	$(TRIGGER_DEP)



$(WAVEFORM_ANA_BUILD_DIR)/%.Dict.cxx: $(WAVEFORM_ANA_INC_DIR)/%.hpp $(WAVEFORM_ANA_INC_DIR)/%_LinkDef.h
	rootcint -f $@ -c $^

$(WAVEFORM_ANA_BUILD_DIR)/%.o: $(WAVEFORM_ANA_SLIB_SRC_FILES)/%.cxx $(LOGGER_DEP)
	$(CC) $(DEBUG) -I./ -I../ -I$(LOGGER_INC_DIR) -I$(WAVEFORM_ANA_DIR) -c $^ -o $@ $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE) $(BOOST_LIB)

$(WAVEFORM_ANA_BUILD_DIR)/%.Dict: $(WAVEFORM_ANA_BUILD_DIR)/%.Dict.cxx
	$(CC) $(DEBUG) -I./ -I../ -I$(LOGGER_INC_DIR) -I$(WAVEFORM_ANA_DIR) -c $^ -o $@ $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE) $(BOOST_LIB)

$(WAVEFORM_ANA_BUILD_DIR)/%.Dict.so: $(WAVEFORM_ANA_BUILD_DIR)/%.o $(WAVEFORM_ANA_BUILD_DIR)/%.Dict
	$(CC) -shared $(DEBUG) -I./ -I../ -I$(LOGGER_INC_DIR) -I$(WAVEFORM_ANA_DIR)  $^ -o $@ $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE) $(BOOST_LIB)


$(WAVEFORM_ANA_BUILD_DIR)/%.o: $(WAVEFORM_ANA_SRC_DIR)/%.cpp
	$(CC) $(DEBUG) -I./ -I../ -I$(LOGGER_INC_DIR) -I$(WAVEFORM_ANA_DIR) -I$(WAVEFORM_ANA_INC_DIR) -c $^ -o $@ $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE) $(BOOST_LIB)

$(WAVEFORM_ANA_BUILD_DIR)/%.o: $(WAVEFORM_ANA_SRC_DIR)/%.cxx
	$(CC) $(DEBUG) -I./ -I../ -I$(LOGGER_INC_DIR) -I$(WAVEFORM_ANA_DIR) -I$(WAVEFORM_ANA_INC_DIR) -c $^ -o $@ $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE) $(BOOST_LIB)




$(LOGGER_BUILD_DIR)/%.o: $(LOGGER_SRC_DIR)/%.cpp
	$(CC) $(DEBUG) -I./ -I../ -I$(LOGGER_INC_DIR) -I$(WAVEFORM_ANA_DIR) -c $^ -o $@ $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE) $(BOOST_LIB)

$(UTILITIES_BUILD_DIR)/%.o: $(UTILITIES_SRC_DIR)/%.cpp
	$(CC) $(DEBUG) -I./ -I../ -I$(UTILITIES_INC_DIR) -I$(UTILITIES_DIR) -c $^ -o $@

$(TRIGGER_BUILD_DIR)/%.o: $(TRIGGER_SRC_DIR)/%.cpp
	$(CC) $(DEBUG) -I./ -I../ -I$(TRIGGER_INC_DIR) -I$(TRIGGER_DIR) -c $^ -o $@ $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE) $(BOOST_LIB)


################################################################################
# cleanning object files.
################################################################################
.PHONY: clean
clean:
	@echo "cleaning .o files in build and binary files in bin."
	@for file in ./*/build/*; do rm -rf $$file; echo Delete Object file: $$file; done
	@for file in ./*/*/build/*.Dict*; do rm -rf $$file; echo Delete Object file: $$file; done
	@for file in ./*/*/build/*; do rm -rf $$file; echo Delete Object file: $$file; done
	@for file in ./bin/*; do rm -rf $$file; echo Delete binary file: $$file; done
	@for file in ./*/*/bin/*; do rm -rf $$file; echo Delete Object file: $$file; done
	@echo "finished"
