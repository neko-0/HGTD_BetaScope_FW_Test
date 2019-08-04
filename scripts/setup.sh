echo "Setting up BetaScope Framwork"

current_path=`pwd`
export BETASCOPE_SCRIPTS=${current_path}

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
echo "setting up alias"
alias meow="python -i ${BETASCOPE_SCRIPTS}/meow.py"
alias run_WaveformConfig='${BETASCOPE_SCRIPTS}/../BetaScope_Ana/BetaScopeWaveformAna/bin/GenerateWaveformConfig'
alias run_WaveformAna='${BETASCOPE_SCRIPTS}/../BetaScope_Ana/BetaScopeWaveformAna/bin/Run_WaveformAna WaveformAnaConfig.ini --skipWaveform'
alias run_RunConfig='/home/yuzhan/HGTD_BetaScope/BetaScopeDataProcessor/bin/GenerateDataProcessorConfig.exe'
alias run_GetResults='/home/yuzhan/HGTD_BetaScope/BetaScopeDataProcessor/bin/GetResults.exe run_info_v08022018.ini'

cd ~