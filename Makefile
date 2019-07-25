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

CORE_DIR = ./
GENERAL_LIBS_DIR = General/

################################################################################
# BetaScope_Driver
################################################################################
BETA_SCOPE_DRIVER_SRC_DIR = BetaScope_Driver/src/
BETA_SCOPE_DRIVER_BUILD_DIR = BetaScope_Driver/build/
BETA_SCOPE_DRIVER_INC_DIR = BetaScope_Driver/include/
BETA_SCOPE_DRIVER_SRC_FILES = $(wildcard $(BETA_SCOPE_DRIVER_SRC_DIR)/*.cpp)
BETA_SCOPE_DRIVER_SRC_FILES += $(wildcard $(BETA_SCOPE_DRIVER_SRC_DIR)/*.c)
BETA_SCOPE_DRIVER_DEP = $(patsubst $(BETA_SCOPE_DRIVER_SRC_DIR)/%.cpp, $(BETA_SCOPE_DRIVER_BUILD_DIR)/%.o, $(BETA_SCOPE_DRIVER_SRC_FILES))
#BETA_SCOPE_DRIVER_DEP += $(patsubst $(BETA_SCOPE_DRIVER_SRC_DIR)/%.c, $(BETA_SCOPE_DRIVER_BUILD_DIR)/%.o, $(BETA_SCOPE_DRIVER_SRC_FILES))

################################################################################
# Colorful_Cout
################################################################################
COLORFUL_COUT_SRC_DIR = $(GENERAL_LIBS_DIR)Colorful_Cout/src/
COLORFUL_COUT_BUILD_DIR = $(GENERAL_LIBS_DIR)Colorful_Cout/build/
COLORFUL_COUT_INC_DIR = $(GENERAL_LIBS_DIR)Colorful_Cout/include/
COLORFUL_COUT_SRC_FILES = $(wildcard $(COLORFUL_COUT_SRC_DIR)/*.cpp)
COLORFUL_COUT_SRC_FILES += $(wildcard $(COLORFUL_COUT_SRC_DIR)/*.c)
COLORFUL_COUT_DEP = $(patsubst $(COLORFUL_COUT_SRC_DIR)/%.cpp, $(COLORFUL_COUT_BUILD_DIR)/%.o, $(COLORFUL_COUT_SRC_FILES))
#COLORFUL_COUT_DEP += $(patsubst $(COLORFUL_COUT_SRC_DIR)/%.c, $(COLORFUL_COUT_BUILD_DIR)/%.o, $(COLORFUL_COUT_SRC_FILES))

################################################################################
# Waveform Ana
################################################################################
WAVEFORM_ANA_DIR = $(GENERAL_LIBS_DIR)WaveformAna/
WAVEFORM_ANA_SRC_DIR = $(GENERAL_LIBS_DIR)WaveformAna/src/
WAVEFORM_ANA_BUILD_DIR = $(GENERAL_LIBS_DIR)WaveformAna/build/
WAVEFORM_ANA_INC_DIR = $(GENERAL_LIBS_DIR)WaveformAna/include/
WAVEFORM_ANA_SRC_FILES = $(wildcard $(WAVEFORM_ANA_SRC_DIR)/*.cpp)
WAVEFORM_ANA_SRC_FILES += $(wildcard $(WAVEFORM_ANA_SRC_DIR)/*.c)
WAVEFORM_ANA_DEP = $(patsubst $(WAVEFORM_ANA_SRC_DIR)/%.cpp, $(WAVEFORM_ANA_BUILD_DIR)/%.o, $(WAVEFORM_ANA_SRC_FILES))


################################################################################
# BetaScope_Ana
################################################################################
BETA_SCOPE_ANA = BetaScope_Ana/

#--BetaScopeWaveformAna
BETA_SCOPE_WAVEFORM_ANA_DIR = $(BETA_SCOPE_ANA)BetaScopeWaveformAna/
BETA_SCOPE_WAVEFORM_ANA_TEMPATE = $(BETA_SCOPE_ANA)BetaScopeWaveformAna/AnaTemplate/
BETA_SCOPE_WAVEFORM_ANA_BUILD_DIR = $(BETA_SCOPE_ANA)BetaScopeWaveformAna/build/
BETA_SCOPE_WAVEFORM_ANA_FILES = $(wildcard $(BETA_SCOPE_WAVEFORM_ANA_TEMPATE)/*.cpp)
BETA_SCOPE_WAVEFORM_ANA_DEP = $(patsubst $(BETA_SCOPE_WAVEFORM_ANA_TEMPATE)/%.cpp, $(BETA_SCOPE_WAVEFORM_ANA_BUILD_DIR)/%.o, $(BETA_SCOPE_WAVEFORM_ANA_FILES))
BETA_SCOPE_WAVEFORM_ANA_MAIN = $(BETA_SCOPE_ANA)BetaScopeWaveformAna/main/
BETA_SCOPE_WAVEFORM_ANA_BIN = $(BETA_SCOPE_ANA)BetaScopeWaveformAna/bin/

################################################################################
# Building object files.
################################################################################
$(BETA_SCOPE_DRIVER_BUILD_DIR)/%.o: $(BETA_SCOPE_DRIVER_SRC_DIR)/%.cpp
	$(CC) $(DEBUG) -I$(CORE_DIR) -I$(GENERAL_LIBS_DIR) -I$(BETA_SCOPE_DRIVER_INC_DIR) -I$(COLORFUL_COUT_INC_DIR) -c $^ -o $@ $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE) $(BOOST_LIB)

$(COLORFUL_COUT_BUILD_DIR)/%.o: $(COLORFUL_COUT_SRC_DIR)/%.cpp
	$(CC) $(DEBUG) -I$(CORE_DIR) -I$(GENERAL_LIBS_DIR) -I$(COLORFUL_COUT_INC_DIR) -c $^ -o $@ $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE) $(BOOST_LIB)

$(WAVEFORM_ANA_BUILD_DIR)/%.o: $(WAVEFORM_ANA_SRC_DIR)/%.cpp
	$(CC) $(DEBUG) -I$(CORE_DIR) -I$(GENERAL_LIBS_DIR) -I$(WAVEFORM_ANA_DIR) -I$(BETA_SCOPE_WAVEFORM_ANA_DIR) -I$(BETA_SCOPE_DRIVER_INC_DIR) -I$(COLORFUL_COUT_INC_DIR) -c $^ -o $@ $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE) $(BOOST_LIB)

$(BETA_SCOPE_WAVEFORM_ANA_BUILD_DIR)/%.o: $(BETA_SCOPE_WAVEFORM_ANA_TEMPATE)/%.cpp
	$(CC) $(DEBUG) -I$(CORE_DIR) -I$(GENERAL_LIBS_DIR) -I$(BETA_SCOPE_WAVEFORM_ANA_DIR) -I$(BETA_SCOPE_DRIVER_INC_DIR) -I$(COLORFUL_COUT_INC_DIR) -c $^ -o $@ $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE) $(BOOST_LIB)




#===============================================================================
# Make exe

all: $(BETA_SCOPE_WAVEFORM_ANA_BIN)GenerateWaveformConfig \
	$(BETA_SCOPE_WAVEFORM_ANA_BIN)Run_WaveformAna

DEPS = $(BETA_SCOPE_DRIVER_DEP)
DEPS += $(COLORFUL_COUT_DEP)
DEPS += $(WAVEFORM_ANA_DEP)
DEPS += $(BETA_SCOPE_WAVEFORM_ANA_DEP)
$(BETA_SCOPE_WAVEFORM_ANA_BIN)GenerateWaveformConfig: $(BETA_SCOPE_WAVEFORM_ANA_MAIN)GenerateWaveformConfig.cpp $(DEPS)
	$(CC) $(DEBUG) $(LFLAGS) -I$(CORE_DIR) -I$(GENERAL_LIBS_DIR) -I$(WAVEFORM_ANA_DIR) -I$(BETA_SCOPE_ANA) -I$(BETA_SCOPE_WAVEFORM_ANA_DIR) -I$(BETA_SCOPE_DRIVER_INC_DIR) -I$(COLORFUL_COUT_INC_DIR) $^ -o $@ $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE) $(BOOST_LIB)

DEPS = $(BETA_SCOPE_DRIVER_DEP)
DEPS += $(COLORFUL_COUT_DEP)
DEPS += $(WAVEFORM_ANA_DEP)
DEPS += $(BETA_SCOPE_WAVEFORM_ANA_DEP)
$(BETA_SCOPE_WAVEFORM_ANA_BIN)Run_WaveformAna: $(BETA_SCOPE_WAVEFORM_ANA_MAIN)Run_WaveformAna.cpp $(DEPS)
	$(CC) $(DEBUG) $(LFLAGS) -I$(CORE_DIR) -I$(GENERAL_LIBS_DIR) -I$(WAVEFORM_ANA_DIR) -I$(BETA_SCOPE_ANA) -I$(BETA_SCOPE_WAVEFORM_ANA_DIR) -I$(BETA_SCOPE_DRIVER_INC_DIR) -I$(COLORFUL_COUT_INC_DIR) $^ -o $@ $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE) $(BOOST_LIB)


#================================================================================================
#================================================================================================

.PHONY: clean
clean:
	@echo "cleaning .o files in build and binary files in bin."
	@for file in ./*/build/*; do rm -rf $$file; echo Delete Object file: $$file; done
	@for file in ./*/*/build/*; do rm -rf $$file; echo Delete Object file: $$file; done
	@for file in ./bin/*; do rm -rf $$file; echo Delete binary file: $$file; done
	@for file in ./*/*/bin/*; do rm -rf $$file; echo Delete Object file: $$file; done
	@echo "finished"
