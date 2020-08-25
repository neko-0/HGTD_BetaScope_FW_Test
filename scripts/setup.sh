#!/bin/bash 

echo "Setting up BetaScope Framwork"

current_path=`pwd`
export BETASCOPE_SCRIPTS=${current_path}
export BETASCOPE_ANALYSIS_DIR=$(dirname ${current_path})/BetaScope_Ana/
export BETASCOPE=$(dirname ${current_path})

echo "going to General folder"
cd ../General
make -j
cd ${current_path}

echo "going to BetaScope Driver folder"
cd ../BetaScope_Driver
make -j
cd ${current_path}

echo "going to BetaScope Ana folder"
cd ../BetaScope_Ana
make -j
for f in */setup.sh
  do
    source $f
done
cd ${current_path}

echo "going to betaScopePlot folder"
cd betaScopePlot
make

export RUN_INFO_VER="2020_7_14"
cd ${current_path}
echo "creating user data direcotry"
mkdir user_data
mkdir user_data/bkup
echo "setting up alias"
alias run_RunConfig='${BETASCOPE_SCRIPTS}/betaScopePlot/bin/genPlotConfig'
alias run_GetResults='${BETASCOPE_SCRIPTS}/betaScopePlot/bin/getResults run_info_v${RUN_INFO_VER}.ini'
alias run_beta_excel='python2 ${BETASCOPE_SCRIPTS}/betaScope_pyScript/parseBetaResultsToExcel.py -task '

export BETA_PLOT_JOBS=$BETASCOPE_SCRIPTS/betaScopePlot/src/plot_list.json

function LGAD(){
  if [ "$1" = "--test" ]
  then
    python ${BETASCOPE_SCRIPTS}/meow.py
  else
    python ${BETASCOPE_SCRIPTS}/lgad.py
  fi
}

echo "::set-env name=BETASCOPE_SCRIPTS::$BETASCOPE_SCRIPTS"
echo "::set-env name=BETASCOPE_ANALYSIS_DIR::$BETASCOPE_ANALYSIS_DIR"
echo "::set-env name=BETASCOPE::$BETASCOPE"
echo "::set-env name=RUN_INFO_VER::$RUN_INFO_VER"

#cd ~
