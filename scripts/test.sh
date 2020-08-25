wget --load-cookies /tmp/cookies.txt "https://docs.google.com/uc?export=download&confirm=$(wget --quiet --save-cookies /tmp/cookies.txt --keep-session-cookies --no-check-certificate 'https://docs.google.com/uc?export=download&id=1qCYdSienjzc9QlKpNPY3AJHNplcROWNr' -O- | sed -rn 's/.*confirm=([0-9A-Za-z_]+).*/\1\n/p')&id=1qCYdSienjzc9QlKpNPY3AJHNplcROWNr" -O run_test.zip && rm -rf /tmp/cookies.tx

unzip run_test.zip
mkdir raw

mv test_run/*root raw/
mv test_run/* .

./BetaScope_Ana/BetaScopeWaveformAna/bin/GenerateWaveformConfig

sed -i 's/channel_2 = 0/channel_2 = 1/g' WaveformAnaConfig.ini
sed -i '0,/channel_3 = 0/s//channel_3 = 1/' WaveformAnaConfig.ini

./BetaScope_Ana/BetaScopeWaveformAna/bin/Run_WaveformAna -config WaveformAnaConfig.ini

${BETASCOPE_SCRIPTS}/betaScopePlot/bin/genPlotConfig
python3 $BETASCOPE_SCRIPTS/betaScope_pyScript/autoCut_v2.py --runNum 801
${BETASCOPE_SCRIPTS}/betaScopePlot/bin/getResults run_info_v${RUN_INFO_VER}.ini


