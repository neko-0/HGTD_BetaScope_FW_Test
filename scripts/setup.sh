echo "Setting up BetaScope Framwork"

current_path=`pwd`
export BETASCOPE_SCRIPTS=${current_path}
export BETASCOPE_ANALYSIS_DIR=${current_path}/../BetaScope_Ana/
export BETASCOPE=${current_path}/../

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
cd ${current_path}

echo "going to betaScopePlot folder"
cd betaScopePlot
make

cd ${current_path}
echo "creating user data direcotry"
mkdir user_data
mkdir user_data/bkup
echo "setting up alias"
alias run_WaveformConfig='${BETASCOPE_SCRIPTS}/../BetaScope_Ana/BetaScopeWaveformAna/bin/GenerateWaveformConfig'
alias run_WaveformAna='${BETASCOPE_SCRIPTS}/../BetaScope_Ana/BetaScopeWaveformAna/bin/Run_WaveformAna WaveformAnaConfig.ini --skipWaveform'
alias run_RunConfig='${BETASCOPE_SCRIPTS}/betaScopePlot/bin/genPlotConfig'
alias run_GetResults='${BETASCOPE_SCRIPTS}/betaScopePlot/bin/getResults run_info_v08022018.ini'
alias run_beta_excel='python2 ${BETASCOPE_SCRIPTS}/betaScope_pyScript/parseBetaResultsToExcel.py -task '

function LGAD(){
  if [ "$1" = "--test" ]
  then
    python ${BETASCOPE_SCRIPTS}/meow.py
  else
    python ${BETASCOPE_SCRIPTS}/lgad.py
  fi
}

#cd ~
